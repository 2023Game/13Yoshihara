#include "CPortalFragment.h"
#include "CImage3D.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CFragmentMergeArea.h"
#include "CConnectPointManager.h"
#include "CConnectTarget.h"

// �摜�̍��W
#define IMG_OFFSET_POS CVector(0.0f,10.0f,0.0f)
// �摜�̃T�C�Y
#define IMG_SIZE 2.0f

// �R���C�_�[�̍���
#define HEIGHT 10.0f
// �R���C�_�[�̔��a
#define RADIUS 10.0f

// �����̈ړ����x
#define MERGE_SPEED 10.0f

// ��яオ��̈ړ����x
#define JUMP_MOVE_SPEEDY	100.0f
#define JUMP_MOVE_SPEED		20.0f

// �ҋ@�̏㉺�ړ����x
#define IDLE_MOVE_SPEED 2.0f
// �ҋ@�̏㉺���؂�ւ�鎞��
#define CHANGE_UD_TIME 1.0f

// �R���X�g���N�^
CPortalFragment::CPortalFragment(EFragmentType fragmentType,
	CFragmentMergeArea* mergeArea)
	: CConnectObject(0.1f, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, CDropItem(this)
	, mFragmentType(fragmentType)
	, mpMergeArea(mergeArea)
{
	// �ŏ��͖���
	SetEnable(false);
	SetShow(false);
	// ��яオ����
	ChangeState(EState::eJump);

	std::string path = "";
	// �摜�̃p�X��ݒ�
	switch (fragmentType)
	{
	case EFragmentType::eFragment1:
		path = "Field/PortalFragment/PortalFragment1.png";
		break;
	case EFragmentType::eFragment2:
		path = "Field/PortalFragment/PortalFragment2.png";
		break;
	case EFragmentType::eFragment3:
		path = "Field/PortalFragment/PortalFragment3.png";
		break;
	case EFragmentType::eFragment4:
		path = "Field/PortalFragment/PortalFragment4.png";
		break;
	}
	// �|�[�^���̌��Ђ̉摜�𐶐�
	mpFragmentImg = new CImage3D(
		path,
		ETag::eConnectObject,
		ETaskPriority::eDefault, 0,
		ETaskPauseType::eGame,
		false,
		false
	);
	// �e�ɐݒ�
	mpFragmentImg->SetParent(this);
	// ���ʕ`�悷��
	mpFragmentImg->SetBackFace(true);
	// ���W��ݒ�
	mpFragmentImg->Position(IMG_OFFSET_POS);
	// �T�C�Y��ݒ�
	mpFragmentImg->SetSize(mpFragmentImg->GetSize() * IMG_SIZE);

	// �R���C�_�[�𐶐�
	CreateCol();

	// �ڑ��^�[�Q�b�g�𐶐�
	CreateTarget(IMG_OFFSET_POS);
}

// �f�X�g���N�^
CPortalFragment::~CPortalFragment()
{
	SAFE_DELETE(mpFragmentImg);
}

// �X�V
void CPortalFragment::Update()
{
	switch (mState)
	{
	case CPortalFragment::EState::eJump:
		UpdateJump();
		break;
	case CPortalFragment::EState::eIdle:
		UpdateIdle();
		break;
	case CPortalFragment::EState::eMerge:
		UpdateMerge();
		break;
	}
	// ���N���X�̍X�V����
	CConnectObject::Update();

	mpFragmentImg->Update();
}

// �`��
void CPortalFragment::Render()
{
	// �摜�̕`��
	mpFragmentImg->Render();
}

// �Փˏ���
void CPortalFragment::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CConnectObject::Collision(self, other, hit);

	if (self == mpCol)
	{
		// ���肪���g�̍����ꏊ�̏ꍇ
		if (other->Owner() == mpMergeArea)
		{
			// �d�͖���
			SetGravity(false);
			// �R���C�_�𖳌�
			SetEnableCol(false);
			for (CConnectTarget* target : mTargets)
			{
				// �ڑ�����Ă���Ή���
				CConnectPointManager::Instance()->DisableConnect(target);
				target->SetConnectObj(nullptr);
				target->Kill();
			}
			// �z�����ɂ���
			mTargets.clear();

			// �����J�n
			ChangeState(EState::eMerge);
		}
	}
}

