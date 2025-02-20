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

	// �Q�[�����I�����邩
	bool IsExitGame() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// [�I��]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickQuit();
};