#pragma once
#include "CRoomBase.h"

class CPortal;

// �Ō�̕���
class CLastRoom : public CRoomBase
{
public:
	// �R���X�g���N�^
	CLastRoom(const CVector& pos);
	// �f�X�g���N�^
	~CLastRoom();

private:
	// �I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	
	// �|�[�^��
	CPortal* mpPortal;
};