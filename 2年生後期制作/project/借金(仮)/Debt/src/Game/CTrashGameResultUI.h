#pragma once
#include "CResultUIBase.h"

class CTextUI2D;

class CTrashGameResultUI : public CResultUIBase
{
public:
	// コンストラクタ
	CTrashGameResultUI();
	// デストラクタ
	~CTrashGameResultUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	// 得点のテキスト
	CTextUI2D* mpScorePointText;
	// ゴミ袋の数のテキスト
	CTextUI2D* mpNumText;
	// ゴミ袋の値段のテキスト
	CTextUI2D* mpPriceText;
	// 単位のテキスト
	CTextUI2D* mpUnitText;
	// ×のテキスト
	CTextUI2D* mpXText;

	// ゴミ袋の画像
	CImage* mpTrashBagImg;
	// ゴールドのゴミ袋の画像
	CImage* mpGoldTrashBagImg;
};