#include "CGameCamera2.h"
#include "CInput.h"
#include "Maths.h"

// カメラの回転速度
#define ROTATE_SPEED 1.0f
// カメラの上下回転の範囲
#define ROTATE_RANGE_X 45.0f
// カメラのオフセット座標Z
#define EYE_OFFSET_POS_Z 100.0f

// コンストラクタ
CGameCamera2::CGameCamera2(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
	, mpLockOnTarget(nullptr)
{
}

// デストラクタ
CGameCamera2::~CGameCamera2()
{
}

void CGameCamera2::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowDefaultEyeVec = mTargetEye - mAt;
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

void CGameCamera2::LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye)
{
	CCamera::LookAt(eye, at, up, updateTargetEye);
	if (mFollowTargetTf != nullptr)
	{
		if (updateTargetEye)
		{
			mFollowDefaultEyeVec = mTargetEye - mAt;
		}
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

// 更新
void CGameCamera2::Update()
{
	// 追従するターゲットとロックオンターゲットが設定されていれば
	if (mFollowTargetTf != nullptr &&
		mpLockOnTarget != nullptr)
	{
		// ロックオンターゲットから追従ターゲットへの方向
		CVector dir = mFollowTargetTf->Position() - mpLockOnTarget->Position();
		dir.Normalize();

		// 注視点は追従ターゲット
		mAt = mFollowTargetTf->Position()+ mFollowOffsetPos;
		// 視点は注視点をdir方向にずらした座標
		CVector eye = mAt + dir * EYE_OFFSET_POS_Z;
		// 補間した値を設定
		mTargetEye = CVector::Lerp(mEye, eye, 0.1f);
		mEye = mTargetEye;
	}
	// 追従ターゲットだけなら
	else if (mFollowTargetTf != nullptr)
	{
		// ロックオンターゲットから追従ターゲットへの方向
		CVector dir = mFollowTargetTf->Position() - CVector::zero;
		dir.Normalize();

		// 注視点は追従ターゲット
		mAt = mFollowTargetTf->Position() + mFollowOffsetPos;
		// 視点は注視点をdir方向にずらした座標
		CVector eye = mAt + dir * EYE_OFFSET_POS_Z;
		// 補間した値を設定
		mTargetEye = CVector::Lerp(mEye, eye, 0.1f);
		mEye = mTargetEye;

		//// 注視点は追従ターゲット
		//mAt = mFollowTargetTf->Position() + mFollowOffsetPos;
		//// 視点は追従ターゲットの後方にずらした座標
		//CVector eye = mAt + (-mFollowTargetTf->VectorZ() * EYE_OFFSET_POS_Z);
		//// 補間した値を設定
		//mTargetEye = CVector::Lerp(mEye, eye, 0.1f);
		//mEye = mTargetEye;
	}

#if _DEBUG
	CDebugPrint::Print("Angle:%f, %f, %f", mRotateAngle.X(), mRotateAngle.Y(), mRotateAngle.Z());
#endif
	// 設定されているコライダーと衝突する場合は、
	// カメラの位置を押し出す
	ApplyCollision();

	// 視点、注視点、上ベクトルから各行列を更新
	LookAt(mEye, mAt, mUp, false);

	// カメラのベースの更新処理
	//CCamera::Update();
}

// 回転角度を設定
void CGameCamera2::SetRotateAngle(CVector angle)
{
	mRotateAngle += angle;
}

// ロックオンターゲットを設定
void CGameCamera2::SetLockOnTarget(CTransform* target)
{
	mpLockOnTarget = target;
}

// ロックオンターゲットを取得
CTransform* CGameCamera2::GetLockOnTarget() const
{
	return mpLockOnTarget;
}

