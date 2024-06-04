#ifndef CMODELX_H //インクルードガード
#define CMODELX_H

#include <vector>    //vectorクラスのインクルード（動的配列）
#include "CMatrix.h" //マトリクスクラスのインクルード
#include "CVector.h" //Vectorクラスのインクルード
class CModelX;      //CModelXクラスの宣言
class CModelXFrame; //CModelXFrameクラスの宣言
class CMesh;        //CMeshクラスの宣言
class CMaterial;    //CMaterialクラスの宣言
class CSkinWeights; //CSkinWeightsクラスの宣言
class CAnimationSet;//CAnimationSetクラスの宣言
class CAnimation;   //CAnimationクラスの宣言
class CAnimationKey;//CAnimationKeyクラスの宣言

#define MODEL_FILE "res\\sample.blend.x"

//領域解放をマクロ化
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a; a = nullptr;}
//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))
/*
CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX {
	friend CModelXFrame;
	friend CAnimationSet;
	friend CAnimation;
public:
	CModelX();
	~CModelX();
	void Render();
	//ノードの読み飛ばし
	void SkipNode();
	//単語の取り出し
	char* GetToken();
	//ファイル読み込み
	void Load(char* file);
	//mTokenのポインタを返す
	char* Token();
	//トークンが無くなったらtrue
	bool EOT();
	//フレーム名に該当するフレームのアドレスを返す
	std::vector<CAnimationSet*>& AnimationSet();
	CModelXFrame* FindFrame(char* name);
	//フレームの変換行列を更新する
	void AnimateFrame();
private:
	std::vector<CModelXFrame*> mFrame;//フレームの配列
	std::vector<CAnimationSet*> mAnimationSet;//アニメーションセットの配列
	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
	char* mpPointer;   //読み込み位置
	char mToken[1024]; //取り出した単語の領域
};

//Frameクラス
class CModelXFrame {
	friend CModelX;
	friend CAnimationSet;
	friend CAnimation;
public:
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame();
	//描画
	void Render();
	//mIndexを返す
	int Index();
private:
	std::vector<CModelXFrame*> mChild;//子フレームの配列
	CMatrix mTransformMatrix;//変換行列
	char* mpName;//フレーム名前
	int mIndex; //フレーム番号
	CMesh* mpMesh; //Meshデータ
};

//CMeshクラスの定義
class CMesh {
public:
	//コンストラクタ
	CMesh();
	//デストラクタ
	~CMesh();
	//描画
	void Render();
	//読み込み処理
	void Init(CModelX* model);
private:
	int mVertexNum;   //頂点数
	CVector* mpVertex;//頂点データ
	int mFaceNum; //面数
	int* mpVertexIndex; //面を構成する頂点インデックス
	int mNormalNum; //法線数
	CVector* mpNormal; //法線ベクトル
	int mMaterialNum;//マテリアル数
	int mMaterialIndexNum;//マテリアル番号数（面数）
	int* mpMaterialIndex;//マテリアル番号
	std::vector<CMaterial*> mMaterial;//マテリアルデータ
	std::vector<CSkinWeights*> mSkinWeights;//スキンウェイト
};

//スキンウェイトクラス
class CSkinWeights {
	friend CModelX;
	friend CMesh;
public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
private:
	char* mpFrameName;//フレーム名
	int mFrameIndex; //フレーム番号
	int mIndexNum;//頂点番号数
	int* mpIndex; //頂点番号配列
	float* mpWeight;//頂点ウェイト配列
	CMatrix mOffset;//オフセットマトリックス
};

//アニメーションセット
class CAnimationSet {
	friend CModelX;
public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();
	void Time(float time);//時間の設定
	void Weight(float weight);//重みの設定
	void AnimateMatrix(CModelX* model);
	std::vector<CAnimation*>& Animation();
private:
	//アニメーションセット名
	char* mpName;
	//アニメーション
	std::vector<CAnimation*>mAnimation;
	float mTime; //現在時間
	float mWeight; //重み
	float mMaxTime; //最大時間
};

//アニメーションクラス
class CAnimation {
	friend CModelX;
	friend CAnimationSet;
public:
	CAnimation(CModelX* model);
	~CAnimation();
private:
	char* mpFrameName;//フレーム名
	int mFrameIndex;//フレーム番号
	int mKeyNum; //キー数（時間数）
	CAnimationKey* mpKey;//キーの配列
};

//アニメーションキークラス
class CAnimationKey {
	friend CAnimation;
	friend CAnimationSet;
private:
	//時間
	float mTime;
	//行列
	CMatrix mMatrix;
};
#endif

