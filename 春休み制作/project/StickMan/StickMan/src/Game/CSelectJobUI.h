#pragma once
#include "CHomeUIBase.h"

class CTextUI2D;

// 選択中の仕事を表示するUI
class CSelectJobUI : CHomeUIBase
{
public:
	// コンストラクタ
	CSelectJobUI();
	// デストラクタ
	~CSelectJobUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
};