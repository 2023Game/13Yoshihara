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

	// ���菈��
	void Decide(int select) override;

	// �X�V
	void Update();

private:
	CJobUnlockMenu* mpJobUnlockMenu;		// �d���A�����b�N���j���[
	CJobUpgradeMenu* mpJobUpgradeMenu;		// �d���������j���[
	CStageSelectMenu* mpStageSelectMenu;	// �d���I�����j���[
};