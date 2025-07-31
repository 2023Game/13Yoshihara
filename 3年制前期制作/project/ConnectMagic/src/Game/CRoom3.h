#pragma once
#include "CRoomBase.h"

class CBox;
class CSwitch;
class CSwitchMoveFloor;
class CMoveObj;
class CWater;
class CRespawnArea;

// ����3
class CRoom3 : public CRoomBase
{
public:
	// �R���X�g���N�^
	CRoom3(const CVector& pos);
	// �f�X�g���N�^
	~CRoom3();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �t�B�[���h�I�u�W�F�N�g���폜
	void DeleteFieldObjects() override;

	// �I�u�W�F�N�g
	CBox* mpBox1;
	CBox* mpBox2;
	CSwitch* mpSwitch1;
	CSwitch* mpSwitch2;
	CSwitchMoveFloor* mpSwitchMoveFloor1;
	CSwitchMoveFloor* mpSwitchMoveFloor2;
	CMoveObj* mpMoveFloor1;
	CMoveObj* mpMoveFloor2;
	CMoveObj* mpMoveFloor3;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};