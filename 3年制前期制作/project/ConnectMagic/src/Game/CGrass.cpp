#include "CGrass.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"
#include "CFlamethrower.h"
#include "CConnectPointManager.h"

// �摜�̃T�C�Y
#define SIZE 2.0f

// �A���t�@�̌������x
#define ALPHA_DECREASE_SPEED 0.5f

// �R���X�g���N�^
CGrass::CGrass(CVector fireOffsetPos, float fireScale)
	: CFire("", fireOffsetPos, fireScale)
{
	SetConnectObjTag(EConnectObjTag::eGrass);

	// ���̉摜�𐶐�
	mpGrassImage = new CBillBoardImage
	(
		"Field/Grass/Grass.png", ETag::eConnectObject,
		ETaskPauseType::eGame
	);
	// �e�ɐݒ�
	mpGrassImage->SetParent(this);
	// �T�C�Y���擾
	CVector2 size = mpGrassImage->GetSize();
	size *= SIZE;
	// �T�C�Y��ݒ�
	mpGrassImage->SetSize(size);
	// �T�C�Y�̔������ɏグ��
	mpGrassImage->SetOffsetPos(CVector(0.0f, size.Y() * 0.8f, 0.0f));
}

// �f�X�g���N�^
CGrass::~CGrass()
{
}

// �X�V
void CGrass::Update()
{
	// ���N���X�̍X�V����
	CConnectObject::Update();

	// �������Ă���Ȃ�
	if (mIsFire)
	{
		// �A���t�@�����Z���Ă���
		float alpha = mpGrassImage->GetAlpha();
		alpha -= ALPHA_DECREASE_SPEED * Times::DeltaTime();
		// �A���t�@��0�ȉ��Ȃ�
		if (alpha <= 0.0f)
		{
			// �S�č폜
			Delete();
			return;
		}
		// �A���t�@��ݒ�
		mpGrassImage->SetAlpha(alpha);
	}
}

// �`��
void CGrass::Render()
{
	mpGrassImage->Render();
}

// �S�č폜
void CGrass::Delete()
{
	// ���������폜
	mpFlamethrower->Kill();
	mpFlamethrower = nullptr;
	// ���̉摜���폜
	mpGrassImage->Kill();
	mpGrassImage = nullptr;
	// �폜
	Kill();
}
