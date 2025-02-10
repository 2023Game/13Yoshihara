#include "CDeliveryItem.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"
#include "CSound.h"

#define ITEM_HEIGHT	2.5f	// ����
#define ITEM_WIDTH	4.5f	// ��
#define ITEM_RADIUS	2.75f	// ���a

// ���ł܂ł̎���
#define DELETE_TIME 3.0f

#define SE_VOLUME 0.5f

// �X�P�[���̔{��
#define SCALE_RATIO 5.0f

// �R���X�g���N�^
CDeliveryItem::CDeliveryItem(CObjectBase* owner)
	: CObjectBase(ETag::eBullet, ETaskPriority::eWeapon,
		0, ETaskPauseType::eGame)
	, CDeliveryItemStatus()
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mpOwner(owner)
	, mOwnerPos(owner->Position())
	, mIsMove(true)
	, mIsPlayer(false)
{
	// �v���C���[��������Ȃ�
	if (mpOwner == CDeliveryPlayer::Instance())
	{
		// ���˂������𑝂₷
		CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(mpOwner);
		player->IncreaseShotNum();
		// �v���C���[��������
		mIsPlayer = true;
	}
	Scale(Scale() * SCALE_RATIO);
	// �S�[�������擾
	mpGoalSE = CResourceManager::Get<CSound>("GetSE");
	// ���f���擾
	mpModel = CResourceManager::Get<CModel>("DeliveryItem");
	
	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryItem::~CDeliveryItem()
{
	SAFE_DELETE(mpBodyCol);
}

// �X�V
void CDeliveryItem::Update()
{
	// �ړ�����Ȃ�
	if (mIsMove)
	{
		Position(Position() + mMoveSpeed);
	}
	else
	{
		CVector move = mpOwner->Position() - mOwnerPos;
		Position(Position() + move);
		mOwnerPos = mpOwner->Position();
	}

	// ���ł܂ł̎��Ԃ��o�߂��Ă��Ȃ��Ȃ�
	if (mElapsedTime < DELETE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	// ���ł܂ł̎��Ԃ��o�߂�����
	else
	{
		// �폜
		Kill();
	}
}

// �`��
void CDeliveryItem::Render()
{
	mpModel->Render(Matrix());
}

// �Փˏ���
void CDeliveryItem::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �R���C�_�[�Ǝ����̎����傪�����Ȃ珈�����Ȃ�
			if (mpOwner == other->Owner()) return;
			// �v���C���[�擾
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			player->TakeDamage(GetDamage(), this);
			// ����
			Kill();
		}
		// �G�̏ꍇ
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �R���C�_�[�Ǝ����̎����傪�����Ȃ珈�����Ȃ�
			if (mpOwner == other->Owner()) return;
			// �G�擾
			CDeliveryEnemy* enemy = dynamic_cast<CDeliveryEnemy*>(other->Owner());
				enemy->TakeDamage(GetDamage(), this);
			// �v���C���[��������Ȃ�
			if (mIsPlayer)
			{
				// �����������𑝂₷
				CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(mpOwner);
				player->IncreaseHitNum();
			}
			// ����
			Kill();
		}
		// �S�[���̏ꍇ
		else if (other->Layer() == ELayer::eGoal)
		{
			// �����傪�v���C���[
			CDeliveryPlayer* owner = dynamic_cast<CDeliveryPlayer*>(mpOwner);
			if (owner != nullptr)
			{
				// �z�B��������1���₷
				owner->IncreaseDeliveryNum();
				// �����������𑝂₷
				owner->IncreaseHitNum();
				// �S�[�������Đ�
				mpGoalSE->Play(SE_VOLUME, true);
			}
			// �������~
			mIsMove = false;
			// �Փ˔�����~
			SetEnableCol(false);
			// ��������t�B�[���h�ɂ���
			mpOwner = other->Owner();
			// �t�B�[���h�̍��W���擾
			mOwnerPos = mpOwner->Position();
		}
		// �ǂ̏ꍇ
		else if (other->Layer() == ELayer::eWall)
		{
			// �������~
			mIsMove = false;
			// �Փ˔�����~
			SetEnableCol(false);
			// ��������t�B�[���h�ɂ���
			mpOwner = other->Owner();
			// �t�B�[���h�̍��W���擾
			mOwnerPos = mpOwner->Position();
		}
	}
}

// �ړ���ݒ�
void CDeliveryItem::SetMoveSpeed(CVector moveSpeed)
{
	mMoveSpeed = moveSpeed;
}

// �R���C�_�\�𐶐�
void CDeliveryItem::CreateCol()
{
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, ITEM_HEIGHT, ITEM_WIDTH - ITEM_RADIUS),
		CVector(0.0f, ITEM_HEIGHT, -ITEM_WIDTH + ITEM_RADIUS),
		ITEM_RADIUS * Scale().X()
	);
}
