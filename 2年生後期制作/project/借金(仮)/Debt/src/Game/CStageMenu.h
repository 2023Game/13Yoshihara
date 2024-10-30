#pragma once
#include "CGameMenuBase.h"

class CStageMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="menuItemPathList">メニューのアイテム画像のパスリスト</param>
	/// <param name="menuSelectPath">メニューのセレクト画像のパス</param>
	CStageMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath);
	~CStageMenu();

	void Decide(int select) override;

	void Update();

private:
};