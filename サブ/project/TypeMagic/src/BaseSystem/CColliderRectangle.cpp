#include "CColliderRectangle.h"
#include "glut.h"
#include "Maths.h"
#include "CColor.h"

// コンストラクタ
CColliderRectangle::CColliderRectangle(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, 
	const CVector& v2, const CVector& v3, 
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eRectangle, isKinematic, weight)
{
	// 四角形の頂点を設定
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
	mV[3] = v3;
}

// 四角形コライダーの設定
void CColliderRectangle::Set(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1,
	const CVector& v2, const CVector& v3)
{
	CCollider::Set(owner, layer);

	// 四角形の頂点を設定
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
	mV[3] = v3;
}

// 四角形の頂点を取得
void CColliderRectangle::Get(
	CVector* v0, CVector* v1,
	CVector* v2, CVector* v3) const
{
	*v0 = mWV[0];
	*v1 = mWV[1];
	*v2 = mWV[2];
	*v3 = mWV[3];
}

// コライダ―描画
void CColliderRectangle::Render()
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

	// 四角形を描画
	glBegin(GL_QUADS);
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

// コライダ―の情報を更新
void CColliderRectangle::UpdateCol()
{
	// 行列を反映した各頂点の座標を計算
	CMatrix m = Matrix();
	mWV[0] = mV[0] * m;
	mWV[1] = mV[1] * m;
	mWV[2] = mV[2] * m;
	mWV[3] = mV[3] * m;

	// バウンティングボックスを更新
	mBounds = CBounds::GetRectangleBounds(mWV[0], mWV[1], mWV[2], mWV[3]);
}
