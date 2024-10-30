#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
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
	CGameMenuBase* mpGameMenu;
};
