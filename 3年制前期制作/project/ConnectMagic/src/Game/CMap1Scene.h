#pragma once
#include "CSceneBase.h"

class CGameMenu;

// マップ１
class CMap1Scene : public CSceneBase
{
public:
	// コンストラクタ
	CMap1Scene();
	// デストラクタ
	~CMap1Scene();
	// シーン読み込み
	void Load() override;
	// シーンの更新処理
	void Update() override;

private:
	// ゲームメニュー
	CGameMenu* mpGameMenu;
};