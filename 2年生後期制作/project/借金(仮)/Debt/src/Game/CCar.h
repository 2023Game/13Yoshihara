#pragma once
#include "CVehicleBase.h"

class CModel;

// �Ԃ̃N���X
class CCar : public CVehicleBase
{
public:
	// �R���X�g���N�^
	CCar(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road);
	// �f�X�g���N�^
	~CCar();

	// �X�V
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

private:
};