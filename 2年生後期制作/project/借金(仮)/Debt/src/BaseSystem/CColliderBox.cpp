#include "CColliderBox.h"
#include "glut.h"
#include "Maths.h"
#include "CColor.h"
#include "CColliderRectangle.h"

CColliderBox::CColliderBox(CObjectBase* owner, ELayer layer, 
	CVector min,CVector max,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eBox, isKinematic, weight)
{
	Set(owner, layer, min, max);
}

// ボックスコライダ―を構成する頂点の設定
void CColliderBox::Set(CObjectBase* owner, ELayer layer, 
	CVector min, CVector max)
{
	mMin = min;
	mMax = max;

	// 前面　右上
	mV[0] = CVector(mMax.X(), mMax.Y(), mMax.Z());
	// 前面　左上
	mV[1] = CVector(mMin.X(), mMax.Y(), mMax.Z());
	// 前面　左下
	mV[2] = CVector(mMin.X(), mMin.Y(), mMax.Z());
	// 前面　右下
	mV[3] = CVector(mMax.X(), mMin.Y(), mMax.Z());
	// 背面　右上
	mV[4] = CVector(mMax.X(), mMax.Y(), mMin.Z());
	// 背面　左上
	mV[5] = CVector(mMin.X(), mMax.Y(), mMin.Z());
	// 背面　左下
	mV[6] = CVector(mMin.X(), mMin.Y(), mMin.Z());
	// 背面　右下
	mV[7] = CVector(mMax.X(), mMin.Y(), mMin.Z());
}

// ボックスの頂点を取得
void CColliderBox::Get(CVector* v0, CVector* v1, CVector* v2, CVector* v3,
	CVector* v4, CVector* v5, CVector* v6, CVector* v7) const
{
	*v0 = mWV[0];
	*v1 = mWV[1];
	*v2 = mWV[2];
	*v3 = mWV[3];
	*v4 = mWV[4];
	*v5 = mWV[5];
	*v6 = mWV[6];
	*v7 = mWV[7];
}

// コライダ―描画
void CColliderBox::Render()
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// 自身の行列を適用
	glMultMatrixf(Matrix().M());

	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	CColor col = CColor::red;
	if (!IsEnable()) col = CColor::gray;
	col.A(0.2f);
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// ボックスを描画
	glBegin(GL_QUADS);
	// 前面
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// 前面　右上
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// 前面　左上
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// 前面　左下
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// 前面　右下

	// 背面
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// 背面　右上
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// 背面　右下
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// 背面　左下
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// 背面　左上

	// 左側面
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// 前面　左上
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// 背面　左上
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// 背面　左下
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// 前面　左下

	// 右側面
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// 背面　右上
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// 前面　右上
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// 前面　右下
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// 背面　右下

	// 上面
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// 背面　右上
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// 背面　左上
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// 前面　左上
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// 前面　右上

	// 底面
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// 背面　右下
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// 前面　右下
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// 前面　左下
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// 背面　左下
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

// コライダ―の情報を更新
void CColliderBox::UpdateCol()
{
	// 行列を反映した各頂点の座標を計算
	CMatrix m = Matrix();
	mWV[0] = mV[0] * m;
	mWV[1] = mV[1] * m;
	mWV[2] = mV[2] * m;
	mWV[3] = mV[3] * m;
	mWV[4] = mV[4] * m;
	mWV[5] = mV[5] * m;
	mWV[6] = mV[6] * m;
	mWV[7] = mV[7] * m;

	// バウンティングボックスを更新
	mBounds = CBounds::GetBoxBounds(
		mWV[0], mWV[1], mWV[2], mWV[3],
		mWV[4], mWV[5], mWV[6], mWV[7]);
}
