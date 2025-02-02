#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CVehicleManager;
class CResidentManager;
class CSound;
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
	CVehicleManager* mpVehicleManager;
	// 住人管理クラス
	CResidentManager* mpResidentManager;
	// 時間表示UI
	CTimeUI* mpTimeUI;
	// スコア表示UI
	CTrashScoreUI* mpTrashScoreUI;
	// 経過時間を計る
	float mElapsedTime;
	// ポーズしたかどうか
	bool mIsPause;
};
