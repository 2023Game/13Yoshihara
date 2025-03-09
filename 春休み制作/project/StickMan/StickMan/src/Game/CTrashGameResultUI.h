#pragma once
#include "CResultUIBase.h"

class CTextUI2D;

// ゴミ拾いのリザルトUIクラス
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
	// ゴミ袋の画像
	CImage* mpTrashBagImg;
	// ゴールドのゴミ袋の画像
	CImage* mpGoldTrashBagImg;
};