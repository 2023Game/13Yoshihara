#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter
{
public:
	//�R���C�_�̏�����
	CXEnemy();
	//�����s��̐ݒ�
	void Init(CModelX* model);
private:
	//�R���C�_�̐錾
	CCollider mColSphereHead;//��
	CCollider mColSphereBody;//��
	CCollider mColSphereSword;//��
};
#endif