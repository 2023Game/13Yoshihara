#pragma once
#include "CCollector.h"

class CPunisherCollector : public CCollector
{
public:
	// �R���X�g���N�^
	CPunisherCollector(CObjectBase* owner,
		std::vector<CNavNode*> patrolPoints);
	// �f�X�g���N�^
	~CPunisherCollector();

private:
	// �ǐՏ�Ԃւ̈ڍs�̏������`�F�b�N
	bool ChangeChase() override;
	// �ǐՏ�Ԃ��瑼�̏�Ԃֈڍs�̏������`�F�b�N
	bool ChangeChaseToOther() override;
};