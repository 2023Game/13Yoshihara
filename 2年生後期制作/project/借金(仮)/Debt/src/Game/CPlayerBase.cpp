//プレイヤークラスのインクルード
#include "CPlayerBase.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"

// プレイヤーのインスタンス
CPlayerBase* CPlayerBase::spInstance = nullptr;



#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define CAPSULE_RADIUS_TRASH 2.5f

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

// コンストラクタ
CPlayerBase::CPlayerBase(float capsuleRadius, float playerHeight)
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsWall(false)
	, mpRideObject(nullptr)
	, mMotionBlurRemainTime(0.0f)
	, mIsDamage(false)
{
	spInstance = this;

	//フィールドとだけ衝突判定をする
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, capsuleRadius, 0.0f),
		CVector(0.0f, playerHeight - capsuleRadius, 0.0f),
		capsuleRadius
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField });
}

CPlayerBase::~CPlayerBase()
{
}

CPlayerBase* CPlayerBase::Instance()
{
	return spInstance;
}

//// 攻撃
//void CPlayerBase::UpdateAttack()
//{
//	// 攻撃アニメーションを開始
//	ChangeAnimation(EAnimType::eAttack);
//	// 攻撃終了待ち状態へ移行
//	mState = EState::eAttackWait;
//
//	// 斬撃SEの再生済みフラグを初期化
//	mIsPlayedSlashSE = false;
//	// 斬撃エフェクトの生成済みフラグを初期化
//	mIsSpawnedSlashEffect = false;
//}
//
//// 攻撃終了待ち
//void CPlayerBase::UpdateAttackWait()
//{
//	// 斬撃SEを再生していないかつ、アニメーションが25%以上進行したら、
//	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
//	{
//		// 斬撃SEを再生
//		mpSlashSE->Play();
//		mIsPlayedSlashSE = true;
//	}
//
//	// 斬撃エフェクトを生成していないかつ、アニメーションが35%以上進行したら、
//	if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.35f)
//	{
//		// 斬撃エフェクトを生成して、正面方向へ飛ばす
//		CSlash* slash = new CSlash
//		(
//			this,
//			Position() + CVector(0.0f, 10.0f, 0.0f),
//			VectorZ(),
//			300.0f,
//			100.0f
//		);
//		// 斬撃エフェクトの色設定
//		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));
//
//		mIsSpawnedSlashEffect = true;
//	}
//
//	// 攻撃アニメーションが終了したら、
//	if (IsAnimationFinished())
//	{
//		// 待機状態へ移行
//		mState = EState::eIdle;
//		ChangeAnimation(EAnimType::eIdle);
//	}
//}


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

// 更新
void CPlayerBase::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// 攻撃を受けていない時は
	if (!mIsDamage)
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

	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("Wall:%s\n", mIsWall ? "true" : "false");

	mIsGrounded = false;
	mIsWall = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// 衝突処理
void CPlayerBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderCapsule)
	{
		// 衝突した相手がフィールドオブジェクトの場合
		if (other->Layer() == ELayer::eField)
		{
			// 許容範囲
			const float epsilon = 0.001f;

			CVector adjust = hit.adjust;

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);

			// 内積の結果がepsilon以上なら、床と衝突した
			if (dot >= epsilon)
			{
				// 落下などで床に上から衝突した時（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = normal;

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}

				// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
				adjust.X(0.0f);
				adjust.Z(0.0f);
			}
			// 内積の結果が-epsilonより小さければ、天井と衝突した
			else if (dot < -epsilon)
			{
				// ジャンプなどで天井に下から衝突した時（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
			// TODO:壁に移動し続けているとき震えるので後で修正
			// 床でも天井でもないなら壁に衝突した
			else
			{
				CVector horizontalNormal = normal;
				horizontalNormal.Y(0.0f); // 水平面の法線

					// X方向の＋へ押し戻す壁なら－側にいるので
				if (horizontalNormal.X() > 0.0f)
				{
					// X移動が－なら0にする
					if (mMoveSpeed.X() < 0.0f)
					{
						mMoveSpeed.X(0.0f);
					}
				}
				// X方向の－へ押し戻す壁なら＋側にいるので
				else if (horizontalNormal.X() < 0.0f)
				{
					// X移動が＋なら0にする
					if (mMoveSpeed.X() > 0.0f)
					{
						mMoveSpeed.X(0.0f);
					}
				}

				// Z方向の＋へ押し戻す壁なら－側にいるので
				if (horizontalNormal.Z() > 0.0f)
				{
					// Z移動が－なら0にする
					if (mMoveSpeed.Z() < 0.0f)
					{
						mMoveSpeed.Z(0.0f);
					}
				}
				// Z方向の－へ押し戻す壁なら＋側にいるので
				else if (horizontalNormal.Z() < 0.0f)
				{
					// Z移動が＋なら0にする
					if (mMoveSpeed.Z() > 0.0f)
					{
						mMoveSpeed.Z(0.0f);
					}
				}

				// 壁に接触した
				mIsWall = true;
				// 接触した壁の法線を記憶
				mWallNormal = horizontalNormal;

				// 縦方向の調整値を0にする
				adjust.Y(0.0f);
			}

			// 位置の調整
			Position(Position() + adjust * hit.weight);
		}
	}
}

// 描画
void CPlayerBase::Render()
{
	CXCharacter::Render();
}
