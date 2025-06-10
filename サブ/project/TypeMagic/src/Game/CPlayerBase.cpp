//プレイヤークラスのインクルード
#include "CPlayerBase.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "Maths.h"
#include "CSceneManager.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CGaugeUI2D.h"

// プレイヤーのインスタンス
CPlayerBase* CPlayerBase::spInstance = nullptr;

#define THRESHOLD 0.1f

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

// コンストラクタ
CPlayerBase::CPlayerBase()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mMotionBlurRemainTime(0.0f)
	, mpBodyCol(nullptr)
	, mpAttackCol(nullptr)
	, mpHpGauge(nullptr)
	, mIsGameEnd(false)
{
	mpTestModel = CResourceManager::Get<CModel>("TestModel");
	spInstance = this;

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);
}

// デストラクタ
CPlayerBase::~CPlayerBase()
{
	// コライダ―を削除
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpAttackCol);

	// 経路探索用のノードを削除
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
	}

	// インスタンスと削除しているプレイヤーが同一なら削除
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	// HPゲージが存在したら、一緒に削除する
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

CPlayerBase* CPlayerBase::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayerBase::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// アニメーションブレンド切り替え
bool CPlayerBase::ChangeAnimationBlend(int type, float blendSpeed, bool restart)
{
	bool end = false;
	if (mpAnimData == nullptr) return end;
	if (!(0 <= type && type < mpAnimData->size())) return end;
	AnimData data = (*mpAnimData)[type];
	end = CXCharacter::ChangeAnimationBlend
	(
		type,
		data.loop,
		data.frameLength,
		restart,
		blendSpeed
	);
	CXCharacter::SetAnimationSpeed(data.speed);
	return end;
}

// キーの入力情報から移動ベクトルを求める
CVector CPlayerBase::CalcMoveVec()
{
	CVector move = CVector::zero;

	// キーの入力ベクトルを取得
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// 入力ベクトルの長さで入力されているか判定
	if (input.LengthSqr() > 0.0f)
	{
		// 上方向ベクトル(設置している場合は、地面の法線)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
		// カメラの向きに合わせた移動ベクトルに変換
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// カメラの正面方向ベクトルと上方向ベクトルの外積から
		// 横方向の移動ベクトルを求める
		CVector moveSide = CVector::Cross(up, camForward);
		// 横方向の移動ベクトルと上方向ベクトルの外積から
		// 正面方向の移動ベクトルを求める
		CVector moveForward = CVector::Cross(moveSide, up);

		// 求めた各方向の移動ベクトルから、
		// 最終的なプレイヤーの移動ベクトルを求める
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}

// オブジェクト削除処理
void CPlayerBase::DeleteObject(CObjectBase* obj)
{
	// 削除されたオブジェクトがHpゲージであれば
	// ポインタを空にする
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

// 更新
void CPlayerBase::Update()
{
	// キャラクターの更新
	CXCharacter::Update();

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}


#if _DEBUG
	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("Pos:%f,%f,%f\n", Position().X(), Position().Y(), Position().Z());
	CDebugPrint::ForcePrint("FPS:%f\n", Times::FPS());
#endif

	mIsGrounded = false;
}

/*
衝突処理
本体コライダと地形の衝突処理
*/
void CPlayerBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体コライダとの衝突判定
	if (self == mpBodyCol)
	{
		// 衝突した相手がフィールドの場合
		if (other->Layer() == ELayer::eGround)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、床と衝突した
			if (dot >= 0.0f)
			{
				// 落下などで床に上から衝突したとき（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else if (dot < 0.0f)
			{
				// ジャンプなどで天井にしたから衝突したとき（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// 衝突した相手が壁の場合
		else if (other->Layer() == ELayer::eWall)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
		// 衝突した相手がオブジェクトだった場合
		else if (other->Layer() == ELayer::eObject)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			// 衝突した面が上か下かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、上面と衝突した
			if (dot >= THRESHOLD)
			{
				// 落下などで上から衝突したとき（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// 内積の結果がマイナスであれば、下面と衝突した
			else if (dot < 0.0f)
			{
				// ジャンプなどで下から衝突したとき（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
	}
}

// 描画
void CPlayerBase::Render()
{
	CXCharacter::Render();
	mpTestModel->Render(Matrix());
}

// 死亡によってゲームが終了するかを取得する
bool CPlayerBase::GetGameEnd()
{
	return mIsGameEnd;
}

// 死亡によってゲームが終了するかを設定する
void CPlayerBase::SetGameEnd(bool isEnd)
{
	mIsGameEnd = isEnd;
}
