#pragma once
#include "CSceneBase.h"

class CGameMenu;

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
};
