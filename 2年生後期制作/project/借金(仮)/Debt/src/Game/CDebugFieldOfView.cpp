#include "CDebugFieldOfView.h"
#include "Primitive.h"
#include "glut.h"

// コンストラクタ
CDebugFieldOfView::CDebugFieldOfView(CObjectBase* owner,
	float fovAngle, float fovLength)
	: CObjectBase(ETag::eDebug, ETaskPriority::eDebugFov, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mFovAngle(fovAngle)
	, mFovLength(fovLength)
{
}

// デストラクタ
CDebugFieldOfView::~CDebugFieldOfView()
{
	// 表示対象が存在する場合
	// 自身が削除されたことを表示対象に伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 視野範囲を表示する対象を設定
void CDebugFieldOfView::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 視野範囲を設定
void CDebugFieldOfView::Set(float angle, float length)
{
	mFovAngle = angle;
	mFovLength = length;
}

// 更新
void CDebugFieldOfView::Update()
{
	// 表示対象のスケール以外を適用
	Position(mpOwner->Position());
	Rotation(mpOwner->Rotation());
}

// 描画
void CDebugFieldOfView::Render()
{
	// 表示対象がいない場合は、描画処理を行わない
	if (mpOwner == nullptr) return;

	// デブステストをオフ
	glDisable(GL_DEPTH_TEST);

	// 強制的に半透明
	mColor.A(0.5f);

	// 視野範囲の扇形を描画
	Primitive::DrawSector
	(
		// 表示対象のスケールを無視した行列
		Matrix(),
		// -視野角度 ～ +視野角度の範囲を設定
		-mFovAngle, mFovAngle,				
		// 視野距離
		mFovLength,
		// 扇形の色
		mColor								
	);

	// デプステストを元に戻す
	glEnable(GL_DEPTH_TEST);
}
