#include "CXEnemy.h"

//�R���C�_�̏�����
CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	mTag = ETag::EENEMY;
}

//�����s��̐ݒ�
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//��
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
	//��
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
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
