#include "CTextUI3D.h"
#include "Maths.h"

// �R���X�g���N�^
CTextUI3D::CTextUI3D(CVector pos, ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CTextUI2D(pauseType, addTaskList, fontPath)
	, mWorldPos(pos)
{
}

// �f�X�g���N�^
CTextUI3D::~CTextUI3D()
{
}

// �X�V
void CTextUI3D::Update()
{
	// ���W���X�V
	Position(mWorldPos);

	CTextUI2D::Update();
}

// ���[���h���W����X�N���[�����W�����߂č��W��ݒ�
void CTextUI3D::Position(const CVector& pos)
{
	// �J�����擾
	CCamera* camera = CCamera::CurrentCamera();
	// �X�N���[�����W�ɕϊ�
	CVector screenPos = camera->WorldToScreenPos(pos);
	// ���W��ݒ�
	CTransform::Position(screenPos);
}