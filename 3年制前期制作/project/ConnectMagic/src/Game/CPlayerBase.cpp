//プレイヤークラスのインクルード
#include "CPlayerBase.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CSceneManager.h"
#include "CGaugeUI2D.h"
#include "CollisionLayer.h"

// プレイヤーのインスタンス
CPlayerBase* CPlayerBase::spInstance = nullptr;

constexpr float THRESHOLD =			0.1f;

// モーションブラーを掛ける時間
constexpr float MOTION_BLUR_TIME =	3.0f;
// モーションブラーの幅
constexpr float MOTION_BLUR_WIDTH = 1.0f;
// モーションブラーの反復回数
constexpr float MOTION_BLUR_COUNT = 5;

// コンストラクタ
CPlayerBase::CPlayerBase()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mIsGrounded(false)
	, mIsGravity(true)
	, mIsMoveDir(true)
	, mpRideObject(nullptr)
	, mMotionBlurRemainTime(0.0f)
	, mpHpGauge(nullptr)
	, mIsGameEnd(false)
{
	spInstance = this;
}

// デストラクタ
CPlayerBase::~CPlayerBase()
{
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


#if _DEBUG
	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("Pos:%f,%f,%f\n", Position().X(), Position().Y(), Position().Z());
	CDebugPrint::ForcePrint("FPS:%f\n", Times::FPS());
#endif

	mIsGrounded = false;
}

// 描画
void CPlayerBase::Render()
{
	CXCharacter::Render();
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

// アニメーションを時間を指定して設定
void CPlayerBase::ChangeAnimationTime(int type, float time)
{
	// アニメーションの設定
	ChangeAnimation(type);
	// 時間の設定
	SetAnimationFrame(time);
}

void CPlayerBase::SetGrounded(bool isGrounded)
{
	mIsGrounded = isGrounded;
}

bool CPlayerBase::GetGrounded() const
{
	return mIsGrounded;
}
