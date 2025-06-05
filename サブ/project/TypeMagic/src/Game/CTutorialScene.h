#pragma once
#include "CSceneBase.h"

class CGameMenu;

// チュートリアルシーン
class CTutorialScene : public CSceneBase
{
public:
	// コンストラクタ
	CTutorialScene();
	// デストラクタ
	~CTutorialScene();

	// シーン読み込み
	void Load() override;
	// シーンの更新処理
	void Update() override;

private:
	// ゲームメニュー
	CGameMenu* mpGameMenu;
};