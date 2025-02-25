#pragma once
#include "CTask.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include <vector>
#include "CSound.h"
#include "CExpandButton.h"

// ゲームメニューの基底クラス
class CGameMenuBase : public CTask
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="menuItemPathList">メニューのアイテム画像のパスリスト</param>
	CGameMenuBase(std::vector<std::string> menuItemPathList);
	// デストラクタ
	~CGameMenuBase();

	// メニューを開く
	void Open();
	// メニューを閉じる
	void Close();
	// 開いているかどうか
	bool IsOpened() const;

	// 更新
	void Update() override;
	// デストラクタ
	void Render() override;

	// メニューの要素の有効無効を設定
	void SetMenuOnOff(int num, bool isOnOff);

	// 一つ前のメニューを設定
	void SetPreMenu(CGameMenuBase* preMenu);
	
	// 有効無効の切り替え
	void SetOnOff(bool isOnOff);

protected:
	// 待機
	void UpdateIdle();
	// メニューを開く
	virtual void UpdateOpen();
	// メニュー選択
	void UpdateSelect();

	// タイトルの状態
	enum class EState
	{
		eIdle,		// 待機状態
		eOpen,		// メニューを開く
		eSelect,	// メニュー選択
		eFadeOut,	// フェードアウト
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用

	// [CLOSE]クリック時のコールバック関数
	void OnClickClose();

	CImage* mpBackground;					// 背景画像
	std::vector<CExpandButton*> mButtons;	// メニューの要素
	std::vector<bool> mMenuOnOff;			// メニューの要素が有効無効
	std::vector<CTextUI2D*> mMenuTexts;		// メニューの説明用テキスト
	bool mIsOpened;						// 開いているか
	CGameMenuBase* mpPreMenu;			// 一つ前のメニューの格納用
	bool mIsNextMenu;	// 次のメニューがあるか
	// 効果音
	CSound* mpSelectSE;
	CSound* mpPushSE;
	CSound* mpBuzzerSE;
};