#pragma once
#include "CGameMenuBase.h"
class CJobLiberationMenu;
class CJobUpgradeMenu;

class CBuyMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="menuItemPathList">メニューのアイテム画像のパスリスト</param>
	/// <param name="menuSelectPath">メニューのセレクト画像のパス</param>
	CBuyMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath);
	~CBuyMenu();

	void Decide(int select) override;

	void Update();

private:
	CJobLiberationMenu* mpJobLiberationMenu;// 仕事解放メニュー
	CJobUpgradeMenu* mpJobUpgradeMenu;		// 仕事強化メニュー
};