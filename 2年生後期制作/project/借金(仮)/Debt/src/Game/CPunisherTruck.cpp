#include "CPunisherTruck.h"
#include "CGaugeUI3D.h"
#include "CTrashPlayer.h"
#include "CPunisherCollector.h"

// ���d�u���p��Hp�Q�[�W�̉摜�̃p�X
#define PUNISHER_HP_GAUGE_PATH "UI\\punisher_garbageTruck_hp_gauge.png"

// �v���C���[�Ƃ̍��W��r���鎞��臒l
#define THRESHOLD 10.0f

// ����������̃I�t�Z�b�g���W
#define COLLECTOR_OFFSET_POS VectorX() * 10.0f

// �R���X�g���N�^
CPunisherTruck::CPunisherTruck(CModel* model, const CVector& pos, const CVector& rotation,
	ERoadType road, std::vector<CNavNode*> patrolPoints)
	: CGarbageTruck(model, pos, rotation, road, patrolPoints, true)
{
	// ���d�u���p��Hp�Q�[�W��ݒ�
	mpHpGauge = new CGaugeUI3D(this, PUNISHER_HP_GAUGE_PATH);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());
	// �ŏ��͖���
	mpHpGauge->SetEnable(false);
	mpHpGauge->SetShow(false);

	int num = GetCollectorsNum();
	// �������S�Đ������A�����ɂ��Ă���
	for (int i = 0; i < num; i++)
	{
		mpCollectors.push_back(new CPunisherCollector(this,
			{ mpNode0,mpNode1,mpNode2,mpNode3 }));
		// �����ɂ���
		mpCollectors[i]->SetOnOff(false);
	}
}

// �f�X�g���N�^
CPunisherTruck::~CPunisherTruck()
{
}

// �ϐ������Z�b�g
void CPunisherTruck::Reset()
{
	CGarbageTruck::Reset();
	// Hp�����Z�b�g
	SetHp();
}

// �ړ�����
void CPunisherTruck::UpdateMove()
{
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());

	// ���g��Z���W���v���C���[��Z���W����臒l�͈͓̔��̏ꍇ����
	// ������J�n�ł�����W���P�ޒ��ł͂Ȃ��ꍇ
	if (Position().Z() <= player->Position().Z() + THRESHOLD &&
		Position().Z() >= player->Position().Z() - THRESHOLD &&
		CanCollectPosZ() && !mIsReturn)
	{
		// �����Ԃ�
		ChangeState(EState::eCollect);
		return;
	}
	// �ʏ�̃S�~���W�Ԃ̈ړ�����������
	CGarbageTruck::UpdateMove();
}

// �������
void CPunisherTruck::UpdateCollect()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F�ݒ��ύX����
	case 0:
	{
		// �����Ă��Ȃ�
		mIsMove = false;
		// �ړ����x���[���ɂ���
		mMoveSpeed = CVector::zero;
		// �P�ނ܂ł̎��Ԃ������l�ɖ߂�
		SetReturnTime();
		// ������̐l���������l�ɖ߂�
		SetCollectorsNum();

		int size = mpCollectors.size();
		// �������S�ėL���ɂ���
		for (int i = 0; i < size; i++)
		{
			// �L���ɂ���
			mpCollectors[i]->SetOnOff(true);
			// �����̍��W�{�I�t�Z�b�g���W��ݒ�
			mpCollectors[i]->Position(Position() + COLLECTOR_OFFSET_POS * (i + 1));
		}

		mStateStep++;
		break;
	}

	// �X�e�b�v1�F����܂ŉ����Ԃł��葱����
	case 1:
	{
		int size = mpCollectors.size();
		// �����ɂȂ��Ă��L���ɂ�����
		for (int i = 0; i < size; i++)
		{
			if (!mpCollectors[i]->IsEnable())
			{
				// �L���ɂ���
				mpCollectors[i]->SetOnOff(true);
				// �����̍��W�{�I�t�Z�b�g���W��ݒ�
				mpCollectors[i]->Position(Position() + COLLECTOR_OFFSET_POS * (i + 1));
			}
		}
		break;
	}
	}
}
