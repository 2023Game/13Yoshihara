#pragma once
#include "CCharaBase.h"

/*
�S�~���W�Ԃ��琶�������
������̃N���X
*/
class CCollector : public CCharaBase
{
public :
	// �R���X�g���N�^
	CCollector();
	// �f�X�g���N�^
	~CCollector();

	// �{�̃R���C�_�\���擾
	CCollider* GetBodyCol() const;

private:
};