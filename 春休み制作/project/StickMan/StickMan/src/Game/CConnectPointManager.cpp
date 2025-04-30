#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "Primitive.h"

// ���C��L�΂��鋗��
#define RAY_DISTANCE 1000.0f

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
	return spInstance;
}

// �R���X�g���N�^
CConnectPointManager::CConnectPointManager()
	: CTask(ETaskPriority::eNone,0,ETaskPauseType::eGame)
	, mConnectMaxNum(DEFAULT_CONNECT_NUM)
{
	spInstance = this;

	// ��p�̐ڑ������̃r���{�[�h
	mpPoint = new CConnectPoint();
	// �ŏ��͔�\��
	mpPoint->SetEnable(false);
	mpPoint->SetShow(false);
	// �X�P�[������
	mpPoint->Scale(mpPoint->Scale() * POINT_SCALE);
}

// �f�X�g���N�^
CConnectPointManager::~CConnectPointManager()
{
	spInstance = nullptr;
}

// �X�V
void CConnectPointManager::Update()
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

// �`��
void CConnectPointManager::Render()
{
	for (int i = 0; i < mPoints.size(); i += 2)
	{
		CVector rayStart;
		CVector rayEnd;
		// i�̎��̗v�f�ԍ����v�f�͈͓��Ȃ�
		// �ڑ������m����Ōq����
		if (i + 1 < mPoints.size())
		{
			rayStart = mPoints[i]->Position();
			rayEnd = mPoints[i + 1]->Position();
		}
		// �Z�b�g�ɂȂ��Ă��Ȃ��ڑ�������ɂȂ���
		else
		{
			// ��̐ڑ����̍��W
			rayStart = mpPoint->Position();
			// �ڑ����̍��W
			rayEnd = mPoints[i]->Position();

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
	CVector rayEnd = rayStart + dir * RAY_DISTANCE;
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

// �ڑ����𐶐�
void CConnectPointManager::CreateConnectPoint(CVector createPos)
{
	// �ڑ����Ă��鐔���ő�l�Ɠ����ꍇ
	// �ŌÂ̐ڑ������폜����
	if (mConnectMaxNum == mPoints.size())
	{
		// �ő�l���P�̏ꍇ�����͈��������
		if (mConnectMaxNum == 1)
		{
			DeleteConnectPoint(0);
		}
		// ����ȊO��2����
		else
		{
			DeleteConnectPoint(0);
			DeleteConnectPoint(0);
		}
	}
	// �ڑ����𐶐�
	CConnectPoint* point = new CConnectPoint();
	// ���W��ݒ�
	point->Position(createPos);
	// ���X�g�ɒǉ�
	mPoints.push_back(point);

	// �ڑ�������Ȃ�
	if (mPoints.size() % 2 == 1)
	{
		// ��ɐڑ�
		SetWandConnect(true);
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
}

// �񂪐ڑ�����Ă��邩
bool CConnectPointManager::GetWandConnect()
{
	return mpPoint->IsShow();
}
