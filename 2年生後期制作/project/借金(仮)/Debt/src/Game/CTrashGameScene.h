#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CCarAndTruckManager;
class CSound;

//タイトルシーン
class CTrashGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CTrashGameScene();
	//デストラクタ
	~CTrashGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CGameMenuBase* mpGameMenu;
	CCarAndTruckManager* mpCCarAndTruckManager;
};
