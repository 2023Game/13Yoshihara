#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "Primitive.h"
#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "Maths.h"

// ���C��L�΂��鋗���̍ő�
#define RAY_MAX_DISTANCE 100.0f
// ���C���k�߂�鋗���̍ŏ�
#define RAY_MIN_DISTANCE 15.0f

// �ڑ����̃I�t�Z�b�g���W
#define POINT_OFFSET_POSX 0.0f
#define POINT_OFFSET_POSY 45.0f
#define POINT_OFFSET_POSZ 0.0f

// �ڑ����̃X�P�[��
#define POINT_SCALE 0.5f

// �ڑ��ł���ő吔�̏����l
#define DEFAULT_CONNECT_NUM 1

// �C���X�^���X
CConnectPointManager* CConnectPointManager::spInstance = nullptr;

// �C���X�^���X���擾
CConnectPointManager* CConnectPointManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CConnectPointManager();
	}
	return spInstance;
}

// �R���X�g���N�^
CConnectPointManager::CConnectPointManager()
	: CTask(ETaskPriority::eNone, 0, ETaskPauseType::eGame)
	, mConnectMaxNum(DEFAULT_CONNECT_NUM)
	, mpConnectWandTarget(nullptr)
	, mWandConnectDistance(0.0f)
{
	// ��p�̐ڑ������̃r���{�[�h
	mpPoint = new CConnectPoint(nullptr);
	// �ŏ��͔�\��
	mpPoint->SetEnable(false);
	mpPoint->SetShow(false);
	// �X�P�[������
	mpPoint->Scale(mpPoint->Scale() * POINT_SCALE);
}

// �f�X�g���N�^
CConnectPointManager::~CConnectPointManager()
{
	if (spInstance == this)
		spInstance = nullptr;
}

// �X�V
void CConnectPointManager::Update()
{
	// ��̐�̍��W�����߂�
	WandPos();

	// �ڑ������m���q�������������ɏՓ˂�����폜
	RayPoint();
}

// �`��
void CConnectPointManager::Render()
{
	// ���C�̊J�n�ƏI���n�_
	CVector rayStart;
	CVector rayEnd;
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		// �ڑ������m�Ōq�����Ă��邩
		bool isPoints = false;
		// i�̎��̗v�f�ԍ����v�f�͈͓��Ȃ�
		// �ڑ������m����Ōq����
		if (i + 1 < mPoints.size())
		{
			// �ڑ��������Ă���I�u�W�F�N�g�������Ȃ�
			if (mPoints[i]->GetConnectObj() == mPoints[i + 1]->GetConnectObj())
			{
				// �폜
				DeleteConnectPoint(i);
				DeleteConnectPoint(i);
				// 1�Z�b�g�߂�
				i -= 2;
				// ����
				continue;
			}
			rayStart = mPoints[i]->Position();
			rayEnd = mPoints[i + 1]->Position();
			isPoints = true;
		}
		// �Z�b�g�ɂȂ��Ă��Ȃ��ڑ�������ɂȂ���
		else
		{
			// ��̐ڑ����̍��W
			rayStart = mpPoint->Position();
			// �ڑ����̍��W
			rayEnd = mPoints[i]->Position();

		}

		// �ڑ������m�̋���
		float distance = (rayEnd - rayStart).Length();
		// �ő勗����艓���Ȃ�
		if (distance > RAY_MAX_DISTANCE)
		{
			// �ڑ��폜
			DeleteConnectPoint(i);
			// �ڑ������m�Ȃ�
			if (isPoints)
			{
				// ������폜
				DeleteConnectPoint(i);
				// 1�Z�b�g�߂�
				i -= 2;
			}
			// ��ƂȂ����Ă���Ȃ�
			else
			{
				// ��̐ڑ����𖳌�
				SetWandConnect(false);
			}
			// ����
			continue;
		}

		// ������`��
		Primitive::DrawLine
		(
			rayStart, rayEnd,
			CColor::yellow,
			5.0f
		);
	}
}

