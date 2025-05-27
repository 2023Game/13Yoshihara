#pragma once
#include "CFire.h"
#include "CBillBoardImage.h"

class CBillboardImage;

// �΂ƂȂ���ƔR���鑐�I�u�W�F�N�g
class CGrass : public CFire
{
public:
	// �R���X�g���N�^
	CGrass(CVector fireOffsetPos = CVector::zero, float fireScale = FIRE_SCALE * 2.0f);
	// �f�X�g���N�^
	~CGrass();

	// �X�V
	void Update() override;

private:
	// ���̉摜
	CBillBoardImage* mpGrassImage;
};