#pragma once
#include "CEndUIBase.h"

class CGameOverUI : public CEndUIBase
{
public:
	// コンストラクタ
	CGameOverUI();
	// デストラクタ
	~CGameOverUI();

	// 前日へ戻るか
	bool IsPreDay() const;
	// ゲームを終了するか
	bool IsExitGame() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// [前日へ]クリック時のコールバック関数
	void OnClickPreDay();
	// [終了]クリック時のコールバック関数
	void OnClickQuit();
};