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

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �t�B�[���h�I�u�W�F�N�g���폜
	void DeleteFieldObjects() override;

	// �I�u�W�F�N�g
	CBox* mpBox1;
	CBox* mpBox2;
	CMoveObj* mpMoveFloor;
	CSwitch* mpSwitch;
	CSwitchMoveWall* mpMoveWall;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};