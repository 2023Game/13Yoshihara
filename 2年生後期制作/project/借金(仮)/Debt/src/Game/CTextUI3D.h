#pragma once
#include "CTextUI2D.h"
#include "CVector.h"
#include "CTexture.h"
#include "CText.h"
#include "CTriangle.h"
#include "CMaterial.h"

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

	// 2D空間でのオフセット座標を取得
	const CVector2& GetOffsetPos() const;
	// 2D空間でのオフセット座標を設定
	void SetOffsetPos(const float& x, const float& y);
	// 2D空間でのオフセット座標を設定（CVector2版）
	void SetOffsetPos(const CVector2& pos);

	// サイズ取得
	const CVector2& GetSize() const;
	// サイズ設定
	void SetSize(const float& x, const float& y);
	// サイズ設定（CVector2版）
	void SetSize(const CVector2& size);

	// UV取得
	const CRect& GetUV() const;
	// UV設定
	void SetUV(const float& left, const float& top, const float& right, const float& bottom);
	// UV設定（CRect版）
	void SetUV(const CRect& uv);

	// 3D空間での縮尺を設定
	void SetWorldUnitPerPixel(float pixel);

	// ビルボードのオンオフを設定
	void SetBillboard(bool enable);
	// デプステストのオンオフを設定
	void SetDepthTest(bool enable);
	// デプステストのオンオフを設定
	void SetDepthMask(bool enable);
	// ライティングのオンオフを設定
	void SetLighting(bool enable);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 描画（マテリアル指定版）
	void Render(CMaterial* mpMaterial);

	// 三角形2つ
	CTriangle mT[2];
	// マテリアル
	CMaterial mFontMaterial;
	// 2D空間でのオフセット座標
	CVector2 mOffsetPos;
	// サイズ記憶用
	CVector2 mSize;
	// テクスチャのUV
	CRect mUV;
	// 3D空間での縮尺
	float mWolrdUnitPerPixel;

	bool mIsBillboard;
	// デプステストが有効かどうか
	bool mIsDepthTest;
	// デプス書き込みが有効かどうか
	bool mIsDepthMask;
	// ライティングが有効かどうか
	bool mIsLighting;
};