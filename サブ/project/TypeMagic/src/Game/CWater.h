#pragma once
#include "CObjectBase.h"
#include "CWaterShader.h"

class CMaterial;

// 水
class CWater : public CObjectBase
{
private:
	GLuint mVAO;				// 設定情報
	GLuint mVBO;				// 頂点情報
	GLuint mNormalMapTex;		// ノーマルマップのテクスチャID
	CWaterShader mWaterShader;	// 水面用シェーダー
	float mTime;				// 経過時間

	// マテリアル
	CMaterial* mpMaterial;

	// コライダー
	CCollider* mpCol;

	// コライダーを生成
	void CreateCol() override;

public:
	// コンストラクタ
	CWater(CVector scale);
	// デストラクタ
	~CWater();

	// 初期設定
	bool Init(const char* vertexPath,
		const char* flagPath);

	// 更新
	void Update() override;
	// 描画
	void Render() override;
};