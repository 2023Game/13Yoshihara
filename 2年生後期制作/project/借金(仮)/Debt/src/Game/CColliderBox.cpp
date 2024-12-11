#include "CColliderBox.h"
#include "glut.h"
#include "Maths.h"
#include "CColor.h"

CColliderBox::CColliderBox(CObjectBase* owner, ELayer layer, 
	const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
	const CVector& v4, const CVector& v5, const CVector& v6, const CVector& v7,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eBox, isKinematic, weight)
{
	// ボックスの頂点を設定
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
	mV[3] = v3;
	mV[4] = v4;
	mV[5] = v5;
	mV[6] = v6;
	mV[7] = v7;
}

// ボックスコライダ―の設定
void CColliderBox::Set(CObjectBase* owner, ELayer layer, 
	const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
	const CVector& v4, const CVector& v5, const CVector& v6, const CVector& v7)
{
	CCollider::Set(owner, layer);

	// ボックスの頂点を設定
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
	mV[3] = v3;
	mV[4] = v4;
	mV[5] = v5;
	mV[6] = v6;
	mV[7] = v7;
}

// ボックスの頂点を取得
void CColliderBox::Get(
	CVector* v0, CVector* v1, CVector* v2, CVector* v3,
	CVector* v4, CVector* v5, CVector* v6, CVector* v7)
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
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// 前面左上
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// 前面左下
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// 前面右下
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// 前面右上

	// 背面
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// 背面左上
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// 背面右上
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// 背面右下
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// 背面左下

	// 左側面
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// 背面左上
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// 背面左下
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// 前面左下
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// 前面左上

	// 右側面
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// 前面右上
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// 前面右下
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// 背面右下
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// 背面右上

	// 上面
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// 背面左上
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// 前面左上
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// 前面右上
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// 背面右上

	// 底面
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// 背面左下
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// 背面右下
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// 前面右下
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// 前面左下
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
