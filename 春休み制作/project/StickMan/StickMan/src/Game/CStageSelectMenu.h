#pragma once
#include "CGameMenuBase.h"

class CStageSelectMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="prevMenu">��O�̃��j���[</param>
	CStageSelectMenu(CGameMenuBase* prevMenu);
	// �f�X�g���N�^
	~CStageSelectMenu();

	// �X�V
	void Update();

private:
	// [TRASH]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickTrash();
	// [DELIVERY]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickDelivery();
};