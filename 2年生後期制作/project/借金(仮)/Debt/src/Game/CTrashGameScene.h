#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CTrashVehicleManager;
class CResidentManager;
class CTrashEnemyManager;
class CTimeUI;
class CTrashScoreUI;
class CManualMenu;
class CTrashWarningUI;

// ゴミ拾いゲームシーン
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
	CGameMenu* mpGameMenu;
	// 車両管理クラス
	CTrashVehicleManager* mpVehicleMgr;
	// 住人管理クラス
	CResidentManager* mpResidentMgr;
	// ゴミ拾いの敵管理クラス
	CTrashEnemyManager* mpTrashEnemyMgr;
	// 時間表示UI
	CTimeUI* mpTimeUI;
	// スコア表示UI
	CTrashScoreUI* mpTrashScoreUI;
	// 警告UI
	CTrashWarningUI* mpWarningUI;
	// 経過時間を計る
	float mElapsedTime;

	// 操作説明
	CManualMenu* mpManual;
};
