#pragma once
#include "CObjectBase.h"
#include "CWaterShader.h"

class CTexture;
class CMaterial;

// 水
class CWater : public CObjectBase
{
private:
	GLuint mVAO;
	GLuint mVBO;
	GLuint mNormalMapTex;
	CWaterShader mWaterShader;	// 水面用シェーダー
	float mTime;				// 経過時間

	CMaterial* mpMaterial;

public:
	// コンストラクタ
	CWater();
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