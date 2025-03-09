#pragma once
#include "CSceneBase.h"
class CMenu;
class CSound;
class CToDoUI;
class CSelectJobUI;
class CMoneyUI;

// 拠点シーン
class CHomeScene :public CSceneBase
{
public:
	//コンストラクタ
	CHomeScene();
	//デストラクタ
	~CHomeScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	// ゲームメニュー
	CMenu* mpGameMenu;
	// やること表示UI
	CToDoUI* mpToDoUI;
	// 選択中の仕事名表示UI
	CSelectJobUI* mpSelectJobUI;
	// 所持金の表示UI
	CMoneyUI* mpMoneyUI;
};
