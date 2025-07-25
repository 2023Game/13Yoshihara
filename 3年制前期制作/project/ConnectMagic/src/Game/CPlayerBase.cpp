//プレイヤークラスのインクルード
#include "CPlayerBase.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "Maths.h"
#include "CInteractObject.h"
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
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsGravity(true)
	, mIsMoveDir(true)
	, mpRideObject(nullptr)
	, mMotionBlurRemainTime(0.0f)
	, mpBodyCol(nullptr)
	, mpAttackCol(nullptr)
	, mpSearchCol(nullptr)
	, mpHpGauge(nullptr)
	, mIsGameEnd(false)
{
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
	SAFE_DELETE(mpSearchCol);

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

// モーションブラーの更新処理
void CPlayerBase::UpdateMotionBlur()
{
	// モーションブラーの残り時間が残っていなければ、処理しない
	if (mMotionBlurRemainTime <= 0.0f) return;
	// 現在のカメラを取得し、存在しなければ処理しない
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// カメラの向きと反対方向へブラーを掛けるため、
	// 反転したカメラの向きを取得
	CVector camDir = -currentCamera->VectorZ();

	// 残り時間から経過時間の割合を取得（経過時間の割合 = 1 - 残り時間の割合）
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// ブラーの幅をサインカーブで経過時間に合わせて増減させる
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// モーションブラーのパラメータを設定
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// 残り時間を経過時間分減少させる
	mMotionBlurRemainTime -= Times::DeltaTime();
	// 残り時間がなくなれば、
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// モーションブラーをオフにする
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
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
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 重力を掛けるなら
	if (mIsGravity)
	{
		mMoveSpeedY -= GRAVITY;
	}
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// 攻撃を受けていない時かつ
	// 移動方向を向く設定がオンの時
	if (!mIsDamage&&
		mIsMoveDir)
	{
		// プレイヤーを移動方向へ向ける
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}


	// 「P」キーを押したら、ゲームを終了
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// 「B」キーを押したら、モーションブラー開始
	if (CInput::PushKey('B'))
	{
		// モーションブラーを掛けている最中であれば、
		// 新しくモーションブラーを掛け直さない
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
	}

	// モーションブラーの更新処理
	UpdateMotionBlur();

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

	// 調べるオブジェクトのリストをクリア
	mNearInteractObjs.clear();
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
		// 衝突した相手が地面の場合
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
			adjust.Y(0.0f);

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
	// 調べるオブジェクトの探知コライダーの衝突判定
	else if (self == mpSearchCol)
	{
		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr)
		{
			// 衝突した調べるオブジェクトをリストに追加
			mNearInteractObjs.push_back(obj);

#if _DEBUG
			// 探知範囲内に入ったオブジェクトの名前を表示
			CDebugPrint::Print
			(
				"%s:%s\n",
				obj->GetDebugName().c_str(),
				obj->GetInteractStr().c_str()
			);
#endif
		}
	}
}

// 描画
void CPlayerBase::Render()
{
	CXCharacter::Render();
}

// 一番近くにある調べるオブジェクトを取得
CInteractObject* CPlayerBase::GetNearInteractObject() const
{
	// 一番近くの調べるオブジェクトのポインタ格納用
	CInteractObject* nearObj = nullptr;
	float nearDist = 0.0f;	// 現在一番近くにある調べるオブジェクトとの距離
	CVector pos = Position();
	// 探知範囲内の調べるオブジェクトを順番に調べる
	for (CInteractObject * obj : mNearInteractObjs)
	{
		// 現在調べられる状態じゃなければスルー
		if (!obj->CanInteract()) continue;

		float dist = (obj->Position() - pos).LengthSqr();
		// 一番最初の調べるオブジェクトか
		// 求めた距離が現在の一番近いオブジェクトよりも近い場合
		if (nearObj == nullptr || dist < nearDist)
		{
			// 一番近いオブジェクトを更新
			nearObj = obj;
			nearDist = dist;
		}
	}
	return nearObj;
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
