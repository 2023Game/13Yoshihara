#pragma once
#include "CObjectBase.h"

class CTextUI2D;
class CImage;

// 今日の返済額を表示する
class CDebtMoneyUI : public CObjectBase
{
public:
	// コンストラクタ
	CDebtMoneyUI();
	// デストラクタ
	~CDebtMoneyUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 待機
	void UpdateIdle();
	// 所持金の減少
	void UpdateDecrease();
	// フェードアウト
	void UpdateFadeOut();
	// ゲームオーバー
	void UpdateGameOver();

	// 状態
	enum class EState
	{
		eIdle,			// 待機
		eDecrease,		// 所持金の減少
		eFadeOut,		// フェードアウト
		eGameOver,		// ゲームオーバー
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// プレイヤーの状態
	int mStateStep;				// 状態内のステップ管理用
	// 経過時間
	float mElapsedTime;

	// 背景
	CImage* mpBackGround;

	// 日数
	CTextUI2D* mpDayText;
	// 返済額
	CTextUI2D* mpDebtMoneyText;
	// 所持金
	CTextUI2D* mpMoneyText;
	// 「クリックで次へ」のテキスト
	CTextUI2D* mpNextText;

	// 所持金を減らす
	void DecreaseMoney();

	// 返済額の値
	int mDebtMoneyAmount;
	// 所持金の値
	int mMoneyAmount;
	// フォントのサイズ
	int mFontSize;

	// 結果
	int mResultAmount;
};