#include "CDeliveryGameResultUI.h"

// 背景画像のパス
#define BACKGROUND_PATH "UI/trash_game_result_bg.png"

// コンストラクタ
CDeliveryGameResultUI::CDeliveryGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
}

// デストラクタ
CDeliveryGameResultUI::~CDeliveryGameResultUI()
{
}

// 更新
void CDeliveryGameResultUI::Update()
{
	CResultUIBase::Update();
}

// 描画
void CDeliveryGameResultUI::Render()
{
	CResultUIBase::Render();
}
