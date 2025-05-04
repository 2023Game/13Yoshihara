#pragma once
#include "CTask.h"

class CImage;
class CExpandButton;
class CTextUI2D;

class CEndUIBase : public CTask
{
public:
	// コンストラクタ
	CEndUIBase(std::string bgPath);
	// デストラクタ
	~CEndUIBase();

	// 画面終了か
	bool IsEnd() const;

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

	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// 画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	CTextUI2D* mpText;	// テキスト
	CImage* mpBg;		// 背景イメージ
	std::vector<CExpandButton*> mButtons;
};