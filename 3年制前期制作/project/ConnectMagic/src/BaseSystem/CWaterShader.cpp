#include  "glew.h"
#include "CWaterShader.h"

// シェーダーファイルを読み込み、プログラムを作成し、
// uniformのロケーションを取得
bool CWaterShader::Load(const char* vertexPath, const char* flagPath)
{
	// 基底クラスのLoadでシェーダープログラムを作成
	bool result = CShader::Load(vertexPath, flagPath);
	if (!result) return false;

	// シェーダー内のuniform変数の場所を取得
	mTimeLocation = glGetUniformLocation(GetProgram(), "u_Time");
	mNormalMapLocation = glGetUniformLocation(GetProgram(), "u_NormalMap");
	mCameraPosLocation = glGetUniformLocation(GetProgram(), "u_CameraPos");
	mModelLocation = glGetUniformLocation(GetProgram(), "u_Model");
	mViewLocation = glGetUniformLocation(GetProgram(), "u_View");
	mProjectionLocation = glGetUniformLocation(GetProgram(), "u_Projection");

	return true;
}

// 各種uniformに値を設定する
void CWaterShader::SetUniforms(
	float time,
	int normalMapTextureUnit,
	const CVector& cameraPos,
	const CMatrix& model,
	const CMatrix& view,
	const CMatrix& projection)
{
	// 経過時間を設定
	glUniform1f(mTimeLocation, time);

	// ノーマルマップ用のテクスチャユニットを設定
	glUniform1i(mNormalMapLocation, normalMapTextureUnit);

	// カメラのワールド座標を設定
	glUniform3fv(mCameraPosLocation, 1, (float*)&cameraPos);

	// モデル変換行列を設定
	glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, model.M());

	// ビュー変換行列を設定
	glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, view.M());

	// 投影変換行列
	glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, projection.M());
}
