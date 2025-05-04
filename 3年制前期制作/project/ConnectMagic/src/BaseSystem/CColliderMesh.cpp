#include "CColliderMesh.h"

CColliderMesh::CColliderMesh(CObjectBase* owner, ELayer layer, CModel* model,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eMesh, isKinematic, weight)
{
	Set(model);
}

CColliderMesh::~CColliderMesh()
{
	mVertices.clear();
}

void CColliderMesh::Set(CModel* model)
{
	mVertices.clear();
	if (model == nullptr) return;

	auto triangles = model->Triangles();
	int count = triangles.size();
	for (auto& tri : triangles)
	{
		STVertex v(tri.V0(), tri.V1(), tri.V2());
		mVertices.push_back({ v, v });
	}
}

const std::list<STVertexData>& CColliderMesh::Get() const
{
	return mVertices;
}

// コライダ―描画
void CColliderMesh::Render()
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE緑色設定
	CColor col = CColor::green;
	if (!IsEnable()) col = CColor::gray;
	col.A(0.2f);
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// 三角形を描画
	glBegin(GL_TRIANGLES);
	for (const auto& v : mVertices)
	{
		glVertex3f(v.wv.V[0].X(), v.wv.V[0].Y(), v.wv.V[0].Z());
		glVertex3f(v.wv.V[1].X(), v.wv.V[1].Y(), v.wv.V[1].Z());
		glVertex3f(v.wv.V[2].X(), v.wv.V[2].Y(), v.wv.V[2].Z());
	}
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

// コライダーの情報を更新
void CColliderMesh::UpdateCol()
{
	// 行列を反映した各頂点の座標を計算
	// 各三角形のバウンディングボックスを保存
	CMatrix m = Matrix();
	for (auto& v : mVertices)
	{
		v.wv.V[0] = v.lv.V[0] * m;
		v.wv.V[1] = v.lv.V[1] * m;
		v.wv.V[2] = v.lv.V[2] * m;
		v.bounds = CBounds::GetTriangleBounds(v.wv.V[0], v.wv.V[1], v.wv.V[2]);
	}
}
