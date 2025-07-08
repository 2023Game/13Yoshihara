#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;
class CScrollUIBase;

class CSelectMenu : public CTask
{
public:
	// コンストラクタ
	CSelectMenu();
	// デストラクタ
	~CSelectMenu();

	// セレクト画面が終了か
	bool IsEnd() const;
	// 対戦を開始するか
	bool IsBattleStart() const;
	// 一つ前に戻るか
	bool IsReturn() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// メニューを開く
	void UpdateOpen();
	// メニューを選択
	void UpdateSelect();
	// フェードアウト
	void UpdateFadeOut();

	// 状態
	enum class EState
	{
		eOpen,		// メニューを開く
		eSelect,	// メニュー選択
		eFadeOut,	// フェードアウト
	};
	// 状態切り替え
	void ChangeState(EState state);

	// [BattleStart]クリック時のコールバック関数
	void OnClickBattleStart();
	// [Return]クリック時のコールバック関数
	void OnClickReturn();
	// [SelectElement]クリック時のコールバック関数
	void OnClickSelectElement();

	EState mState;		// 現在の状態
	int mStateStep;		// 状態内のステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// 画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	CFont* mpFont;	// フォント
	CImage* mpBg;	// 背景イメージ
	std::vector<CExpandButton*> mButtons;

	// スクロールできるUI
	CScrollUIBase* mpScrollUI;

	CImage* mpPlayerImg;	// プレイヤーのアイコン画像
};