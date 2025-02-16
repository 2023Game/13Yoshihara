#pragma once
#include "CGameMenuBase.h"
class CJobUnlockMenu;
class CJobUpgradeMenu;
class CStageSelectMenu;

class CBuyMenu : public CGameMenuBase
{
public:
	// �R���X�g���N�^
	CBuyMenu();
	// �f�X�g���N�^
	~CBuyMenu();

	// �X�V
	void Update();

private:
	// [UNLOCK]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickUnlock();
	// [UPGRADE]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickUpgrade();
	// [SELECT]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickSelect();

	CJobUnlockMenu* mpJobUnlockMenu;		// �d���A�����b�N���j���[
	CJobUpgradeMenu* mpJobUpgradeMenu;		// �d���������j���[
	CStageSelectMenu* mpStageSelectMenu;	// �d���I�����j���[
};