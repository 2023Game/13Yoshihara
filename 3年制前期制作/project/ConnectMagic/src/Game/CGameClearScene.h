#pragma once
#include "CSceneBase.h"
#include "CInput.h"

class CSound;
class CGameClearUI;

// ゲームクリアシーン
class CGameClearScene : public CSceneBase
{
public:
	// コンストラクタ
	CGameClearScene();
	// デストラクタ
	~CGameClearScene();
	// シーン読み込み
	void Load();
	// シーンの更新処理
	void Update();

private:
	// ゲームクリアUI
	CGameClearUI* mpGameClearUI;
};