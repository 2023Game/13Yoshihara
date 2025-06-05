#pragma once
#include "CEndUIBase.h"

class CFont;
class CImage;
class CExpandButton;

class CGameClearUI : public CEndUIBase
{
public:
	// コンストラクタ
	CGameClearUI();
	// デストラクタ
	~CGameClearUI();

	// ゲームを終了するか
	bool IsExitGame() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// [終了]クリック時のコールバック関数
	void OnClickQuit();
};