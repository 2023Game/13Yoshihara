#include "CDebugFieldOfView.h"
#include "Primitive.h"
#include "glut.h"

// �R���X�g���N�^
CDebugFieldOfView::CDebugFieldOfView(CObjectBase* owner,
	float fovAngle, float fovLength)
	: CObjectBase(ETag::eDebug, ETaskPriority::eDebugFov, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mFovAngle(fovAngle)
	, mFovLength(fovLength)
{
}

// �f�X�g���N�^
CDebugFieldOfView::~CDebugFieldOfView()
{
	// �\���Ώۂ����݂���ꍇ
	// ���g���폜���ꂽ���Ƃ�\���Ώۂɓ`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// ����͈͂�\������Ώۂ�ݒ�
void CDebugFieldOfView::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ����͈͂�ݒ�
void CDebugFieldOfView::Set(float angle, float length)
{
	mFovAngle = angle;
	mFovLength = length;
}

// �X�V
void CDebugFieldOfView::Update()
{
}

// �`��
void CDebugFieldOfView::Render()
{
	// �\���Ώۂ����Ȃ��ꍇ�́A�`�揈�����s��Ȃ�
	if (mpOwner == nullptr) return;

	// �f�u�X�e�X�g���I�t
	glDisable(GL_DEPTH_TEST);

	// ����͈͂̐�`��`��
	Primitive::DrawSector
	(
		mpOwner->Matrix(),		// �\���Ώۂ̍s��
		-mFovAngle,mFovAngle,	// -����p�x �` +����p�x�͈̔͂�ݒ�
		mFovLength,				// ���싗��
		mColor					// ��`�̐F
	);

	// �f�v�X�e�X�g�����ɖ߂�
	glEnable(GL_DEPTH_TEST);
}
