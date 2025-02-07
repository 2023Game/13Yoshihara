#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CTimeUI;
class CTrashScoreUI;

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

	// 時間表示UI
	CTimeUI* mpTimeUI;
	// スコア表示UI
	CTrashScoreUI* mpTrashScoreUI;
	// 経過時間を計る
	float mElapsedTime;
};
