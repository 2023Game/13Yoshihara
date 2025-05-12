#pragma once
#include "CFieldBase.h"

class CSwitch;
class CDoor;
class CBox;
class CAirConnectObj;

// �e�X�g�p�̃t�B�[���h
class CTestField : public CFieldBase
{
public:
	// �R���X�g���N�^
	CTestField();
	// �f�X�g���N�^
	~CTestField();
private:
	// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �t�B�[���h�̃R���C�_�\�𐶐�
	void CreateCol() override;

	CSwitch* mpSwitch;	// �X�C�b�`
	CDoor* mpDoor;		// �h�A
	CBox* mpBox;		// ��
	// �󒆂̐ڑ��I�u�W�F�N�g
	std::list<CAirConnectObj*> mAirConnectObjs;
};