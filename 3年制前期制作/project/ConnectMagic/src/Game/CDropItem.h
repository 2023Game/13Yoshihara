#pragma once

class CObjectBase;

// ���Ƃ��A�C�e���̃N���X
class CDropItem
{
public:
	// �R���X�g���N�^
	CDropItem(CObjectBase* obj);
	// �f�X�g���N�^
	~CDropItem();

	// �I�u�W�F���擾
	CObjectBase* GetObj() const;

private:
	// ���g�̃I�u�W�F�N�g�N���X�̃|�C���^
	CObjectBase* mpObj;
};