#pragma once
#include "CGameMenuBase.h"
class CJobLiberationMenu;
class CJobUpgradeMenu;

class CBuyMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="menuItemPathList">���j���[�̃A�C�e���摜�̃p�X���X�g</param>
	/// <param name="menuSelectPath">���j���[�̃Z���N�g�摜�̃p�X</param>
	CBuyMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath);
	~CBuyMenu();

	void Decide(int select) override;

	void Update();

private:
	CJobLiberationMenu* mpJobLiberationMenu;// �d��������j���[
	CJobUpgradeMenu* mpJobUpgradeMenu;		// �d���������j���[
};