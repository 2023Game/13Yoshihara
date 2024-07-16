#include "CXEnemy.h"

//�R���C�_�̏�����
CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColSphereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
	, mColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
{
	mTag = ETag::EENEMY;
}

//�����s��̐ݒ�
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.Matrix(&mpCombinedMatrix[1]);
	//��
	mColSphereBody.Matrix(&mpCombinedMatrix[1]);
	//��
	mColSphereSword0.Matrix(&mpCombinedMatrix[26]);
	mColSphereSword1.Matrix(&mpCombinedMatrix[26]);
	mColSphereSword2.Matrix(&mpCombinedMatrix[26]);
}

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	switch (m->Type())
	{
	case CCollider::EType::ESPHERE://���������R���C�_
		if (o->Type() == CCollider::EType::ESPHERE &&//���肪���R���C�_
			o->Tag() == CCollider::ETag::ESWORD &&//����̃^�O����
			o->Parent()->Tag() == CCharacter3::ETag::EPLAYER &&//����̐e���v���C���[
			m->Tag() == CCollider::ETag::EBODY)//�����̃^�O����
		{
			//�Փ˔���
			if (m->Collision(m, o) == true)
			{
				//30�t���[�������ă_�E�����A�J��Ԃ��Ȃ�
				ChangeAnimation(11, false, 30);
			}
		}
	}
}
