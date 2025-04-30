#pragma once
#include "CSceneBase.h"

class CGameMenu;
class CConnectPointManager;

// テストシーン
class CTestScene :public CSceneBase
{
public:
	//コンストラクタ
	CTestScene();
	//デストラクタ
	~CTestScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	// ゲームメニュー
	CGameMenu* mpGameMenu;

	// 接続部の管理クラス
	CConnectPointManager* mpConnectPointMgr;
};
