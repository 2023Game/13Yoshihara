#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

// TODO:種類別のゲームメニューに対応させる
class CGameMenuBase : public CTask
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="menuItemPathList">メニューのアイテム画像のパスリスト</param>
	/// <param name="menuSelectPath">メニューのセレクト画像のパス</param>
	CGameMenuBase(std::vector<std::string> menuItemPathList, std::string menuSelectPath);
	~CGameMenuBase();

	void Open();
	void Close();
	bool IsOpened() const;

	// 決定したボタンの処理
	virtual void Decide(int select);

	void Update() override;
	void Render() override;

protected:
	CImage* mpBackground;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	int mSelectIndex;
	bool mIsOpened;
	CGameMenuBase* mPrevMenu;	// 一つ前のメニューの格納用
};