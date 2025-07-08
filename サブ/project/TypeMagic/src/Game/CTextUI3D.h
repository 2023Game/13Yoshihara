#pragma once
#include "CTextUI2D.h"
#include "CTextUI2D.h"

class CFont;

// 3D空間に配置するテキストクラス
class CTextUI3D : public CTextUI2D
{
public:
	// コンストラクタ
	CTextUI3D(CVector pos,
		ETaskPauseType pauseType = ETaskPauseType::eGame,
		bool addTaskList = true,
		char* fontPath = "res\\Font\\toroman.ttf");
	// デストラクタ
	~CTextUI3D();

	// 更新
	void Update() override;

	// ワールド座標からスクリーン座標を求めて座標を設定
	void Position(const CVector& pos) override;

	// ワールド座標を設定
	void SetWorldPos(const CVector& pos);

protected:
	// ワールド座標
	CVector mWorldPos;
};