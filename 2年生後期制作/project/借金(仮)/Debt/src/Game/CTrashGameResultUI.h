#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CTrashGameResultUI : public CTask
{
public:
	// コンストラクタ
	CTrashGameResultUI();
	// デストラクタ
	~CTrashGameResultUI();

	// 結果画面終了か
	bool IsEnd() const;
	// リトライするか
	bool IsStartGame() const;
	// 拠点へ戻るか
	bool IsReturnHome() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 待機状態
	void UpdateIdle();
	// メニューを開く
	void UpdateOpen();
	// メニュー選択
	void UpdateSelect();
	// フェードアウト
	void UpdateFadeOut();

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

	// [RETRY]クリック時のコールバック関数
	void OnClickOption();
	// [RETURN]クリック時のコールバック関数
	void OnClickReturn();


	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// タイトル画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	CFont* mpFont;	// フォント
	CText* mpScore;	// スコア
	CImage* mpResultBg;	// 結果背景イメージ
	std::vector<CExpandButton*> mButtons;
};