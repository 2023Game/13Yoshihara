#pragma once
#include "CGameMenuBase.h"

// �z�[���V�[���ł̃��j���[
class CMenu : public CGameMenuBase
{
public:
	// �R���X�g���N�^
	CMenu();
	// �f�X�g���N�^
	~CMenu();

	// �X�V
	void Update();

private:
	// [�^�C�g����}�N���b�N���̃R�[���o�b�N�֐�
	void OnClickTitle();
};