#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "Primitive.h"
#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "Maths.h"

// ���C��L�΂��鋗���̍ő�̏����l
#define RAY_MAX_DISTANCE 100.0f
// ���C���k�߂�鋗���̍ŏ�
#define RAY_MIN_DISTANCE 15.0f

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
	, mpConnectWandTarget(nullptr)
	, mConnectDistance(0.0f)
{
	// ��p�̐ڑ������̃r���{�[�h
	mpWandPoint = new CConnectPoint(nullptr);
	// �ŏ��͔�\��
	mpWandPoint->SetEnable(false);
	mpWandPoint->SetShow(false);
	// �X�P�[������
	mpWandPoint->Scale(mpWandPoint->Scale() * POINT_SCALE);

	// �ڑ���̐ڑ����̃r���{�[�h
	mpConnectPoint = new CConnectPoint(nullptr);
	// �ŏ��͖���
	mpConnectPoint->SetEnable(false);
	mpConnectPoint->SetShow(false);
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

	// �ڑ����ƌq�������������ɏՓ˂����疳��
	RayPoint();

	// �ڑ����Ƃ̋������ő�l��艓����
	// �ŏ��l���߂���ΐڑ��𖳌��ɂ���
	FarOrNearDist();

	// �q�����Ă��鏈��
	Connect();

#if _DEBUG
	CDebugPrint::Print("ConnectDist:%f\n", mConnectDistance);
#endif
}

// �`��
void CConnectPointManager::Render()
{
	// �񂪐ڑ�����Ă��Ȃ���Ώ������Ȃ�
	if (!GetWandConnect()) return;
	// ������`��
	Primitive::DrawLine
	(
		mpWandPoint->Position(), mpConnectPoint->Position(),
		CColor::yellow,
		5.0f
	);
}

// �q�����Ă��鏈�������s
void CConnectPointManager::Connect()
{
	// �񂪐ڑ�����Ă��Ȃ��Ȃ珈�����Ȃ�
	if (!GetWandConnect()) return;
	
	mpConnectPoint->GetConnectObj()->Connect(mpWandPoint->Position(), mpConnectPoint->Position());
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

// 2�_���q�������C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
bool CConnectPointManager::Ray(const CVector& start, const CVector& end, CHitInfo* hit)
{
	return false;
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

// �ڑ����ƌq�������C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
// �Փ˂��Ă�����ڑ�����
void CConnectPointManager::RayPoint()
{
	CHitInfo hit;
	// ���C�̊J�n�ƏI���n�_
	CVector rayStart = mpWandPoint->Position();
	CVector rayEnd = mpConnectPoint->Position();

	// �ݒ肳��Ă���R���C�_�[�����Ԃɒ��ׂ�
	for (CCollider* c : mColliders)
	{
		// ���C�ƃR���C�_�[�̏Փ˔�����s��
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// �ڑ��𖳌�
			DisableConnect(GetConnectWandTarget());
		}
	}
}

// �ڑ����Ƃ̋������ő�l��艓����
// �ŏ��l���߂���ΐڑ��𖳌��ɂ���
void CConnectPointManager::FarOrNearDist()
{
	// �񂪐ڑ�����Ă��Ȃ��Ȃ珈�����Ȃ�
	if (!GetWandConnect()) return;

	// �ő勗����艓���Ȃ�
	if (GetNowConnectDistance() > RAY_MAX_DISTANCE)
	{
		// �ڑ��𖳌�
		DisableConnect(GetConnectWandTarget());
	}
	// �ŏ��������߂��Ȃ�
	else if (GetNowConnectDistance() < RAY_MIN_DISTANCE)
	{
		// �ڑ��𖳌�
		DisableConnect(GetConnectWandTarget());
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

// �ڑ�����L��
void CConnectPointManager::EnableConnect(CConnectTarget* connectTarget)
{
	// ���W��ݒ�
	mpConnectPoint->Position(connectTarget->Position());
	// �e�q�ݒ�
	mpConnectPoint->SetParent(connectTarget);
	// �ڑ��I�u�W�F�N�g��ݒ�
	mpConnectPoint->SetConnectObj(connectTarget->GetConnectObj());

	// ��ɐڑ�
	SetWandConnect(true, connectTarget);

	// �ڑ�����L��
	mpConnectPoint->SetEnable(true);
	mpConnectPoint->SetShow(true); 
	
	// �ڑ����Ƃ̋�����ݒ�
	SetConnectDistance();
}

// �ڑ��𖳌�
void CConnectPointManager::DisableConnect(CConnectTarget* connectTarget)
{
	// �ڑ����̃^�[�Q�b�g�łȂ��Ȃ珈�����Ȃ�
	if (GetConnectWandTarget() != connectTarget) return;

	// ��̐ڑ�����
	SetWandConnect(false, nullptr);

	// �ڑ����𖳌�
	mpConnectPoint->SetEnable(false);
	mpConnectPoint->SetShow(false);
}

// ��̐ڑ����̗L��������ݒ�
void CConnectPointManager::SetWandConnect(bool isOnOff, CConnectTarget* target)
{
	mpWandPoint->SetEnable(isOnOff);
	mpWandPoint->SetShow(isOnOff);

	// ��Ɛڑ����̃^�[�Q�b�g��ݒ�
	SetConnectWandTarget(target);
}

// �񂪐ڑ�����Ă��邩
bool CConnectPointManager::GetWandConnect()
{
	if (mpWandPoint == nullptr)
		return false;

	return mpWandPoint->IsShow();
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
void CConnectPointManager::SetConnectDistance()
{
	// �񂪐ڑ�����Ă���Ȃ�
	if (GetWandConnect())
	{
		// �v���C���[�̍��W
		CVector rayStart = CPlayer::Instance()->Position();
		// ��ƌq�����Ă���ڑ����̍��W
		CVector rayEnd = mpConnectPoint->Position();
		// 2�_�̋���
		float distance = (rayEnd - rayStart).Length();
		// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋�����ݒ�
		mConnectDistance = distance;
	}
	// ���Ȃ��Ȃ�
	else
	{
		// �s���l��ݒ�
		mConnectDistance = -1.0f;
	}
}

// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋������擾
float CConnectPointManager::GetConnectDistance()
{				
	return mConnectDistance;
}

// ���݂̏񂪐ڑ����Ă���ڑ����ƃv���C���[�̋������擾
float CConnectPointManager::GetNowConnectDistance()
{
	// �v���C���[�̍��W
	CVector rayStart = CPlayer::Instance()->Position();
	// ��ƌq�����Ă���ڑ����̍��W
	CVector rayEnd = mpConnectPoint->Position();
	// 2�_�̋���
	float distance = (rayEnd - rayStart).Length();

	return distance;
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
		if (wand != nullptr)
		{
			// ��̐�ɐڑ����̈ʒu��ݒ�
			CVector offsetPos = wand->Matrix().Position();
			offsetPos += wand->Matrix().VectorX() * POINT_OFFSET_POSX;
			offsetPos += wand->Matrix().VectorY() * POINT_OFFSET_POSY;
			offsetPos += wand->Matrix().VectorZ() * POINT_OFFSET_POSZ;
			mpWandPoint->Position(offsetPos);
		}
	}
}
