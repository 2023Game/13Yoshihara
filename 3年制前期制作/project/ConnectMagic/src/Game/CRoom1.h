#pragma once
#include "CRoomBase.h"

class CBox;
class CSwitch;
class CSwitchMoveFloor;
class CWater;
class CRespawnArea;

// ����1
class CRoom1 : public CRoomBase
{
public:
	// �R���X�g���N�^
	CRoom1(const CVector& pos);
	// �f�X�g���N�^
	~CRoom1();

	// �����̗L��������ݒ�
	void SetEnableRoom(bool enable) override;

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;

	// �I�u�W�F�N�g
	CBox* mpBox;
	CSwitch* mpSwitch;
	CSwitchMoveFloor* mpMoveFloor;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};