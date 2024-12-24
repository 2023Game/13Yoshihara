#include "CInteractUI3D.h"
#include "CImage3D.h"

#define WORLD_UNIT_PER_PIXEL 128.0f

CInteractUI3D::CInteractUI3D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
{
	// UI�̃C���[�W��ǂݍ���
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
	// �ǂݍ��񂾃C���[�W���폜
	SAFE_DELETE(mpInteractUIImg);

	// ������ɍ폜���ꂽ���Ƃ�`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// �������ݒ�
void CInteractUI3D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �X�V
void CInteractUI3D::Update()
{
	mpInteractUIImg->Update();
}

// TODO�F�������\������悤�ɕύX
// �`��
void CInteractUI3D::Render()
{
	//�s��̕ۑ�
	glPushMatrix();

	//��ɃJ�����̕�������������
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	// �C���^���N�gUI��`��
	mpInteractUIImg->Render();

	//�s���߂�
	glPopMatrix();
}
