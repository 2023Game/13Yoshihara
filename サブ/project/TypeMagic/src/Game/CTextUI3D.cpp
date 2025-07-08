#include "CTextUI3D.h"
#include "Maths.h"

// �e�L�X�g�̍ŏ��t�H���g�T�C�Y
#define TEXT_FONT_SIZE_MIN 8
// �e�L�X�g�̍ő�t�H���g�T�C�Y
#define TEXT_FONT_SIZE_MAX 32

// �e�L�X�g�T�C�Y���ŏ��ɂȂ鋗��
#define TEXT_FONT_SIZE_MIN_DIST 500.0f
// �e�L�X�g�T�C�Y���ő�ɂȂ鋗��
#define TEXT_FONT_SIZE_MAX_DIST 50.0f

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
	CCamera* camera = CCamera::CurrentCamera();
	// ����
	float dist = CVector::Distance(mWorldPos, camera->Position());
	// �����ɂ���ăT�C�Y�����߂�
	float size = Math::Lerp(TEXT_FONT_SIZE_MAX, TEXT_FONT_SIZE_MIN, (dist - TEXT_FONT_SIZE_MAX_DIST) / (TEXT_FONT_SIZE_MIN_DIST - TEXT_FONT_SIZE_MAX_DIST));
	// �t�H���g�T�C�Y��ݒ�
	SetFontSize(size);

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

// ���[���h���W��ݒ�
void CTextUI3D::SetWorldPos(const CVector& pos)
{
	mWorldPos = pos;
}
