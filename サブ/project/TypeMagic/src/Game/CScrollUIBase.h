#pragma once
#include "CTask.h"

class CExpandButton;

class CScrollUIBase : public CTask
{
public:
	// コンストラクタ
	CScrollUIBase(const CVector2& pos, const CVector2& size,
		ETaskPriority prio, int sortOrder,
		ETaskPauseType pause, bool dontDelete, bool addTaskList);
	// デストラクタ
	~CScrollUIBase();

	// 更新
	void Update() override;

	// 描画
	void Render() override;

private:
	CVector2 mPosition;	// 座標
	CVector2 mSize;		// サイズ
	std::vector<CExpandButton*> mButtons;
};