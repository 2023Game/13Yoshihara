#pragma once
#include "CMapBase.h"

class CMap_3 : public CMapBase
{
public:
	// �R���X�g���N�^
	CMap_3(bool isUp, bool isDown, bool isLeft, bool isRight);
	// �f�X�g���N�^
	~CMap_3();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �o�H�T���p�̃m�[�h�𐶐�
	void CreateNavNodes() override;
};