#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"
class CSound;
class CTitleUI;

//タイトルシーン
class CTitleScene : public CSceneBase
{
public:
	//コンストラクタ
	CTitleScene();
	//デストラクタ
	~CTitleScene();
	//シーン読み込み
	void Load() override;
	//シーンの更新処理
	void Update() override;

private:
	CTitleUI* mpTitleUI;
};