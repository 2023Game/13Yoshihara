#ifndef CMATERIAL_H
#define CMATERIAL_H
#define MATERIAL_NAME_LEN 64//名前の長さ

#include "CTexture.h"
#include "CMyShader.h"
class CModelX;
/*
マテリアルクラス
マテリアルのデータを扱う
*/
class CMaterial
{
	friend CMyShader;
public:
	//頂点数の設定
	//VertexNum(頂点数)
	void VertexNum(int num);
	//頂点数の取得
	int VertexNum();
	//デフォルトコンストラクタ
	CMaterial();
	CMaterial(CModelX* model);
	~CMaterial();
	//マテリアルを有効にする
	void Enabled();
	//マテリアルの名前の取得
	char* Name();
	//マテリアルの名前を設定する
	//Name(マテリアルの名前)
	void Name(char* name);
	//mDiffuse配列の取得
	float* Diffuse();
	//マテリアルを無効にする
	void Disabled();
	//テクスチャの取得
	CTexture* Texture();
private:
	//マテリアル毎の頂点数
	int mVertexNum;
	//マテリアル名
	char mName[MATERIAL_NAME_LEN + 1];
	//拡散光の色RGBA
	float mDiffuse[4];
	//テクスチャ
	CTexture mTexture;
	float mPower;
	float mSpecular[3];
	float mEmissive[3];
	//テクスチャファイル名
	char* mpTextureFilename;
};

#endif 