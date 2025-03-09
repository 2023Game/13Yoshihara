#pragma once
#include "CGameMenuBase.h"
#include "JobType.h"

class CJobUnlockMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="prevMenu">��O�̃��j���[</param>
	CJobUnlockMenu(CGameMenuBase* prevMenu);
	// �f�X�g���N�^
	~CJobUnlockMenu();

	// �X�V
	void Update();
	
private:
	// [TRASH]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickTrash();
	// [DELIVERY]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickDelivery();

	// �A�����b�N���z�����点����
	bool IncreaseMoney(EJobType jobType);
};