// �������鏈�������s
void CConnectPointManager::Pull()
{
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		// i�̎��̗v�f�ԍ����v�f�͈͓��Ȃ�
		// �Z�b�g�ɂȂ��Ă���ڑ���
		if (i + 1 < mPoints.size())
		{
			// �ڑ��������Ă���I�u�W�F�N�g
			CConnectObject* connectObj1 = mPoints[i]->GetConnectObj();
			CConnectObject* connectObj2 = mPoints[i + 1]->GetConnectObj();

			// ������������������߂�
			CVector pullDir = mPoints[i + 1]->Position() - mPoints[i]->Position();
			// �������ŏ������Ȃ玟��
			if (pullDir.Length() < RAY_MIN_DISTANCE) continue;
			// ���K��
			pullDir.Normalize();
			// ��������
			connectObj1->Pull(pullDir, connectObj2->GetWeight());

			// ������������������߂�
			pullDir = mPoints[i]->Position() - mPoints[i + 1]->Position();
			// ���K��
			pullDir.Normalize();
			// ��������
			connectObj2->Pull(pullDir, connectObj1->GetWeight());
		}
		// �Z�b�g�ɂȂ��Ă��Ȃ��ڑ���
		else
		{
			// ������������������߂�
			// �ڑ��������̕���
			CVector pullDir = mpPoint->Position() - mPoints[i]->Position();
			// �������ŏ������Ȃ玟��
			if (pullDir.Length() < RAY_MIN_DISTANCE) continue;
			// ���K��
			pullDir.Normalize();
			// �ڑ��������Ă���I�u�W�F�N�g
			CConnectObject* connectObj = mPoints[i]->GetConnectObj();
			// ��������(�񑤂͓����Ȃ��̂ŏd��1.0f�j
			connectObj->Pull(pullDir, 1.0f);
		}
	}
}

// �ݒ肳��Ă���R���C�_�[�ƏՓ˔�����s��
bool CConnectPointManager::Ray(CVector& hitPos)
{
	CHitInfo hit;
	// �J�������擾
	CCamera* camera = CCamera::CurrentCamera();
	// ���_���璍���_�ւ̕���
	CVector dir = camera->GetEyeVec();
	dir.Normalize();
	// ���C�̊J�n�ƏI���n�_
	CVector rayStart = camera->GetEye();
	CVector rayEnd = rayStart + dir * RAY_MAX_DISTANCE;
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

// ���_����^�[�Q�b�g�܂ł̃��C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
bool CConnectPointManager::RayTarget(CVector targetPos)
{
	CHitInfo hit;
	// �v���C���[���擾
	CPlayerBase* player = CPlayerBase::Instance();
	// �v���C���[�̍��W
	CVector playerPos = player->Position();
	// �J�������擾
	CCamera* camera = CCamera::CurrentCamera();
	// �v���C���[����^�[�Q�b�g�ւ̕���
	CVector dir = targetPos - playerPos;
	dir.Y(0.0f);
	dir.Normalize();
	// ���C�̊J�n�ƏI���n�_
	CVector rayStart = camera->GetEye();
	CVector rayEnd = targetPos;
	// �ݒ肳��Ă���R���C�_�[�����Ԃɒ��ׂ�
	for (CCollider* c : mColliders)
	{
		// ���C�ƃR���C�_�[�̏Փ˔�����s��
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// �v���C���[����Փ˓_�ւ̕���
			CVector playerCrossDir = hit.cross - playerPos;
			playerCrossDir.Y(0.0f);
			playerCrossDir.Normalize();
			// ��̕����̓��ς����߂�
			float dot = playerCrossDir.Dot(dir);
			
			// ���ς����̒l�Ȃ甽�Ε����i�v���C���[�̌��j�ɂ���̂ŏՓ˂𖳎�
			if (dot < 0.0f)
				continue;

			// �Փ˂���
			return true;
		}
	}

	// �Փ˂��Ă��Ȃ�
	return false;
}

