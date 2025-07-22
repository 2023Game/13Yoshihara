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
	Set(min, max);
}

CColliderBox::~CColliderBox()
{
	mVertices.clear();
}

// ボックスコライダ―を構成する頂点の設定
void CColliderBox::Set(CVector min, CVector max)
{
	mVertices.clear();
	// 前面　右上
	CVector v0 = CVector(max.X(), max.Y(), max.Z());
	// 前面　左上
	CVector v1 = CVector(min.X(), max.Y(), max.Z());
	// 前面　左下
	CVector v2 = CVector(min.X(), min.Y(), max.Z());
	// 前面　右下
	CVector v3 = CVector(max.X(), min.Y(), max.Z());
	// 背面　右上
	CVector v4 = CVector(max.X(), max.Y(), min.Z());
	// 背面　左上
	CVector v5 = CVector(min.X(), max.Y(), min.Z());
	// 背面　左下
	CVector v6 = CVector(min.X(), min.Y(), min.Z());
	// 背面　右下
	CVector v7 = CVector(max.X(), min.Y(), min.Z());

	// 前面
	SRVertex rv0(v0, v1, v2, v3);
	mVertices.push_back({ rv0,rv0 });
	// 背面
	SRVertex rv1(v4, v7, v6, v5);
	mVertices.push_back({ rv1,rv1 });
	// 左面
	SRVertex rv2(v1, v5, v6, v2);
	mVertices.push_back({ rv2,rv2 });
	// 右面
	SRVertex rv3(v4, v0, v3, v7);
	mVertices.push_back({ rv3,rv3 });
	// 上面
	SRVertex rv4(v4, v5, v1, v0);
	mVertices.push_back({ rv4,rv4 });
	// 下面
	SRVertex rv5(v7, v3, v2, v6);
	mVertices.push_back({ rv5,rv5 });
}

const std::vector<SRVertexData>& CColliderBox::Get() const
{
	return mVertices;
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

	// 四角形を描画
	glBegin(GL_QUADS);
	for (const auto& v : mVertices)
	{
		glVertex3f(v.wv.V[0].X(), v.wv.V[0].Y(), v.wv.V[0].Z());
		glVertex3f(v.wv.V[1].X(), v.wv.V[1].Y(), v.wv.V[1].Z());
		glVertex3f(v.wv.V[2].X(), v.wv.V[2].Y(), v.wv.V[2].Z());
		glVertex3f(v.wv.V[3].X(), v.wv.V[3].Y(), v.wv.V[3].Z());
	}
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

// コライダ―の情報を更新
void CColliderBox::UpdateCol(bool isInit)
{
	// 前回の更新と同じ行列であれば、処理しない
	CMatrix m = Matrix();
	if (!isInit && m == mLastMtx) return;
	mLastMtx = m;

	// 行列を反映した各頂点の座標を計算
	// 各四角形のバウンディングボックスを保存
	for (auto& v : mVertices)
	{
		v.wv.V[0] = v.lv.V[0] * m;
		v.wv.V[1] = v.lv.V[1] * m;
		v.wv.V[2] = v.lv.V[2] * m;
		v.wv.V[3] = v.lv.V[3] * m;
		v.bounds = CBounds::GetRectangleBounds(v.wv.V[0], v.wv.V[1], v.wv.V[2], v.wv.V[3]);
	}
}
