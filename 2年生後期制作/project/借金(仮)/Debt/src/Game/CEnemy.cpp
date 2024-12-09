#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
{
}

CEnemy::~CEnemy()
{
}

//�X�V����
void CEnemy::Update()
{
	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ�
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::ePatrol:			UpdatePatrol();			break;
	case EState::eChase:			UpdateChase();			break;
	case EState::eLost:				UpdateLost();			break;
	case EState::eDamageStart:		UpdateDamageStart();	break;
	case EState::eDamage:			UpdateDamage();			break;
	case EState::eDamageEnd:		UpdateDamageEnd();		break;
	case EState::eJumpStart:		UpdateJumpStart();		break;
	case EState::eJump:				UpdateJump();			break;
	case EState::eJumpEnd:			UpdateJumpEnd();		break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttack:			UpdateAttack();			break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	case EState::eCriticalStart:	UpdateCriticalStart();	break;
	case EState::eCritical:			UpdateCritical();		break;
	case EState::eCriticalEnd:		UpdateCriticalEnd();	break;
	case EState::eOpenClose:		UpdateOpenClose();		break;
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
}

// �Փˏ���
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// �ҋ@���
void CEnemy::UpdateIdle()
{
}

// ���񏈗�
void CEnemy::UpdatePatrol()
{
}

// �ǐՏ���
void CEnemy::UpdateChase()
{
}

// �v���C���[��������������
void CEnemy::UpdateLost()
{
}

// ��e�J�n
void CEnemy::UpdateDamageStart()
{
}

// ��e�m�b�N�o�b�N
void CEnemy::UpdateDamage()
{
}

// ��e�I��
void CEnemy::UpdateDamageEnd()
{
}

// �W�����v�J�n
void CEnemy::UpdateJumpStart()
{
}

// �W�����v��
void CEnemy::UpdateJump()
{

}

// �W�����v�I��
void CEnemy::UpdateJumpEnd()
{
}

// �U���J�n
void CEnemy::UpdateAttackStart()
{
}

// �U����
void CEnemy::UpdateAttack()
{
}

// �U���I��
void CEnemy::UpdateAttackEnd()
{
}

// �N���e�B�J���U���J�n
void CEnemy::UpdateCriticalStart()
{
}

// �N���e�B�J���U����
void CEnemy::UpdateCritical()
{
}

// �N���e�B�J���U���I��
void CEnemy::UpdateCriticalEnd()
{
}

// �W���J����
void CEnemy::UpdateOpenClose()
{
}

// �A�j���[�V�����؂�ւ�
void CEnemy::ChangeAnimation(EAnimType type)
{
}

// �X�e�[�^�X�؂�ւ�
void CEnemy::ChangeState(EState state)
{
	// ���ɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// ��Ԃ̕�������擾
std::string CEnemy::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "�ҋ@";
	case EState::ePatrol:			return "����";
	case EState::eChase:			return "�ǐ�";
	case EState::eLost:				return "������";
	case EState::eDamageStart:		return "��e�J�n";
	case EState::eDamage:			return "��e��";
	case EState::eDamageEnd:		return "��e�I��";
	case EState::eJumpStart:		return "�W�����v�J�n";
	case EState::eJump:				return "�W�����v��";
	case EState::eJumpEnd:			return "�W�����v�I��";
	case EState::eAttackStart:		return "�U���J�n";
	case EState::eAttack:			return "�U����";
	case EState::eAttackEnd:		return "�U���I��";
	case EState::eCriticalStart:	return "�N���e�B�J���U���J�n";
	case EState::eCritical:			return "�N���e�B�J���U����";
	case EState::eCriticalEnd:		return "�N���e�B�J���U���I��";
	case EState::eOpenClose:		return "�J��";
	}
	return "";
}
