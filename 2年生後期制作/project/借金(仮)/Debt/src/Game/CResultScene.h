#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"
class CSound;
class CResultUIBase;

// リザルトシーン
class CResultScene : public CSceneBase
{
public:
	//コンストラクタ
	CResultScene();
	//デストラクタ
	~CResultScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CResultUIBase* mpResultUI;
};