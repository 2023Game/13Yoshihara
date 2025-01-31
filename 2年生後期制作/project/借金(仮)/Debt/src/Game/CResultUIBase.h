#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CResultUIBase : public CTask
{
public:
	// コンストラクタ
	CResultUIBase(std::string backGroundPath);
	// デストラクタ
	~CResultUIBase();

	// 結果画面終了か
	bool IsEnd() const;
	// 拠点へ戻るか
	bool IsReturnHome() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

protected:
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

	// [拠点へ]クリック時のコールバック関数
	virtual void OnClickReturn();


	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// タイトル画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	CFont* mpScoreFont;				// スコアのフォント
	CText* mpScoreText;				// スコアという文字のテキスト
	CImage* mpResultBg;				// リザルト背景イメージ
	CExpandButton* mpReturnButton;	// 拠点へ戻るボタン
};