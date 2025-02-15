#pragma once
#include "CTextUI2D.h"
#include "CText.h"

class CFont;

// 3D空間に配置するテキストクラス
class CTextUI3D : public CTextUI2D
{
public:
	// コンストラクタ
	CTextUI3D(ETaskPauseType pauseType = ETaskPauseType::eGame,
		bool addTaskList = true,
		char* fontPath = "res\\Font\\toroman.ttf");
	// デストラクタ
	~CTextUI3D();

	// 更新
	void Update();
	// 描画
	void Render();

	// 2D空間でのオフセット座標を取得
	const CVector2& GetOffsetPos() const;
	// 2D空間でのオフセット座標を設定
	void SetOffsetPos(const float& x, const float& y);
	// 2D空間でのオフセット座標を設定（CVector2版）
	void SetOffsetPos(const CVector2& pos);

private:
	// 2D空間でのオフセット座標
	CVector2 mOffsetPos;
};