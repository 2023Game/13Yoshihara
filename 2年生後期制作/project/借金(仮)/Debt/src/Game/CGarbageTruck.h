#pragma once
#include "CCharaBase.h"
#include "CGarbageTruckStatus.h"
#include "CCar.h"

class CModel;

// �S�~���W�Ԃ̃N���X
// �Ԃ̃N���X���p��
class CGarbageTruck : public CCar
{
public:
	CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation);
	~CGarbageTruck();

	void Update();
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	void Render();


private:
	// ��~����
	void UpdateStop();
	// �������
	void UpdateCollect();

	CGarbageTruckStatus mTruckStatus;	// �S�~���W�Ԃ̃X�e�[�^�X�ǉ���
};