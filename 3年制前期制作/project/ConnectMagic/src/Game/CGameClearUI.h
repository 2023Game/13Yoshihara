#pragma once
#include "CEndUIBase.h"

class CFont;
class CImage;
class CExpandButton;

class CGameClearUI : public CEndUIBase
{
public:
	// �R���X�g���N�^
	CGameClearUI();
	// �f�X�g���N�^
	~CGameClearUI();

	// �^�C�g���֖߂邩
	bool IsReturnTitle() const;
	// �Q�[�����I�����邩
	bool IsExitGame() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// [�^�C�g����]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickTitle();
	// [�I��]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickQuit();
};