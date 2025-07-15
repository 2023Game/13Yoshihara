#pragma once
#include "CSceneBase.h"

class CGameMenu;

// チュートリアルシーン
class CMap2Scene : public CSceneBase
{
public:
	// コンストラクタ
	CMap2Scene();
	// デストラクタ
	~CMap2Scene();

	// シーン読み込み
	void Load() override;
	// シーンの更新処理
	void Update() override;

private:
	// ゲームメニュー
	CGameMenu* mpGameMenu;
};