#include "CPunisherCollector.h"
#include "CTrashPlayer.h"

// �R���X�g���N�^
CPunisherCollector::CPunisherCollector(CObjectBase* owner, std::vector<CNavNode*> patrolPoints)
	: CCollector(true, owner, patrolPoints)
{
}

// �f�X�g���N�^
CPunisherCollector::~CPunisherCollector()
{
}

// �ǐՏ�Ԃւ̈ڍs�̏������`�F�b�N
bool CPunisherCollector::ChangeChase()
{
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// ��������������Ă��Ȃ��ꍇ
	if (!player->GetStickCollector())
	{
		// �ǐՏ�Ԃֈڍs
		ChangeState(EState::eChase);
		return true;
	}
	return false;
}

// �ǐՏ�Ԃ��瑼�̏�Ԃֈڍs�̏������`�F�b�N
bool CPunisherCollector::ChangeChaseToOther()
{
	// �v���C���[�N���X�擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// ���ɉ�������������Ă���ꍇ
	// �ǂ�������̂���߂�
	if (player->GetStickCollector())
	{
		ChangeState(EState::eIdle);
		return true;
	}
	return false;
}