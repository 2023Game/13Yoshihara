#include "CInteractUI3D.h"
#include "CImage3D.h"

#define WORLD_UNIT_PER_PIXEL 128.0f

CInteractUI3D::CInteractUI3D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
{
	// UIのイメージを読み込み
	mpInteractUIImg = new CImage3D
	(
		"UI\\interact.png",
		ETag::eUI,
		ETaskPriority::eUI3D,0,
		ETaskPauseType::eGame,
		false,false
	);
	mpInteractUIImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpInteractUIImg->SetDepthTest(false);
}

CInteractUI3D::~CInteractUI3D()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpInteractUIImg);

	// 持ち主に削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 持ち主を設定
void CInteractUI3D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 更新
void CInteractUI3D::Update()
{
	mpInteractUIImg->Update();
}

// TODO：文字列を表示するように変更
// 描画
void CInteractUI3D::Render()
{
	//行列の保存
	glPushMatrix();

	//常にカメラの方向を向かせる
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	// インタラクトUIを描画
	mpInteractUIImg->Render();

	//行列を戻す
	glPopMatrix();
}
