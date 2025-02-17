#include "CTextUI3D.h"
#include "CFont.h"
#include FT_FREETYPE_H

// 3D空間での縮尺（3D空間での1のサイズに相当するピクセル数）
#define WORLD_UNIT_PER_PIXEL 256.0f

// デフォルトサイズ
#define DEFAULT_SIZE CVector2(512.0f,64.0f)

// コンストラクタ
CTextUI3D::CTextUI3D(ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CTextUI2D(pauseType, addTaskList, fontPath)
	, mOffsetPos(0.0f, 0.0f)
	, mUV(0.0f, 0.0f, 1.0f, 1.0f)
	, mWolrdUnitPerPixel(WORLD_UNIT_PER_PIXEL)
	, mIsBillboard(false)
	, mIsDepthTest(true)
	, mIsDepthMask(false)
	, mIsLighting(false)
{
	// テクスチャ読み込み
	//mFontMaterial.LoadTexture(fontPath, fontPath, false);
	
	// デフォルトサイズを設定
	SetSize(DEFAULT_SIZE);

	SetColor(CColor::white);

	// UV設定
	SetUV(CRect(0.0f, 0.0f, 1.0f, 1.0f));
}

// デストラクタ
CTextUI3D::~CTextUI3D()
{
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

// サイズ取得
const CVector2& CTextUI3D::GetSize() const
{
	return mSize;
}

// サイズ設定
void CTextUI3D::SetSize(const float& x, const float& y)
{
	SetSize(CVector2(x, y));
}

// サイズ設定（CVector2版）
void CTextUI3D::SetSize(const CVector2& size)
{
	mSize = size;
	CVector2 s = mSize * (1.0f / mWolrdUnitPerPixel);

	// 三角形の頂点座標設定
	mT[0].Vertex
	(
		CVector(s.X(), s.Y(), 0.0f),
		CVector(-s.X(), -s.Y(), 0.0f),
		CVector(s.X(), -s.Y(), 0.0f)
	);
	mT[1].Vertex
	(
		CVector(-s.X(), s.Y(), 0.0f),
		CVector(-s.X(), -s.Y(), 0.0f),
		CVector(s.X(), s.Y(), 0.0f)
	);
	// 法線をZ軸方向
	mT[0].Normal(CVector(0.0f, 0.0f, 1.0f));
	mT[1].Normal(CVector(0.0f, 0.0f, 1.0f));
}

// UV取得
const CRect& CTextUI3D::GetUV() const
{
	return mUV;
}

// UV設定
void CTextUI3D::SetUV(const float& left, const float& top, const float& right, const float& bottom)
{
	CRect rect;
	rect.SetPos(left, top);
	rect.W(right - left);
	rect.H(bottom - top);
	SetUV(rect);
}

// UV設定（CRect版）
void CTextUI3D::SetUV(const CRect& uv)
{
	mUV = uv;

	float l = uv.X();
	float t = 1.0f - uv.Y();
	float r = l + uv.W();
	float b = t - uv.H();

	mT[0].UV
	(
		CVector(r, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, b, 0.0f)
	);
	mT[1].UV
	(
		CVector(l, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, t, 0.0f)
	);
}

// 3D空間での縮尺を設定
void CTextUI3D::SetWorldUnitPerPixel(float pixel)
{
	mWolrdUnitPerPixel = pixel;
	SetSize(mSize);
}

// ビルボードのオンオフを設定
void CTextUI3D::SetBillboard(bool enable)
{
	mIsBillboard = enable;
}

// デプステストのオンオフを設定
void CTextUI3D::SetDepthTest(bool enable)
{
	mIsDepthTest = enable;
}

// デプステストのオンオフを設定
void CTextUI3D::SetDepthMask(bool enable)
{
	mIsDepthMask = enable;
}

// ライティングのオンオフを設定
void CTextUI3D::SetLighting(bool enable)
{
	mIsLighting = enable;
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

	// 自身の行列を取得
	CMatrix m = Matrix();

	// ビルボードが有効ならば
	if (mIsBillboard)
	{
		// 常にカメラの方向を向かせる
		CCamera* cam = CCamera::CurrentCamera();
		CMatrix camMtx = cam->GetViewMatrix().Inverse();
		camMtx.Position(CVector::zero);
		m = camMtx * m;
	}

	// 行列を反映
	glMultMatrixf(m.M());

	// オフセット座標を反映
	CVector2 offset = mOffsetPos * (1.0f / mWolrdUnitPerPixel);
	glTranslatef(offset.X(), offset.Y(), 0.0f);

	// 各設定のフラグの状態に合わせて、オフにする
	if (!mIsDepthTest) glDisable(GL_DEPTH_TEST);	// デプステスト
	if (!mIsDepthMask) glDepthMask(false);		// デプス書き込み
	if (!mIsLighting) glDisable(GL_LIGHTING);		// ライティング

	CTextUI2D::Render();

	// 各設定を元に戻す
	glEnable(GL_DEPTH_TEST);	// デプステスト
	glDepthMask(true);			// デプス書き込み
	glEnable(GL_LIGHTING);		// ライティング


	// 行列を戻す
	glPopMatrix();
	//Render(&mFontMaterial);
}

// 描画（マテリアル指定版）
void CTextUI3D::Render(CMaterial* mpMaterial)
{
	// 行列の保存
	glPushMatrix();

	// 自身の行列を取得
	CMatrix m = Matrix();

	// ビルボードが有効ならば
	if (mIsBillboard)
	{
		// 常にカメラの方向を向かせる
		CCamera* cam = CCamera::CurrentCamera();
		CMatrix camMtx = cam->GetViewMatrix().Inverse();
		camMtx.Position(CVector::zero);
		m = camMtx * m;
	}

	// 行列を反映
	glMultMatrixf(m.M());

	// オフセット座標を反映
	CVector2 offset = mOffsetPos * (1.0f / mWolrdUnitPerPixel);
	glTranslatef(offset.X(), offset.Y(), 0.0f);

	// 各設定のフラグの状態に合わせて、オフにする
	if (!mIsDepthTest) glDisable(GL_DEPTH_TEST);	// デプステスト
	if (!mIsDepthMask) glDepthMask(false);		// デプス書き込み
	if (!mIsLighting) glDisable(GL_LIGHTING);		// ライティング

	// マテリアル適用
	mpMaterial->Enabled(mColor);
	//// 三角形の描画
	//mT[0].Render();
	//mT[1].Render();
	CTextUI2D::Render();
	// マテリアル解除
	mpMaterial->Disabled();

	// 各設定を元に戻す
	glEnable(GL_DEPTH_TEST);	// デプステスト
	glDepthMask(true);			// デプス書き込み
	glEnable(GL_LIGHTING);		// ライティング


	// 行列を戻す
	glPopMatrix();
}
