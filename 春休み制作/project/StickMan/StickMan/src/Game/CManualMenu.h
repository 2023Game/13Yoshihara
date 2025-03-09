#pragma once
#include "CGameMenuBase.h"

class CTextUI2D;

class CManualMenu : public CGameMenuBase
{
public:
	// コンストラクタ
	CManualMenu(std::string bgPath);
	// デストラクタ
	~CManualMenu();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// メニューを開く
	void UpdateOpen() override;

	CTextUI2D* mpCloseText;	// 「クリックで閉じる」のテキスト
};