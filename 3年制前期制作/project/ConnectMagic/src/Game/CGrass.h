#pragma once
#include "CFire.h"
#include "CBillBoardImage.h"
#include "CItemDrop.h"

class CBillboardImage;

// �΂ƂȂ���ƔR���鑐�I�u�W�F�N�g
class CGrass : public CFire, public CItemDrop
{
public:
	// �R���X�g���N�^
	CGrass(CVector fireOffsetPos = CVector::zero, float fireScale = FIRE_SCALE * 2.0f);
	// �f�X�g���N�^
	~CGrass();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �R�����Ƃ��̏���
	void Burning() override;

	// �S�č폜
	void Delete() override;

	// ���̉摜
	CBillBoardImage* mpGrassImage;
};