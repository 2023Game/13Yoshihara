#pragma once
#include "CEnemyBase.h"
#include "CSpellCaster.h"

// �G�̊��N���X��
// �����r���N���X���p�������G�N���X
class CEnemy : public CEnemyBase, public CSpellCaster
{
public:
	// �R���X�g���N�^
	CEnemy();
	// �f�X�g���N�^
	~CEnemy();

	// �X�V
	void Update() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �o�ߎ���
	float mElapsedTime;
};