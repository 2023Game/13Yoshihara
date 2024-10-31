#pragma once
#include "CGameMenuBase.h"

class CGameMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="menuItemPathList">メニューのアイテム画像のパスリスト</param>
	/// <param name="menuSelectPath">メニューのセレクト画像のパス</param>
	CGameMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath);
	~CGameMenu();

	void Decide(int select) override;

	void Update();
};