#include "CTextUI3D.h"
#include "CFont.h"
#include <stdarg.h>
#include <glut.h>

// コンストラクタ
CTextUI3D::CTextUI3D(ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CTextUI2D(pauseType, addTaskList, fontPath)
	, mOffsetPos(0.0f,0.0f)
{

}

// デストラクタ
CTextUI3D::~CTextUI3D()
{
}

// 更新
void CTextUI3D::Update()
{
	CTextUI2D::Update();
}

// 描画
void CTextUI3D::Render()
{
	// 行列の保存
	glPushMatrix();

	// オフセット座標を反映
	CVector2 offset = mOffsetPos;
	glTranslatef(offset.X(), offset.Y(), 0.0f);

	CTextUI2D::Render();

	// 行列を戻す
	glPopMatrix();
}

// 2D空間でのオフセット座標を取得
const CVector2& CTextUI3D::GetOffsetPos() const
{
	return mOffsetPos;
}

// 2D空間でのオフセット座標を設定
void CTextUI3D::SetOffsetPos(const float& x, const float& y)
{
	SetOffsetPos(CVector2(x, y));
}

// 2D空間でのオフセット座標を設定（CVector2版）
void CTextUI3D::SetOffsetPos(const CVector2& pos)
{
	mOffsetPos = pos;
}
