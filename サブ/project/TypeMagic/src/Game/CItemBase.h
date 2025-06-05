#pragma once
#include "CObjectBase.h"

// �A�C�e���̊��N���X
class CItemBase : public CObjectBase
{
public:
	// �R���X�g���N�^
	CItemBase(ETag tag = ETag::eItem, ETaskPriority prio = ETaskPriority::eBackground,
		int sortOrder = 0, ETaskPauseType pause = ETaskPauseType::eGame);
	// �f�X�g���N�^
	~CItemBase();

	// �擾���̏���
	virtual void GetItem() = 0;

private:
	// �R���C�_�[
	CCollider* mpCol;
};