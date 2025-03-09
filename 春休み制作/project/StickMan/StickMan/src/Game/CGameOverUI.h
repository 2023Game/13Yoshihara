#pragma once
#include "CEndUIBase.h"

class CGameOverUI : public CEndUIBase
{
public:
	// �R���X�g���N�^
	CGameOverUI();
	// �f�X�g���N�^
	~CGameOverUI();

	// �O���֖߂邩
	bool IsPreDay() const;
	// �Q�[�����I�����邩
	bool IsExitGame() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// [�O����]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickPreDay();
	// [�I��]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickQuit();
};