#pragma once
#include "CConnectObject.h"
#include "CBillBoardImage.h"

class CBillboardImage;

// �΂ƂȂ���ƔR���鑐�I�u�W�F�N�g
class CGrass : public CConnectObject
{
public:
	// �R���X�g���N�^
	CGrass();
	// �f�X�g���N�^
	~CGrass();

	// �q�������Ƃ��̏���
	void Connect(CConnectObject* other);

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ���̉摜
	CBillBoardImage* mpGrassImage;
};