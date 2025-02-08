#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CVehicleManager;
class CResidentManager;
class CTrashEnemyManager;
class CTimeUI;
class CTrashScoreUI;

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
	// ゲームメニュー
	CGameMenuBase* mpGameMenu;
	// 車両管理クラス
	CVehicleManager* mpVehicleMgr;
	// 住人管理クラス
	CResidentManager* mpResidentMgr;
	// ゴミ拾いの敵管理クラス
	CTrashEnemyManager* mpTrashEnemyMgr;
	// 時間表示UI
	CTimeUI* mpTimeUI;
	// スコア表示UI
	CTrashScoreUI* mpTrashScoreUI;
	// 経過時間を計る
	float mElapsedTime;
};
