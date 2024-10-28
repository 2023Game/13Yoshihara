#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;

//タイトルシーン
class CHomeScene :public CSceneBase
{
public:
	//コンストラクタ
	CHomeScene();
	//デストラクタ
	~CHomeScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CGameMenu* mpGameMenu;
};
