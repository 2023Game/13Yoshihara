#pragma once
#include "CRoomBase.h"

class CBox;
class CMoveObj;
class CSwitch;
class CSwitchMoveWall;
class CWater;
class CRespawnArea;

// ����5
class CRoom5 : public CRoomBase
{
public:
	// �R���X�g���N�^
	CRoom5(const CVector& pos);
	// �f�X�g���N�^
	~CRoom5();

	// �����̗L��������ݒ�
	void SetEnableRoom(bool enable) override;

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;

	// �I�u�W�F�N�g
	CBox* mpBox1;
	CBox* mpBox2;
	CMoveObj* mpMoveFloor;
	CSwitch* mpSwitch;
	CSwitchMoveWall* mpMoveWall;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};