// �ڑ������m���q�������C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
// �Փ˂��Ă�����폜����
void CConnectPointManager::RayPoint()
{

	CHitInfo hit;
	// ���C�̊J�n�ƏI���n�_
	CVector rayStart;
	CVector rayEnd;
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		// �ڑ������m�Ōq�����Ă��邩
		bool isPoints = false;
		// i�̎��̗v�f�ԍ����v�f�͈͓��Ȃ�
		// �ڑ�������J�n�ƏI���n�_�ɐݒ�
		if (i + 1 < mPoints.size())
		{
			rayStart = mPoints[i]->Position();
			rayEnd = mPoints[i + 1]->Position();
			// �ڑ������m�Ōq�����Ă���
			isPoints = true;
		}
		// �Z�b�g�ɂȂ��Ă��Ȃ��ꍇ�A�ڑ����Ə�̐ڑ�����ݒ�
		else
		{
			// ��̐ڑ����̍��W
			rayStart = mpPoint->Position();
			// �ڑ����̍��W
			rayEnd = mPoints[i]->Position();

		}

		// �ݒ肳��Ă���R���C�_�[�����Ԃɒ��ׂ�
		for (CCollider* c : mColliders)
		{
			// ���C�ƃR���C�_�[�̏Փ˔�����s��
			if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
			{
				DeleteConnectPoint(i);
				// �ڑ������m�Ōq�����Ă���Ȃ�
				if (isPoints)
				{
					// �ڑ�������폜
					DeleteConnectPoint(i);
				}
				// ��ƂȂ����Ă���Ȃ�
				else
				{
					// ��̐ڑ��𖳌�
					SetWandConnect(false);
				}
			}
		}
	}
}

// �Փ˔�����s���R���C�_�[�����X�g�ɒǉ�
void CConnectPointManager::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// �Փ˔�����s���R���C�_�[�����X�g�����菜��
void CConnectPointManager::RemoveCollider(CCollider* col)
{
	mColliders.remove(col);
}

// �Փ˔�����s���R���C�_�[�����Z�b�g
void CConnectPointManager::ResetCollider()
{
	mColliders.clear();
}

// �ڑ����𐶐�
void CConnectPointManager::CreateConnectPoint(CConnectTarget* connectTarget)
{
	// �ڑ����Ă��鐔���ő�l�Ɠ����ꍇ
	// �ŌÂ̐ڑ������폜����
	if (mConnectMaxNum == mPoints.size())
	{
		DeleteConnectPoint(0);
		// �ő�l��1����Ȃ���ΐڑ�������폜
		if (mConnectMaxNum != 1)
		{
			DeleteConnectPoint(0);
		}
	}
	// �ڑ����𐶐�
	CConnectPoint* point = new CConnectPoint(connectTarget->GetConnectObj());
	// ���W��ݒ�
	point->Position(connectTarget->Position());
	// �e�q�ݒ�
	point->SetParent(connectTarget);
	// ���X�g�ɒǉ�
	mPoints.push_back(point);

	// �ڑ�������Ȃ�
	if (mPoints.size() % 2 == 1)
	{
		// ��ɐڑ�
		SetWandConnect(true);
		// ��Ɛڑ����̃^�[�Q�b�g��ݒ�
		SetConnectWandTarget(connectTarget);
	}
	// �����Ȃ�
	else
	{
		// ��̐ڑ�����
		SetWandConnect(false);
	}
}

// �ڑ���������
void CConnectPointManager::DeleteConnectPoint(int num)
{
	// �T�C�Y��0�Ȃ珈�����Ȃ�
	if (mPoints.size() == 0) return;
	// �ꎞ�ۑ�
	CConnectPoint* point = mPoints[num];
	// �v�f���珜�O
	mPoints.erase(mPoints.begin() + num);
	// �폜
	point->Kill();
}

