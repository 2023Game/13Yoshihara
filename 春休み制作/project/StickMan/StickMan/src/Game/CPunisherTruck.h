#pragma once
#include "CGarbageTruck.h"

class CPunisherTruck : public CGarbageTruck
{
public:
	// �R���X�g���N�^
	CPunisherTruck(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints);
	// �f�X�g���N�^
	~CPunisherTruck();

	// �ϐ������Z�b�g
	void Reset() override;

private:
	// �ړ�����
	void UpdateMove() override;
	// �������
	void UpdateCollect() override;
};