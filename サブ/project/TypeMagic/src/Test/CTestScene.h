#pragma once
#include "CSceneBase.h"

class CGameMenu;
class CEnemyManager;

// テストシーン
class CTestScene :public CSceneBase
{
public:
	//コンストラクタ
	CTestScene();
	//デストラクタ
	~CTestScene();
	//シーン読み込み
	void Load() override;
	//シーンの更新処理
	void Update() override;

private:
	// ゲームメニュー
	CGameMenu* mpGameMenu;
	// 敵の管理クラス
	CEnemyManager* mpEnemyMgr;
};
