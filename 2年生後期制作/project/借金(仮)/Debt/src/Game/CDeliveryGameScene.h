#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CDeliveryEnemyManager;
class CDeliveryFieldManager;
class CTimeUI;
class CDeliveryScoreUI;
class CManualMenu;

// 配達ゲームシーン
class CDeliveryGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CDeliveryGameScene();
	//デストラクタ
	~CDeliveryGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	// ゲームメニュー
	CGameMenu* mpGameMenu;
	// 敵管理クラス
	CDeliveryEnemyManager* mpEnemyMgr;

	// フィールド管理クラス
	CDeliveryFieldManager* mpFieldMgr;
	// 時間表示UI
	CTimeUI* mpTimeUI;
	// スコア表示UI
	CDeliveryScoreUI* mpDeliveryScoreUI;
	// 経過時間を計る
	float mElapsedTime;

	// 操作説明
	CManualMenu* mpManual;
};
