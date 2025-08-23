#pragma once
#include "CRoomBase.h"

class CBox;
class CMoveObj;
class CWater;
class CRespawnArea;

// ����2
class CRoom2 : public CRoomBase
{
public:
	// �R���X�g���N�^
	CRoom2(const CVector& pos);
	// �f�X�g���N�^
	~CRoom2();

	// �����̗L��������ݒ�
	void SetEnableRoom(bool enable) override;

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;

	// �I�u�W�F�N�g
	CBox* mpBox;
	CMoveObj* mpMoveFloor;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};