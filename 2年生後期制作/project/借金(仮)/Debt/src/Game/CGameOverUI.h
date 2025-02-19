#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CGameOverUI : public CTask
{
public:
	// コンストラクタ
	CGameOverUI();
	// デストラクタ
	~CGameOverUI();

	// ゲームオーバー画面終了か
	bool IsEnd() const;
	// 前日へ戻るか
	bool IsPreDay() const;
	// ゲームを終了するか
	bool IsExitGame() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// メニューを開く
	void UpdateOpen();
	// メニュー選択
	void UpdateSelect();
	// フェードアウト
	void UpdateFadeOut();

	// タイトルの状態
	enum class EState
	{
		eOpen,		// メニューを開く
		eSelect,	// メニュー選択
		eFadeOut,	// フェードアウト
	};
	// 状態切り替え
	void ChangeState(EState state);

	// [前日へ]クリック時のコールバック関数
	void OnClickPreDay();
	// [終了]クリック時のコールバック関数
	void OnClickQuit();

	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// タイトル画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	CFont* mpFont;	// フォント
	CText* mpGameOverText;	// ゲームオーバーテキス
	CImage* mpGameOverBg;	// ゲームオーバー背景イメージ
	std::vector<CExpandButton*> mButtons;
};