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

#define MODEL_FILE "res\\ラグナ.x" //入力ファイル名
#define KNIGHT_MODEL "res\\knight\\knight_low.X"

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
	//mFrameの参照を返す
	std::vector<CModelXFrame*>& Frames();
	CModelXFrame* FindFrame(char* name);
	//フレームの変換行列を更新する
	void AnimateFrame();
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();
	//頂点にアニメーションを適用
	void AnimateVertex();
	//指定した合成行列を使って計算
	void AnimateVertex(CMatrix*);
	//マテリアルの検索
	CMaterial* FindMaterial(char* name);
	//マテリアル配列の取得
	std::vector<CMaterial*>& Material();
	/// <summary>
	/// アニメーションを抜き出す
	/// </summary>
	/// <param name="idx">分割したいアニメーションセットの番号</param>
	/// <param name="start">分割したいアニメーションの開始時間</param>
	/// <param name="end">分割したいアニメーションの終了時間</param>
	/// <param name="name">追加するアニメーションセットの名前</param>
	void SeparateAnimationSet(int idx, int start, int end, char* name);
	//読み込み済みか判定
	bool IsLoaded();
	//アニメーションセットの追加
	void AddAnimationSet(const char* file);
private:
	std::vector<CModelXFrame*> mFrame;//フレームの配列
	std::vector<CAnimationSet*> mAnimationSet;//アニメーションセットの配列
	std::vector<CMaterial*> mMaterial;//マテリアル配列
	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
	char* mpPointer;   //読み込み位置
	char mToken[1024]; //取り出した単語の領域
	bool mLoaded;	//読み込み済みかどうか
};

//Frameクラス
class CModelXFrame {
	friend CModelX;
	friend CAnimationSet;
	friend CAnimation;
	friend CAnimationKey;
public:
	CModelXFrame();
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame();
	//描画
	void Render();
	//mIndexを返す
	int Index();
	//合成行列の作成
	void AnimateCombined(CMatrix* parent);
	//mCombinedMatrixを返す
	const CMatrix& CombinedMatrix();
private:
	std::vector<CModelXFrame*> mChild;//子フレームの配列
	CMatrix mTransformMatrix;//変換行列
	char* mpName;//フレーム名前
	int mIndex; //フレーム番号
	CMesh* mpMesh; //Meshデータ
	CMatrix mCombinedMatrix;//合成行列
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
	//スキンウェイトにフレーム番号を設定する
	void SetSkinWeightFrameIndex(CModelX* model);
	//頂点にアニメーション適用
	void AnimateVertex(CModelX* model);
	//指定した合成行列を使って計算
	void AnimateVertex(CMatrix*);
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
	CVector* mpAnimateVertex;//アニメーション用頂点
	CVector* mpAnimateNormal;//アニメーション用法線
	//テクスチャ座標データ
	float* mpTextureCoords;
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
	CAnimationSet();
	CAnimationSet(CModelX* model);
	~CAnimationSet();
	void Time(float time);//時間の設定
	void Weight(float weight);//重みの設定
	void AnimateMatrix(CModelX* model);
	std::vector<CAnimation*>& Animation();
	//mTimeを返す
	float Time();
	//mMaxTimeを返す
	float MaxTime();
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
	CAnimation();
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
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
private:
	//時間
	float mTime;
	//行列
	CMatrix mMatrix;
};
#endif

