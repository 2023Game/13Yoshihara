#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CGaugeUI3D.h"

#define GRAVITY 0.0625f

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mState(0)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
	, mpHpGauge(nullptr)
{
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
	// �R���C�_�[�폜
	SAFE_DELETE(mpBodyCol);

	// HP�Q�[�W�����݂�����A�ꏏ�ɍ폜����
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

// �I�u�W�F�N�g�폜��`����֐�
void CEnemy::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�̂�HP�Q�[�W�ł���΁A
	// HP�Q�[�W�̃|�C���^����ɂ���
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

//�X�V����
void CEnemy::Update()
{
	//// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ�
	//switch (mState)
	//{
	//case EState::eIdle:				UpdateIdle();			break;
	//case EState::ePatrol:			UpdatePatrol();			break;
	//case EState::eChase:			UpdateChase();			break;
	//case EState::eLost:				UpdateLost();			break;
	//case EState::eDamageStart:		UpdateDamageStart();	break;
	//case EState::eDamage:			UpdateDamage();			break;
	//case EState::eDamageEnd:		UpdateDamageEnd();		break;
	//case EState::eJumpStart:		UpdateJumpStart();		break;
	//case EState::eJump:				UpdateJump();			break;
	//case EState::eJumpEnd:			UpdateJumpEnd();		break;
	//case EState::eAttackStart:		UpdateAttackStart();	break;
	//case EState::eAttack:			UpdateAttack();			break;
	//case EState::eAttackEnd:		UpdateAttackEnd();		break;
	//case EState::eCriticalStart:	UpdateCriticalStart();	break;
	//case EState::eCritical:			UpdateCritical();		break;
	//case EState::eCriticalEnd:		UpdateCriticalEnd();	break;
	//case EState::eOpenClose:		UpdateOpenClose();		break;
	//}

	// �d��
	mMoveSpeedY -= GRAVITY;

	// �ړ�
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;

	//CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
}

// �Փˏ���
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̂��Փ˂���
	if (self == mpBodyCol)
	{
		// �t�B�[���h�ƏՓ˂���
		if (other->Layer() == ELayer::eField)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// �Փ˂����n�ʂ������V�䂩����ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// ���ς̌��ʂ��v���X�ł���΁A���ƏՓ˂���
			if (dot >= 0.0f)
			{
				// �����Ȃǂŏ��ɏォ��Փ˂������i���ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// �ڒn����
				mIsGrounded = true;
				// �ڒn�����n�ʂ̖@�����L�����Ă���
				mGroundNormal = hit.adjust.Normalized();
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A�V��ƏՓ˂���
			else
			{
				// �W�����v�ȂǂœV��ɉ�����Փ˂������i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// �v���C���[�ƏՓ˂����ꍇ
		else if (other->Layer() == ELayer::ePlayer)
		{
			// �������ɂ̂݉����߂����߁A
			// �����߂��x�N�g����Y�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
}

// �`��
void CEnemy::Render()
{
	CXCharacter::Render();
}

// �A�j���[�V�����؂�ւ�
void CEnemy::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// ��Ԑ؂�ւ�
void CEnemy::ChangeState(int state)
{
	// ������Ԃ̏ꍇ�͐؂�ւ��Ȃ�
	if (state == mState) return;

	// ��Ԃ�ύX���āA��Ԋ֘A�̕ϐ��̏�����
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

//// �X�e�[�^�X�؂�ւ�
//void CEnemy::ChangeState(EState state)
//{
//	// ���ɓ�����Ԃł���΁A�������Ȃ�
//	if (state == mState) return;
//
//	mState = state;
//	mStateStep = 0;
//	mElapsedTime = 0.0f;
//}
//
//// ��Ԃ̕�������擾
//std::string CEnemy::GetStateStr(EState state) const
//{
//	switch (state)
//	{
//	case EState::eIdle:				return "�ҋ@";
//	case EState::ePatrol:			return "����";
//	case EState::eChase:			return "�ǐ�";
//	case EState::eLost:				return "������";
//	case EState::eDamageStart:		return "��e�J�n";
//	case EState::eDamage:			return "��e��";
//	case EState::eDamageEnd:		return "��e�I��";
//	case EState::eJumpStart:		return "�W�����v�J�n";
//	case EState::eJump:				return "�W�����v��";
//	case EState::eJumpEnd:			return "�W�����v�I��";
//	case EState::eAttackStart:		return "�U���J�n";
//	case EState::eAttack:			return "�U����";
//	case EState::eAttackEnd:		return "�U���I��";
//	case EState::eCriticalStart:	return "�N���e�B�J���U���J�n";
//	case EState::eCritical:			return "�N���e�B�J���U����";
//	case EState::eCriticalEnd:		return "�N���e�B�J���U���I��";
//	case EState::eOpenClose:		return "�J��";
//	}
//	return "";
//}
