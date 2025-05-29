#pragma once
#include "CShader.h"

// 水面専用シェーダークラス
class CWaterShader : public CShader
{
private:
	GLint mTimeLocation;		// 経過時間
	GLint mNormalMapLocation;	// ノーマルマップのテクスチャユニット
	GLint mCameraPosLocation;	// カメラのワールド座標
	GLint mModelLocation;		// モデル変換行列
	GLint mViewLocation;		// ビュー変換行列
	GLint mProjectionLocation;	// 投影変換行列

public:

	// シェーダーファイルを読み込み、プログラムを作成し、
	// uniformのロケーションを取得
	bool Load(const char* vertexPath, const char* flagPath);

	// 各種uniformに値を設定する
	void SetUniforms(
		float time,
		int normalMapTextureUnit,
		const CVector& cameraPos,
		const CMatrix& model,
		const CMatrix& view,
		const CMatrix& projection);
};