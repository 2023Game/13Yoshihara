#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CDeliveryFieldManager;
class CTimeUI;
class CDeliveryScoreUI;

//タイトルシーン
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
	CGameMenuBase* mpGameMenu;

	// フィールド管理クラス
	CDeliveryFieldManager* mpFieldMgr;
	// 時間表示UI
	CTimeUI* mpTimeUI;
	// スコア表示UI
	CDeliveryScoreUI* mpDeliveryScoreUI;
	// 経過時間を計る
	float mElapsedTime;
};
