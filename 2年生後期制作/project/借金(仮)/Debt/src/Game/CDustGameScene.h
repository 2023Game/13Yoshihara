#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;

//タイトルシーン
class CDustGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CDustGameScene();
	//デストラクタ
	~CDustGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CGameMenu* mpGameMenu;
};
