#pragma once
#include "CSceneBase.h"

class CSelectMenu;

// 選択画面のシーン
class CSelectScene : public CSceneBase
{
public:
	// コンストラクタ
	CSelectScene();
	// デストラクタ
	~CSelectScene();
	// シーン読み込み
	void Load() override;
	// シーンの更新処理
	void Update() override;

private:
	CSelectMenu* mpSelectUI;
};