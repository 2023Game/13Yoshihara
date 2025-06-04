#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "Primitive.h"
#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "Maths.h"

// ���C��L�΂��鋗���̍ő�̏����l
#define DEFAULT_RAY_MAX_DISTANCE 100.0f
// ���C���k�߂�鋗���̍ŏ�
#define RAY_MIN_DISTANCE 15.0f

// �L�΂��鋗���̋����l
#define RAY_MAX_UPGRADE_SCALE 10.0f
// �ڑ��ł���ő吔�̑����ɕK�v�ȋ����A�C�e���̐�
#define CONNECT_UPGRADE_NUM 10

// �^�[�U���̍ŒZ����
#define TARZAN_MIN_DISTANCE 30.0f

// �ڑ����̃I�t�Z�b�g���W
#define POINT_OFFSET_POSX 0.0f
#define POINT_OFFSET_POSY 45.0f
#define POINT_OFFSET_POSZ 0.0f

// �ڑ����̃X�P�[��
#define POINT_SCALE 0.5f

// �ڑ��ł���ő吔�̏����l
#define DEFAULT_CONNECT_NUM 1

// ��̏d��
#define WAND_WEIGHT 0.5f

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
	, mConnectMaxDist(DEFAULT_RAY_MAX_DISTANCE)
	, mUpgradeItemNum(0)
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

	// �q�����Ă��鏈�������s
	Connect();
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
				// �y�A�ō폜
				DeleteConnectPointPair(i);
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
		if (distance > mConnectMaxDist)
		{
			// �ڑ��폜
			DeleteConnectPointPair(i);
			// �ڑ������m�Ȃ�
			if (isPoints)
			{
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
			// ��������
			connectObj->Pull(pullDir, WAND_WEIGHT);
		}
	}
}

// �q�����Ă��鏈�������s
void CConnectPointManager::Connect()
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

			// �ǂ��炩����Ȃ珈�����Ȃ�
			if (connectObj1 == nullptr || connectObj2 == nullptr) return;
			// �q����
			connectObj1->Connect(connectObj2);

			// �q����
			connectObj2->Connect(connectObj1);
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
	CVector rayEnd = rayStart + dir * mConnectMaxDist;
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
				// �y�A�폜
				DeleteConnectPointPair(i);
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
		// �폜
		DeleteConnectPointPair(0);
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
		// ���������ڑ����ƈ�O�̐ڑ������y�A�ɐݒ�
		int num = mPoints.size() - 2;
		point->SetPair(mPoints[num]);
		mPoints[num]->SetPair(point);
	}
}

// �w��̃I�u�W�F�N�g���e�̐ڑ������폜
void CConnectPointManager::DeleteConnectPoint(CConnectObject* obj)
{
	// �폜����ԍ�
	std::vector<int> deleteNum;
	// ��납��`�F�b�N
	for (int i = mPoints.size() - 1; i >= 0; i--)
	{
		// ��v������
		if (mPoints[i]->GetConnectObj() == obj)
		{
			// �폜����ԍ��ɒǉ�
			deleteNum.push_back(i);
		}
	}

	// �y�A�ō폜
	for (int num : deleteNum)
	{
		DeleteConnectPointPair(num);
	}
}

// �ڑ������폜
void CConnectPointManager::DeleteConnectPoint(int num)
{
	// �T�C�Y��0��num���͈͊O�Ȃ珈�����Ȃ�
	if (mPoints.size() == 0 || mPoints.size() <= num) return;
	// �ꎞ�ۑ�
	CConnectPoint* point = mPoints[num];
	// �v�f���珜�O
	mPoints.erase(mPoints.begin() + num);
	// �폜
	point->Kill();
}

// �ڑ������y�A�ō폜
void CConnectPointManager::DeleteConnectPointPair(int num)
{
	// �ڑ���
	CConnectPoint* point1 = mPoints[num];
	CConnectPoint* point2 = point1->GetPair();

	// �y�A��nullptr����Ȃ����
	if (point2 != nullptr)
	{
		// �z�񂩂��菜��
		mPoints.erase(
			std::remove_if(
				mPoints.begin(), mPoints.end(),
				[&](CConnectPoint* deletePoint)
				{return deletePoint == point1 || deletePoint == point2; }
			),
			mPoints.end()
		);
		// �폜
		point1->Kill();
		point2->Kill();
	}
	// �y�A��nullptr�Ȃ�
	else
	{
		// ������폜
		DeleteConnectPoint(num);
		// ��̐ڑ�������
		SetWandConnect(false);
	}
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

	// �^�[�U���̍ŒZ�������Z���Ȃ�
	if (mWandConnectDistance < TARZAN_MIN_DISTANCE)
	{
		// �ŒZ�����ɐݒ�
		mWandConnectDistance = TARZAN_MIN_DISTANCE;
	}
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

// �ڑ��ł��鋗���𑝉�
void CConnectPointManager::AddConnectMaxDist()
{
	mConnectMaxDist += RAY_MAX_UPGRADE_SCALE;
}

// �ڑ��ł��鐔�𑝉�
void CConnectPointManager::AddConnectMaxNum()
{
	mConnectMaxNum++;
}

// �����A�C�e���̊l�����𑝉�
void CConnectPointManager::AddUpgradeItemNum()
{
	mUpgradeItemNum++;

	// �ڑ��ł��鋗���𑝉�
	AddConnectMaxDist();

	// �ڑ��ł���ő吔�̑����ɕK�v�Ȑ��Ȃ�
	if (mUpgradeItemNum % CONNECT_UPGRADE_NUM == 0)
	{
		// �ڑ��ł��鐔�̑���
		AddConnectMaxNum();
	}
}

// ��̐�̐ڑ����̈ʒu�����
void CConnectPointManager::WandPos()
{
	// �v���C���[���擾
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());

	if (player != nullptr)
	{
		CWand* wand = player->GetWand();
		if (wand != nullptr)
		{
			// ��̐�ɐڑ����̈ʒu��ݒ�
			CVector offsetPos = wand->Matrix().Position();
			offsetPos += wand->Matrix().VectorX() * POINT_OFFSET_POSX;
			offsetPos += wand->Matrix().VectorY() * POINT_OFFSET_POSY;
			offsetPos += wand->Matrix().VectorZ() * POINT_OFFSET_POSZ;
			mpPoint->Position(offsetPos);
		}
	}
}
