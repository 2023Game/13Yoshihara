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
	// �\���Ώۂ̃X�P�[���ȊO��K�p
	Position(mpOwner->Position());
	Rotation(mpOwner->Rotation());
}

// �`��
void CDebugFieldOfView::Render()
{
	// �\���Ώۂ����Ȃ��ꍇ�́A�`�揈�����s��Ȃ�
	if (mpOwner == nullptr) return;

	// �f�u�X�e�X�g���I�t
	glDisable(GL_DEPTH_TEST);

	// �����I�ɔ�����
	mColor.A(0.5f);

	// ����͈͂̐�`��`��
	Primitive::DrawSector
	(
		// �\���Ώۂ̃X�P�[���𖳎������s��
		Matrix(),
		// -����p�x �` +����p�x�͈̔͂�ݒ�
		-mFovAngle, mFovAngle,				
		// ���싗��
		mFovLength,
		// ��`�̐F
		mColor								
	);

	// �f�v�X�e�X�g�����ɖ߂�
	glEnable(GL_DEPTH_TEST);
}
