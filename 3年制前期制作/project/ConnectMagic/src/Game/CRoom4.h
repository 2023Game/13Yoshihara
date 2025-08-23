#pragma once
#include "CRoomBase.h"

class CBox;
class CMoveObj;
class CWater;
class CRespawnArea;

// ����4
class CRoom4 : public CRoomBase
{
public:
	// �R���X�g���N�^
	CRoom4(const CVector& pos);
	// �f�X�g���N�^
	~CRoom4();

	// �����̗L��������ݒ�
	void SetEnableRoom(bool enable) override;

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;

	// �I�u�W�F�N�g
	CBox* mpBox;
	CMoveObj* mpMoveFloor;
	CMoveObj* mpMoveWall;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};