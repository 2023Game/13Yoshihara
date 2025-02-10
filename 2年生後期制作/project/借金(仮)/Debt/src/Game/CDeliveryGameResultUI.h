#pragma once
#include "CResultUIBase.h"

class CDeliveryGameResultUI : public CResultUIBase
{
public:
	// コンストラクタ
	CDeliveryGameResultUI();
	// デストラクタ
	~CDeliveryGameResultUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
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

	// 配達物の画像
	CImage* mpDeliveryItemImg;
};