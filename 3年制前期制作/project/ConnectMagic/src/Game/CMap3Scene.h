#pragma once
#pragma once
#include "CSceneBase.h"

class CGameMenu;

class CFieldBase;

// マップ3
class CMap3Scene : public CSceneBase
{
public:
	// コンストラクタ
	CMap3Scene();
	// デストラクタ
	~CMap3Scene();
	// シーン読み込み
	void Load() override;
	// シーンの更新処理
	void Update() override;

private:
	// ゲームメニュー
	CGameMenu* mpGameMenu;

	CFieldBase* mpField;
};