// ��Ԑ؂�ւ�
void CPortalFragment::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
	mMoveSpeedY = 0.0f;
}

// ��яオ��
void CPortalFragment::UpdateJump()
{
	switch (mStateStep)
	{
		// ������̈ړ���ݒ�
	case 0:
		mMoveSpeedY = JUMP_MOVE_SPEEDY * Times::DeltaTime();
		mStateStep++;
		break;

		// �O�����̈ړ���ݒ�
	case 1:
		// �O�����̈ړ�
		mMoveSpeed += VectorZ() * JUMP_MOVE_SPEED * Times::DeltaTime();

		if (mIsGrounded)
		{
			// �ҋ@��
			ChangeState(EState::eIdle);
			return;
		}
		break;
	}
}

// �ҋ@
void CPortalFragment::UpdateIdle()
{
	// ���Ԍo��
	mElapsedTime += Times::DeltaTime();
	// �摜�̍��W
	CVector pos = mpFragmentImg->Position();
	// �ړ�
	CVector move = CVector(0.0f, IDLE_MOVE_SPEED * Times::DeltaTime(), 0.0f);

	switch (mStateStep)
	{
		// ��ړ�
	case 0:
		// �ړ��𔽉f
		pos = pos + move;

		// �؂�ւ�鎞�Ԃ��߂�����
		if (mElapsedTime > CHANGE_UD_TIME)
		{
			mElapsedTime = 0.0f;
			mStateStep++;
		}
		break;

		// ���ړ�
	case 1:
		// �ړ��𔽉f
		pos = pos - move;

		// �؂�ւ�鎞�Ԃ��߂�����
		if (mElapsedTime > CHANGE_UD_TIME)
		{
			mElapsedTime = 0.0f;
			mStateStep--;
		}
		break;
	}

	// ���W��ݒ�
	mpFragmentImg->Position(pos);
}

// ����
void CPortalFragment::UpdateMerge()
{		
	// �ڕW�̍��W
	CVector targetPos = mpMergeArea->Position();
	// �����n�ւ̕���
	CVector dir = targetPos - Position();
	// ����
	float distance = dir.Length();
	// ���K��
	dir.Normalize();
	// �ړ�����
	float moveDistance = MERGE_SPEED * Times::DeltaTime();
	// �ړ������̕����������
	if (distance < moveDistance)
	{
		// �摜�̍��������Z�b�g
		CVector pos = mpFragmentImg->Position();
		pos.Y(targetPos.Y() + IMG_OFFSET_POS.Y());
		mpFragmentImg->Position(pos);
		// ���Ђ�����
		mpMergeArea->SetIsFragment((int)mFragmentType, true);
		// ���W��ݒ�
		Position(targetPos);
		// ����
		SetEnable(false);
	}
	// �Z�����
	else
	{
		// �V�������W���v�Z
		CVector newPos = Position() + dir * moveDistance;
		Position(newPos);
	}
}

// �R���C�_�[�𐶐�
void CPortalFragment::CreateCol()
{
	mpCol = new CColliderSphere(
		this, ELayer::eObject,
		RADIUS
	);
	// �t�B�[�h�ƃv���C���[��
	// �G�Ɛڑ��I�u�W�F�N�g�̒T�m�p�ƌ��Ђ���������ꏊ�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eConnectSearch,
		ELayer::eFragmentMergeArea });
	// ���W��ݒ�
	mpCol->Position(IMG_OFFSET_POS * 0.5f);
}

// ���ׂč폜
void CPortalFragment::Delete()
{
	mpFragmentImg->Kill();
}
