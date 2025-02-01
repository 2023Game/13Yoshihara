#pragma once
#include "CTask.h"
#include "CImage.h"
#include "CText.h"
#include <vector>
#include "CSound.h"

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
	// デストラクタ
	~CGameMenuBase();

	// メニューを開く
	void Open();
	// メニューを閉じる
	void Close();
	// 開いているかどうか
	bool IsOpened() const;

	// 決定したボタンの処理
	virtual void Decide(int select);

	// 更新
	void Update() override;
	// デストラクタ
	void Render() override;

	// メニューの要素の有効無効を設定
	void SetMenuOnOff(int num, bool isOnOff);

protected:
	CImage* mpBackground;				// 背景画像
	std::vector<CImage*> mMenuItems;	// メニューの要素
	std::vector<bool> mMenuOnOff;		// メニューの要素が有効無効
	std::vector<CText*> mMenuTexts;		// TODO：メニューのテキスト
	CImage* mpSelectFrame;				// 選択中の画像
	int mSelectIndex;					// 選択中の番号
	bool mIsOpened;						// 開いているか
	CGameMenuBase* mpPrevMenu;			// 一つ前のメニューの格納用
	float mElapsedTime;					// 経過時間
	// 効果音
	CSound* mpSelectSE;
	CSound* mpPushSE;
	CSound* mpBuzzerSE;
};