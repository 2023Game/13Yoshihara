#pragma once
#include "CGameMenuBase.h"

class CJobUpgradeMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="menuItemPathList">メニューのアイテム画像のパスリスト</param>
	/// <param name="menuSelectPath">メニューのセレクト画像のパス</param>
	/// <param name="prevMenu">一つ前のメニュー</param>
	CJobUpgradeMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath, CGameMenuBase* prevMenu);
	~CJobUpgradeMenu();

	void Decide(int select) override;

	void Update();
};