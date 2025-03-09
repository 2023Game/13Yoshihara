#pragma once

class CTextUI2D;
class CTrashBagStatus;

// �X�R�A�\��UI�̊��N���X
class CScoreUIBase
{
public:
	// �X�R�A�\��UI�̃C���X�^���X���擾
	static CScoreUIBase* Instance();
	// �R���X�g���N�^
	CScoreUIBase();
	// �f�X�g���N�^
	~CScoreUIBase();

	// �X�V
	void Update();

	// �X�R�A���v�Z���Ď擾����
	virtual int GetScore() const = 0;

protected:
	// �X�R�A�\��UI�̃C���X�^���X
	static CScoreUIBase* spInstance;

	// �e�L�X�gUI
	CTextUI2D* mpScoreUI;
};