#pragma once
#include "CItemBase.h"

class CBillBoardImage;

// �����A�C�e��
class CUpgradeItem : public CItemBase
{
public:
	// �R���X�g���N�^
	CUpgradeItem();
	// �f�X�g���N�^
	~CUpgradeItem();

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �擾���̏���
	void GetItem() override;

	// �X�V
	void Update() override;

private:
	// �S�č폜
	void Delete() override;

	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �R���C�_�[
	CCollider* mpCol;

	// �A�C�e���̉摜
	CBillBoardImage* mpItemImg;

	// ��ֈړ����Ă���
	bool mIsMoveUp;
	// �o�ߎ���
	float mElapsedTime;
};