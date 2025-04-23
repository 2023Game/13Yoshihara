#include "CWand.h"
#include "CModel.h"
#include "CConnectPoint.h"

// ���C��L�΂��鋗��
#define RAY_DISTANCE 10.0f

// �ڑ����̃I�t�Z�b�g���W
#define POINT_OFFSET_POSX 0.0f
#define POINT_OFFSET_POSY 10.0f
#define POINT_OFFSET_POSZ 5.0f
// �ڑ����̃X�P�[��
#define POINT_SCALE 0.5f


// �R���X�g���N�^
CWand::CWand(CObjectBase* owner, ETag tag)
	: CWeapon(tag)
	, mpModel(nullptr)
{
	mpOwner = owner;

	// ���f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Wand");

	// �ڑ������̃r���{�[�h
	mpPoint = new CConnectPoint();
	// �ŏ��͔�\��
	mpPoint->SetEnable(false);
	mpPoint->SetShow(false);
	// �X�P�[������
	mpPoint->Scale(mpPoint->Scale() * POINT_SCALE);
}

// �f�X�g���N�^
CWand::~CWand()
{
}

// �X�V
void CWand::Update()
{
	CVector offsetPos = mpOwner->Position();
	offsetPos += mpOwner->VectorX() * POINT_OFFSET_POSX;
	offsetPos += mpOwner->VectorY() * POINT_OFFSET_POSY;
	offsetPos += mpOwner->VectorZ() * POINT_OFFSET_POSZ;
	mpPoint->Position(offsetPos);
}

// �`��
void CWand::Render()
{
	mpModel->Render(Matrix());
}

// �Փ˔�����s���R���C�_�[�����X�g�ɒǉ�
void CWand::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// �Փ˔�����s���R���C�_�[�����X�g�����菜��
void CWand::RemoveCollider(CCollider* col)
{
	mColliders.remove(col);
}

// �ݒ肳��Ă���R���C�_�[�ƏՓ˔�����s��
bool CWand::Ray(CVector& hitPos)
{
	CHitInfo hit;
	// �J�������擾
	CCamera* camera = CCamera::CurrentCamera();
	// ���_���璍���_�ւ̕���
	CVector dir = camera->GetEyeVec();
	dir.Normalize();
	// ���C�̊J�n�ƏI���n�_
	CVector rayStart = camera->GetEye();
	CVector rayEnd = dir * RAY_DISTANCE;
	float nearDist = 0.0f;
	bool isHit = false;
	// �ݒ肳��Ă���R���C�_�[�����Ԃɒ��ׂ�
	for (CCollider* c : mColliders)
	{
		// ���C�ƃR���C�_�[�̏Փ˔�����s��
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// ��_���s���l�łȂ���΁A
			if (hit.cross.LengthSqr() != 0.0f)
			{
				// �Փˈʒu�܂ł̋����ň�ԋ߂����������߂�
				if (!isHit)
				{
					nearDist = hit.dist;
					hitPos = hit.cross;
				}
				else if (nearDist > hit.dist)
				{
					nearDist = hit.dist;
					hitPos = hit.cross;
				}
				isHit = true;
			}
		}
	}

	return isHit;
}

// �ڑ����̗L��������ݒ�
void CWand::SetConnect(bool isOnOff)
{
	mpPoint->SetEnable(isOnOff);
	mpPoint->SetShow(isOnOff);
}

// �ڑ�����Ă��邩
bool CWand::GetConnect()
{
	return mpPoint->IsShow();
}
