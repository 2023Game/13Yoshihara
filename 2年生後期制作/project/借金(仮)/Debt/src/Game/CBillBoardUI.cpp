#include "CBillBoardUI.h"

CBillBoardUI::CBillBoardUI(std::string path, ETag tag, ETaskPauseType pause)
	:CBillBoardImage(path, tag, pause)
{
}

CBillBoardUI::~CBillBoardUI()
{
}

void CBillBoardUI::Render(CMaterial* mpMaterial)
{
	//行列の保存
	glPushMatrix();

	//常にカメラの方向を向かせる
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	//デプス値の書き込みをオフ
	glDepthMask(false);
	// 震度テストを無効
	glDisable(GL_DEPTH_TEST);
	//ライトオフ
	glDisable(GL_LIGHTING);
	//マテリアル適用
	mpMaterial->Enabled(mColor);
	//三角形の描画
	mT[0].Render();
	mT[1].Render();
	//マテリアル解除
	mpMaterial->Disabled();
	//ライトオン
	glEnable(GL_LIGHTING);
	// 震度テストを有効
	glDisable(GL_DEPTH_TEST);
	//デプス値の書き込みをオンに戻す
	glDepthMask(true);

	//行列を戻す
	glPopMatrix();
}
