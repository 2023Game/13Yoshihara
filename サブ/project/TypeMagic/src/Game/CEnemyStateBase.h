#pragma once

class CEnemy;

// �G�̏�Ԃ̊��N���X
class CEnemyStateBase
{
public:
	// �f�X�g���N�^
	virtual ~CEnemyStateBase() = default;
	// �J�n���̏���
	virtual void Start(CEnemy* enemy);
	// �X�V���̏���
	virtual void Update(CEnemy* enemy) = 0;
	// �I�����̏���
	virtual void End(CEnemy* enemy) = 0;
	// ���O���擾
	virtual std::string GetName() const = 0;
	// �����r���̏���
	void Cast(CEnemy* enemy, bool atkSpell);
};