#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;
class CTextUI2D;

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

	// スコアを取得
	int GetScore() const;
	// スコアを設定
	void SetScore(int score);

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

	// スコアという文字のテキスト
	CTextUI2D* mpScoreText;
	// 得点のテキスト
	CTextUI2D* mpScorePointText;
	// 数値のテキスト1
	CTextUI2D* mpNumText1;
	// 数値のテキスト2
	CTextUI2D* mpNumText2;
	// 計算結果のテキスト
	CTextUI2D* mpTotalText;
	// 単位のテキスト
	CTextUI2D* mpUnitText;
	// 記号のテキスト
	CTextUI2D* mpSymbolText;
	// 一番下の計算の説明テキスト
	CTextUI2D* mpDescriptionText;
	// ボーナスの説明テキスト
	CTextUI2D* mpBonusDescriptionText;
	// リザルト背景イメージ
	CImage* mpResultBg;
	// 拠点へ戻るボタン
	CExpandButton* mpReturnButton;

	// 得点
	int mScore;
};