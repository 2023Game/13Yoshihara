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
	//�s��̕ۑ�
	glPushMatrix();

	//��ɃJ�����̕�������������
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	//�f�v�X�l�̏������݂��I�t
	glDepthMask(false);
	// �k�x�e�X�g�𖳌�
	glDisable(GL_DEPTH_TEST);
	//���C�g�I�t
	glDisable(GL_LIGHTING);
	//�}�e���A���K�p
	mpMaterial->Enabled(mColor);
	//�O�p�`�̕`��
	mT[0].Render();
	mT[1].Render();
	//�}�e���A������
	mpMaterial->Disabled();
	//���C�g�I��
	glEnable(GL_LIGHTING);
	// �k�x�e�X�g��L��
	glDisable(GL_DEPTH_TEST);
	//�f�v�X�l�̏������݂��I���ɖ߂�
	glDepthMask(true);

	//�s���߂�
	glPopMatrix();
}
