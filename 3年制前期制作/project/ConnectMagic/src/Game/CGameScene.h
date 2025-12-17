#pragma once
#include "CSceneBase.h"

class CGameMenu;
class CRoomManager;
class CPhysicsManager;

// ゲームシーン
class CGameScene : public CSceneBase
{
public:
	// コンストラクタ
	CGameScene();
	// デストラクタ
	~CGameScene();
	// シーン読み込み
	void Load() override;
	// シーンの更新処理
	void Update() override;

private:
	// ゲームメニュー
	CGameMenu* mpGameMenu;
	// 部屋の管理クラス
	CRoomManager* mpRoomMgr;
};