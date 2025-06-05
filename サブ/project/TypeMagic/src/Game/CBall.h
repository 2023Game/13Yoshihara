#pragma once
#include "CSpellBase.h"

// �{�[������
// �����x�[�X�N���X���p��
class CBall : public CSpellBase
{
public:
	// �R���X�g���N�^
	CBall(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target, int generateNum = 0);
	// �f�X�g���N�^
	~CBall();

	// �X�V����
	void Update() override;

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ���˒��̍X�V
	void UpdateShooting() override;

	// �����ԍ�
	int mGenerateNum;
};