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

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �t�B�[���h�I�u�W�F�N�g���폜
	void DeleteFieldObjects() override;

	// �I�u�W�F�N�g
	CBox* mpBox;
	CMoveObj* mpMoveFloor;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};