// �Ō�̗v�f����������
void CConnectPointManager::DeleteLastConnectPoint()
{	
	// �T�C�Y��0�Ȃ珈�����Ȃ�
	if (mPoints.size() == 0) return;
	// �ꎞ�ۑ�
	CConnectPoint* point = mPoints[mPoints.size() - 1];
	// �v�f���珜�O
	mPoints.pop_back();
	// �폜
	point->Kill();
}

// ��̐ڑ����̗L��������ݒ�
void CConnectPointManager::SetWandConnect(bool isOnOff)
{
	mpPoint->SetEnable(isOnOff);
	mpPoint->SetShow(isOnOff);

	// �ڑ���������Ƃ�
	if (!isOnOff)
	{
		// ��Ɛڑ����̃^�[�Q�b�g������
		SetConnectWandTarget(nullptr);
	}
}

// �񂪐ڑ�����Ă��邩
bool CConnectPointManager::GetWandConnect()
{
	if (mpPoint == nullptr)
		return false;

	return mpPoint->IsShow();
}

// ��Ɛڑ����̃^�[�Q�b�g��ݒ�
void CConnectPointManager::SetConnectWandTarget(CConnectTarget* connectTarget)
{
	mpConnectWandTarget = connectTarget;
}

// ��Ɛڑ����̃^�[�Q�b�g���擾
CConnectTarget* CConnectPointManager::GetConnectWandTarget()
{
	return mpConnectWandTarget;
}

// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋�����ݒ�
void CConnectPointManager::SetWandConnectDistance()
{
	// �񂪐ڑ�����Ă���Ȃ�
	if (GetWandConnect())
	{
		// �v���C���[�̍��W
		CVector rayStart = CPlayer::Instance()->Position();
		// ��ƌq�����Ă���ڑ����̍��W
		CVector rayEnd = mPoints[mPoints.size() - 1]->Position();
		// 2�_�̋���
		float distance = (rayEnd - rayStart).Length();
		// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋�����ݒ�
		mWandConnectDistance = distance;
	}
	// ���Ȃ��Ȃ�
	else
	{
		// �s���l��ݒ�
		mWandConnectDistance = -1.0f;
	}
}

// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋������擾
float CConnectPointManager::GetWandConnectDistance()
{
	return mWandConnectDistance;
}

// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋�����ݒ�
void CConnectPointManager::SetWandConnectDistance(int sign)
{
	mWandConnectDistance += sign * PULL_POWER / 2 *Times::DeltaTime();
}

// ��Ɛڑ����Ă���I�u�W�F�N�g���󒆂̐ڑ��I�u�W�F�N�g��
bool CConnectPointManager::IsWandConnectAirObject()
{
	// �񂪐ڑ�����Ă��邩
	if (GetWandConnect())
	{
		// ��Ɛڑ����̃I�u�W�F�N�g
		CConnectObject* obj = GetConnectWandTarget()->GetConnectObj();
		if (obj != nullptr)
		{
			// �󒆂̐ڑ��I�u�W�F�N�g�Ȃ�
			if (obj->GetConnectObjTag() == EConnectObjTag::eAir)
			{
				return true;
			}
		}
	}

	return false;
}

// ��̐�̐ڑ����̈ʒu�����
void CConnectPointManager::WandPos()
{
	// �v���C���[���擾
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());

	if (player != nullptr)
	{
		CWand* wand = player->GetWand();
		// ��̐�ɐڑ����̈ʒu��ݒ�
		CVector offsetPos = wand->Matrix().Position();
		offsetPos += wand->Matrix().VectorX() * POINT_OFFSET_POSX;
		offsetPos += wand->Matrix().VectorY() * POINT_OFFSET_POSY;
		offsetPos += wand->Matrix().VectorZ() * POINT_OFFSET_POSZ;
		mpPoint->Position(offsetPos);
	}
}
