#pragma once
#include "CEndUIBase.h"

class CTextUI2D;

class CResultUIBase : public CEndUIBase
{
public:
	// コンストラクタ
	CResultUIBase(std::string backGroundPath);
	// デストラクタ
	~CResultUIBase();

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

	// [拠点へ]クリック時のコールバック関数
	void OnClickReturn();

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

	// 得点
	int mScore;
};