#include "CTextUI3D.h"
#include "Maths.h"

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