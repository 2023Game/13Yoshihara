#pragma once

class CDropItem;

// �A�C�e���𗎂Ƃ��N���X
class CItemDrop
{
public:
	// �R���X�g���N�^
	CItemDrop();
	// �f�X�g���N�^
	~CItemDrop();

	// ���Ƃ��A�C�e����ݒ�
	void SetDropItem(CDropItem* item);
	// ���Ƃ��A�C�e���̍��W��ݒ�
	void SetDropItemPos(CVector pos);

	// �A�C�e���𗎂Ƃ�
	void Drop();

private:
	// ���Ƃ��A�C�e��
	CDropItem* mpDropItem;
};