#include "CTextUI3D.h"
#include "Maths.h"

// テキストの最小フォントサイズ
#define TEXT_FONT_SIZE_MIN 8
// テキストの最大フォントサイズ
#define TEXT_FONT_SIZE_MAX 32

// テキストサイズが最小になる距離
#define TEXT_FONT_SIZE_MIN_DIST 500.0f
// テキストサイズが最大になる距離
#define TEXT_FONT_SIZE_MAX_DIST 50.0f

// コンストラクタ
CTextUI3D::CTextUI3D(CVector pos, ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CTextUI2D(pauseType, addTaskList, fontPath)
	, mWorldPos(pos)
{
}

// デストラクタ
CTextUI3D::~CTextUI3D()
{
}

// 更新
void CTextUI3D::Update()
{
	CCamera* camera = CCamera::CurrentCamera();
	// 距離
	float dist = CVector::Distance(mWorldPos, camera->Position());
	// 距離によってサイズを求める
	float size = Math::Lerp(TEXT_FONT_SIZE_MAX, TEXT_FONT_SIZE_MIN, (dist - TEXT_FONT_SIZE_MAX_DIST) / (TEXT_FONT_SIZE_MIN_DIST - TEXT_FONT_SIZE_MAX_DIST));
	// フォントサイズを設定
	SetFontSize(size);

	// 座標を更新
	Position(mWorldPos);

	CTextUI2D::Update();
}

// ワールド座標からスクリーン座標を求めて座標を設定
void CTextUI3D::Position(const CVector& pos)
{
	// カメラ取得
	CCamera* camera = CCamera::CurrentCamera();
	// スクリーン座標に変換
	CVector screenPos = camera->WorldToScreenPos(pos);
	// 座標を設定
	CTransform::Position(screenPos);
}

// ワールド座標を設定
void CTextUI3D::SetWorldPos(const CVector& pos)
{
	mWorldPos = pos;
}
