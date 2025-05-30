#include "glew.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CModelX.h"
#include "glut.h"
#include "CMaterial.h"
#include "CVertex.h"
#include "Maths.h"

/*
IsDelimiter(c)
cが\t \r \n スペースなどの空白文字
または、;　" などの文字であれば
区切り文字としてtrueを返す
*/
bool CModelX::IsDelimiter(char c)
{
	/*ascii以外の文字だとエラー
	cが0より小さい時、falseを返す
	*/
	if (c <= 0)
	{
		return false;
	}
	//isspace(c)
	//cが空白文字なら0以外を返す
	if (isspace(c) != 0)
		return true;
	/*
	turchr(文字列,文字)
	文字列に文字が含まれていれば、
	見つかった文字へポインタを返す
	見つからなかったらNULLを返す
	*/
	if (strchr(",;\"", c) != NULL)
		return true;
	//区切り文字ではない
	return false;
}

std::vector<CMaterial*>& CModelX::Material()
{
	return mMaterial;
}

std::vector<CModelXFrame*>& CModelX::Frames()
{
	return mFrame;
}

std::vector<CAnimationSet*>& CModelX::AnimationSet()
{
	return mAnimationSet;
}

/*
FinedFrame(フレーム名)
フレーム名に該当するフレームのアドレスを返す
*/
CModelXFrame* CModelX::FinedFrame(const char* name)
{
	//イテレータの作成
	//std::vector<CModelXFrame*>::iterator itr;
	//先頭から最後まで繰り返す
	//for (itr = mFrame.begin(); itr != mFrame.end(); itr++)
	for (auto frame : mFrame)
	{
		//名前が一致したか？
		if (strcmp(name, frame->mpName) == 0)
		{
			//一致したらそのアドレスを返す
			return frame;
		}
	}
	//一致するフレームが無い場合はnullptrを返す
	return nullptr;
}

CMaterial* CModelX::FindMaterial(char* name)
{
	//マテリアル配列のイテレータ作成
	std::vector<CMaterial*>::iterator itr;
	//マテリアル配列を先頭から順に検索
	for (itr = mMaterial.begin(); itr != mMaterial.end(); itr++)
	{
		//名前が一致すればマテリアルのポインタを返却
		if (strcmp(name, (*itr)->Name()) == 0)
		{
			return *itr;
		}
	}
	//無い時はnullptrを返却
	return nullptr;
}

CModelX::CModelX()
	: mpSkinningMatrix(nullptr)
	, mpPointer(nullptr)
	, mLoaded(false)
	, mFilePath("")
	, mDirPath("")
{
	//mTokenを初期化
	memset(mToken, 0, sizeof(mToken));
	//マテリアルの解放
	for (size_t i = 0; i < mMaterial.size(); i++)
	{
		delete mMaterial[i];
	}
}

CModelX::~CModelX()
{
	if (mFrame.size() > 0)
	{
		delete mFrame[0];
	}
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		delete mAnimationSet[i];
	}
	SAFE_DELETE_ARRAY(mpSkinningMatrix);
}

/*
GetToken
文字列データから、単語を1つ取得する
*/
char* CModelX::GetToken()
{
	char* p = mpPointer;
	char* q = mToken;
	//タブ(\t)空白()改行(\r)(\n),;"の区切り文字以外になるまで読み飛ばす
	while (*p != '\0' && IsDelimiter(*p))p++;
	if (*p == '{' || *p == '}')
		*q++ = *p++;
	else
	{
		while (*p != '\0' && !IsDelimiter(*p) && *p != '}')
			*q++ = *p++;
	}

	*q = '\0';    //mTokenの最後に\0を代入
	mpPointer = p; //次の読み込むポイントを更新する

	//もしTokenが//の場合は、コメントなので改行まで読み飛ばす
	/*
	strcmp(文字列1,文字列2)
	文字列１と文字列2が等しい場合、0を返します。
	文字列1と文字列2が等しくない場合、0以外を返します。
	*/
	if (!strcmp("//", mToken))
	{
		//改行まで読み飛ばす
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//読み込み位置の更新
		mpPointer = p;
		//単語を取得する(再帰呼び出し)
		return GetToken();
	}
	return mToken;
}

char* CModelX::Token()
{
	return mToken;
}

bool CModelX::EOT()
{
	if (*mpPointer == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CModelX::IsLoaded()
{
	return mLoaded;
}

std::string CModelX::FilePath() const
{
	return mFilePath;
}

std::string CModelX::DirPath() const
{
	return mDirPath;
}

void CModelX::RenderShader(CMatrix* pCombinedMatrix)
{
	// 完全に透明な状態であれば、描画しない
	if (mColor.A() == 0.0f) return;
	mShader.Render(this, pCombinedMatrix);
}

bool CModelX::IsAddAnimationSet(std::string path) const
{
	for (CAnimationSet* anim : mAnimationSet)
	{
		if (path == anim->mPath) return true;
	}
	return false;
}


void CModelX::AddAnimationSet(const char* file)
{
	if (IsAddAnimationSet(file)) return;

	//ファイルパスにリソースデータのディレクトリパスを追加
	std::string filePath = RES_DIR;
	filePath += file;

	//ファイルサイズを取得する
	FILE* fp;	//ファイルポインタ変数の作成
	fp = fopen(filePath.c_str(), "rb");	//ファイルをオープンする
	//エラーチェック
	if (fp == NULL)
	{
		printf("fopen error:%s\n", filePath.c_str());
		return;
	}
	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);

	//ファイルサイズ+1バイト分の領域を確保
	char* buf = mpPointer = new char[size + 1];

	//ファイルから3Dモデルのデータを読み込む
	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ分でデータを読み込む
	fread(buf, size, 1, fp);
	//最後に\0を設定する(文字列の終端)
	buf[size] = '\0';
	fclose(fp);	//ファイルクローズ

	//文字列の最後まで繰り返し
	while (*mpPointer != '\0')
	{
		GetToken();	//単語の取得
		//template読み飛ばし
		if (strcmp(mToken, "template") == 0)
		{
			SkipNode();
		}
		//単語がAnimationSetの場合
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			CAnimationSet* anim = new CAnimationSet(this);
			anim->mPath = file;
		}
	}
	SAFE_DELETE_ARRAY(buf);	//確保した領域を解放する
}

/*
SeparateAnimationSet
アニメーションを抜き出す
idx:分割したいアニメーションセットの番号
start:分割したいアニメーションの開始時間
end:分割死体アニメーションの終了時間
name:追加するアニメーションセットの名前
*/
void CModelX::SeparateAnimationSet(int idx, int start, int end, char* name)
{
	//分割するアニメーションセットを確定
	CAnimationSet* anim = mAnimationSet[idx];
	//アニメーションセットの生成
	CAnimationSet* as = new CAnimationSet();
	as->mpName = new char[strlen(name) + 1];
	strcpy(as->mpName, name);
	as->mMaxTime = end - start;
	//既存のアニメーション分繰り返し
	for (size_t i = 0; i < anim->mAnimation.size(); i++)
	{
		//アニメーションの生成
		CAnimation* animation = new CAnimation();
		animation->mpFrameName = new char[strlen(anim->mAnimation[i]->mpFrameName) + 1];
		strcpy(animation->mpFrameName, anim->mAnimation[i]->mpFrameName);
		animation->mFrameIndex = anim->mAnimation[i]->mFrameIndex;
		animation->mKeyNum = end - start + 1;
		//アニメーションキーの生成
		animation->mpKey = new CAnimationKey[animation->mKeyNum];
		animation->mKeyNum = 0;
		for (int j = start; j <= end && anim->mAnimation[i]->mKeyNum; j++)
		{
			if (j < anim->mAnimation[i]->mKeyNum)
			{
				animation->mpKey[animation->mKeyNum] = anim->mAnimation[i]->mpKey[j];
			}
			else
			{
				animation->mpKey[animation->mKeyNum] =
					anim->mAnimation[i]->mpKey[anim->mAnimation[i]->mKeyNum - 1];
			}
			animation->mpKey[animation->mKeyNum].mTimeLocation = animation->mKeyNum++;
		} //アニメーションキーのコピー
		//アニメーションの追加
		as->mAnimation.push_back(animation);
	}
	//アニメーションセットの追加
	mAnimationSet.push_back(as);
}

void CModelX::AnimateVertex(CMatrix* mat)
{
	//フレーム数分繰り返し
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//メッシュが有れば
		if (mFrame[i]->mpMesh)
		{
			//頂点をアニメーションで更新する
			mFrame[i]->mpMesh->AnimateVertex(mat);
		}
	}
}

/*
AnimateVertex
頂点にアニメーションを適用する
*/
void CModelX::AnimateVertex()
{
	//フレーム数分繰り返し
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//メッシュに面があれば
		if (mFrame[i]->mpMesh != nullptr)
		{
			//頂点をアニメーションで更新する
			mFrame[i]->mpMesh->AnimateVertex(this);
		}
	}
}

void CModelX::SetSkinWeightFrameIndex()
{
	//フレーム数分繰り返し
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//メッシュが有れば
		if (mFrame[i]->mpMesh != nullptr)
		{
			mFrame[i]->mpMesh->SetSkinWeightFrameIndex(this);
		}
	}
}

/*
AnimateFrame
フレームの変換行列をアニメーションデータで更新する
*/
void CModelX::AnimateFrame()
{
	//アニメーションで適用されるフレームの変換行列をゼロクリアする
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		CAnimationSet* animSet = mAnimationSet[i];
		//重みが0が飛ばす
		if (animSet->mWeight == 0) continue;
		//フレーム分(Animation分)繰り返す
		for (size_t j = 0; j < animSet->Animation().size(); j++)
		{
			CAnimation* animation = animSet->Animation()[j];
			//該当するフレームの変換行列をゼロクリアする
			memset(&mFrame[animation->mFrameIndex]->mTransformMatrix, 0, sizeof(CMatrix));
		}
	}
	//アニメーションに該当するフレームの変換行列をアニメーションのデータで設定する
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		CAnimationSet* animSet = mAnimationSet[i];
		//重みが0は飛ばす
		if (animSet->mWeight == 0) continue;
		animSet->AnimateMarix(this);
	}
#ifdef _DEBUG
	//for (int i = 1; i < 5; i++)
	//{
	//	printf("Frame:%s\n", mFrame[i]->mpName);
	//	for (int row = 0; row < 4; row++)
	//	{
	//		for (int col = 0; col < 4; col++)
	//		{
	//			printf("%f	", mFrame[i]->mTransformMatrix.M(row, col));
	//		}
	//		printf("\n");
	//	}
	//}
#endif
}

/*
SkipNode
ノードを読み飛ばす
*/
void CModelX::SkipNode()
{
	//文字が終わったら終了
	while (*mpPointer != '\0')
	{
		GetToken(); //次の単語取得
		//{が見つかったらループ終了
		if (strchr(mToken, '{')) break;
	}
	int count = 1;
	//文字が終わるか、カウントが0になったら終了
	while (*mpPointer != '\0' && count > 0)
	{
		GetToken();  //次の単語取得
		//{を見つけるとカウントアップ
		if (strchr(mToken, '{')) count++;
		//}を見つけるとカウントダウン
		else if (strchr(mToken, '}')) count--;
	}
}

// カラーを設定
void CModelX::SetColor(const CColor& color)
{
	mColor = color;
}

// カラーを取得
const CColor& CModelX::GetColor() const
{
	return mColor;
}

// アルファ値設定
void CModelX::SetAlpha(float alpha)
{
	mColor.A(Math::Clamp01(alpha));
}

// アルファ値取得
float CModelX::GetAlpha() const
{
	return mColor.A();
}

bool CModelX::Load(std::string path, bool dontDelete)
{
	//ファイルサイズを取得する
	FILE* fp;

	//ファイルパスにリソースデータのディレクトリパスを追加
	std::string filePath = RES_DIR + path;

	fp = fopen(filePath.c_str(), "rb"); //ファイルをオープンする
	//エラーチェック
	if (fp == NULL)
	{
		printf("fopen error:%s\n", filePath.c_str());
		return false;
	}
	printf("%s\n", filePath.c_str());

	//ファイルパスを記憶
	mFilePath = filePath;
	//ファイルパスからディレクトリパスを取得
	mDirPath = path;
	size_t index = mDirPath.find_last_of('\\');
	if (index >= 0)
	{
		mDirPath = mDirPath.substr(0, index + 1);
	}

	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);
	//ファイルサイズ+1バイト文の領域を確保
	char* buf = mpPointer = new char[size + 1];
	//ファイルから3Dモデルのデータを読み込む
	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ文データを読み込む
	fread(buf, size, 1, fp);
	//最後に\0を設定する(文字列の終端)
	buf[size] = '\0';

	//ダミールートフレームの作成
	CModelXFrame* p = new CModelXFrame();
	//名前なし
	p->mpName = new char[1];
	p->mpName[0] = '\0';
	//フレーム配列に追加
	mFrame.push_back(p);

	//文字列の最後まで繰り返し
	while (*mpPointer != '\0')
	{
		GetToken(); //単語の取得
		//template 読み飛ばし
		if (strcmp(mToken, "template") == 0)
		{
			SkipNode();
		}
		//Materialの時
		else if (strcmp(mToken, "Material") == 0)
		{
			new CMaterial(this, dontDelete);
		}
		//単語がFrameの場合
		else if (strcmp(mToken, "Frame") == 0)
		{
			//フレーム名取得
			GetToken();
			if (strchr(mToken, '{'))
			{
				//フレーム名なし : スキップ
				SkipNode();
				GetToken();	// }
			}
			else
			{
				//フレーム名が無ければ
				if (FinedFrame(mToken) == 0)
				{
					//フレームを作成する
					p->mChild.push_back(new CModelXFrame(this, dontDelete));
				}
			}
		}
		//単語がAnimationSetの場合
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			new CAnimationSet(this);
		}
	}
	fclose(fp); //ファイルをクローズする

	SAFE_DELETE_ARRAY(buf); //確保した領域を解放する
	//スキンウェイトのフレーム番号設定
	SetSkinWeightFrameIndex();
	mLoaded = true;	//読み込み済み

	//頂点バッファ作成
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		if (mFrame[i]->mpMesh != nullptr)
		{
			mFrame[i]->mpMesh->CreateVertexBuffer();
		}
	}
	//スキンマトリックスのエリア作成
	mpSkinningMatrix = new CMatrix[mFrame.size()];
	//シェーダー読み込み
	mShader.Load("Shader\\skinmesh.vert", "Shader\\skinmesh.flag");

	return true;

}

/*
Render
すべてのフレームの描画処理を呼び出す
*/
void CModelX::Render()
{
	// 完全に透明な状態であれば、描画しない
	if (mColor.A() == 0.0f) return;
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		mFrame[i]->Render(mColor);
	}
}

//デフォルトコンストラクタ
CModelXFrame::CModelXFrame()
	: mpMesh(nullptr)
	, mpName(nullptr)
	, mIndex(0)
{

}

/*
CModelXFrame
model:CModelXインスタンスへのポインタ
プレームを作成する
読み込み中にFrameが見つかれば、フレームを作成し、
子フレームに追加する
*/
CModelXFrame::CModelXFrame(CModelX* model, bool dontDelete)
	: mpMesh(nullptr)
	, mpName(nullptr)
	, mIndex(0)
{
	//現在のフレーム配列に要素数を取得し設定する
	mIndex = model->mFrame.size();
	//CModelXフレーム配列に追加する
	model->mFrame.push_back(this);
	//変換行列を単位行列にする
	mTransformMatrix.Identity();
	//フレーム名分エリアを確保する
	mpName = new char[strlen(model->mToken) + 1];
	//フレーム名コピーする
	strcpy(mpName, model->mToken);
	//次の単語({の予定)を取得する
	model->GetToken();//{
	//文字がなくなったら終わり
	while (*model->mpPointer != '\0')
	{
		//次の単語取得
		model->GetToken(); //Frame
	    //}の場合は終了
		if (strchr(model->mToken, '}'))break;
		//新たなフレームの場合は、子フレームに追加
		if (strcmp(model->mToken,"Frame") == 0)
		{
			//フレーム名取得
			model->GetToken();
			if (strchr(model->mToken, '{'))
			{
				//フレーム名なし : スキップ
				model->SkipNode();
				model->GetToken();	// }
			}
			else
			{
				//フレームが無ければ
				if (model->FinedFrame(model->mToken) == 0)
				{
					//フレームを作成し、子フレームの配列に追加
					mChild.push_back(new CModelXFrame(model, dontDelete));
				}
			}
		}
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0)
		{
			model->GetToken();  //{
			for (int i = 0; i < mTransformMatrix.Size(); i++)
			{
				//atof->文字列のデータをfloat型に変換する
				mTransformMatrix.M()[i] = atof(model->GetToken());
			}
			model->GetToken();
		}
		else if (strcmp(model->mToken, "Mesh") == 0)
		{
		mpMesh = new CMesh;
		mpMesh->Init(model, dontDelete);
		}
		else
		{
		//上記以外の要素は読み飛ばす
		model->SkipNode();
		}
	}
	//デバッグバージョンのみ有効
#ifdef _DEBUG
	//printf("%s\n", mpName);
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	", mTransformMatrix.M(i, j));
	//	}
	//	printf("\n");
	//}
#endif
}

CModelXFrame::~CModelXFrame()
{
	//子フレームを全て解放する
	std::vector<CModelXFrame*>::iterator itr;
	for (itr = mChild.begin(); itr != mChild.end(); itr++) {
		delete* itr;
	}
	//名前のエリアを解放する
	SAFE_DELETE_ARRAY(mpName);

	if (mpMesh != nullptr)
		delete mpMesh;
}

int CModelXFrame::Index()
{
	return mIndex;
}

const CMatrix& CModelXFrame::CombinedMatrix()
{
	return mCombinedMatrix;
}

/*
AnimateCombined
合成行列の作成
*/
void CModelXFrame::AnimateCombined(const CMatrix* parent)
{
	//自分の変換行列に、親から変換行列を掛ける
	mCombinedMatrix = mTransformMatrix * (*parent);
	//子フレームの合成行列を作成する
	for (size_t i = 0; i < mChild.size(); i++)
	{
		mChild[i]->AnimateCombined(&mCombinedMatrix);
	}
#ifdef _DEBUG
	//printf("Frame:%s\n", mpName);
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	", mCombinedMatrix.M(i, j));
	//	}
	//	printf("\n");
	//}
#endif
}

/*
Render
メッシュが存在すれば描画する
*/
void CModelXFrame::Render(const CColor& color)
{
	if (mpMesh != nullptr)
		mpMesh->Render(color);
}

//コンストラクタ
CMesh::CMesh()
	: mFaceNum(0)
	, mVertexNum(0)
	, mNormalNum(0)
	, mMaterialNum(0)
	, mMaterialIndexNum(0)
	, mpMaterialIndex(nullptr)
	, mpVertexIndex(nullptr)
	, mpTextureCoords(nullptr)
	, mpVertex(nullptr)
	, mpAnimateVertex(nullptr)
	, mpAnimateNormal(nullptr)
	, mpNormal(nullptr)
	, mMyVertexBufferId(0)
{

}

//デストラクタ
CMesh::~CMesh()
{
	SAFE_DELETE_ARRAY(mpVertex);
	SAFE_DELETE_ARRAY(mpVertexIndex);
	SAFE_DELETE_ARRAY(mpNormal);
	SAFE_DELETE_ARRAY(mpMaterialIndex);
	SAFE_DELETE_ARRAY(mpTextureCoords);
	SAFE_DELETE_ARRAY(mpAnimateVertex);
	SAFE_DELETE_ARRAY(mpAnimateNormal);
	//スキンウェイトの削除
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		delete mSkinWeights[i];
	}
}

void CMesh::CreateVertexBuffer()
{
	//メッシュ毎に一回作成すればよい
	if (mMyVertexBufferId > 0)
		return;
	if (mVertexNum > 0)
	{
		//頂点インデックスを使わず、全ての面データを作成
		CVertex* pmyVertex, * vec;
		//頂点数計算
		int myVertexNum = mFaceNum * 3;
		//頂点数分頂点配列作成
		pmyVertex = new CVertex[myVertexNum];
		vec = new CVertex[mVertexNum];
		for (int j = 0; j < mVertexNum; j++)
		{
			//頂点座標設定
			vec[j].mPosition = mpVertex[j];
			//テクスチャマッピング設定
			if (mpTextureCoords != NULL)
			{
				vec[j].mTextureCoords.X(mpTextureCoords[j * 2]);
				vec[j].mTextureCoords.Y(mpTextureCoords[j * 2 + 1]);
			}
			vec[j].mBoneWeight[0] = 1.0f;
		}
		int wi = 0;
		//スキンウェイト設定
		for (size_t k = 0; k < mSkinWeights.size(); k++)
		{
			for (int l = 0; l < mSkinWeights[k]->mIndexNum; l++)
			{
				int idx = mSkinWeights[k]->mpIndex[l];
				for (int m = 0; m < 4; m++)
				{
					if (vec[idx].mBoneIndex[m] == 0)
					{
						vec[idx].mBoneIndex[m] =
							mSkinWeights[k]->mFrameIndex;
						vec[idx].mBoneWeight[m] =
							mSkinWeights[k]->mpWeight[l];
						break;
					}
				}
			}
		}
		int k = 0;
		//マテリアル番号の昇順に面の頂点を設定
		for (size_t i = 0; i < mMaterial.size(); i++)
		{
			int w = k;
			for (int j = 0; j < mMaterialIndexNum; j++)
			{
				if (mpMaterialIndex[j] == i)
				{
					//頂点配列に設定し、法線を設定する
					pmyVertex[k] = vec[mpVertexIndex[j * 3]];
					pmyVertex[k++].mNormal = mpNormal[j * 3];
					pmyVertex[k] = vec[mpVertexIndex[j * 3 + 1]];
					pmyVertex[k++].mNormal = mpNormal[j * 3 + 1];
					pmyVertex[k] = vec[mpVertexIndex[j * 3 + 2]];
					pmyVertex[k++].mNormal = mpNormal[j * 3 + 2];
				}
			}
			//マテリアル毎の頂点数を追加する
			mMaterialVertexCount.push_back(k - w);
		}
		//頂点バッファの作成
		glGenBuffers(1, &mMyVertexBufferId);
		//頂点バッファをバインド
		glBindBuffer(GL_ARRAY_BUFFER, mMyVertexBufferId);
		//バインドしたバッファにデータを転送
		glBufferData(GL_ARRAY_BUFFER
			, sizeof(CVertex) * myVertexNum
			, pmyVertex, GL_STATIC_DRAW);
		//バインド解除
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//配列解放
		delete[] pmyVertex;
		delete[] vec;
		pmyVertex = nullptr;
	}
}

void CMesh::AnimateVertex(CMatrix* mat)
{
	//アニメーション用の頂点エリアクリア
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
	//スキンウェイト分繰り返し
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//フレーム番号取得
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//オフセット行列とフレーム合成行列を合成
		CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * mat[frameIndex];
		//頂点数分繰り返し
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++)
		{
			//頂点番号取得
			int index = mSkinWeights[i]->mpIndex[j];
			//重み取得
			float weight = mSkinWeights[i]->mpWeight[j];
			//頂点と法線を更新する
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
	}
	//法線を正規化する
	for (int i = 0; i < mNormalNum; i++)
	{
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalized();
	}
}

void CMesh::AnimateVertex(CModelX* model)
{
	//アニメーション用の頂点エリアクリア
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
	//スキンウェイト分繰り返し
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//フレーム番号取得
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//オフセット行列とフレーム合成行列を合成
		CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * model->Frames()[frameIndex]->CombinedMatrix();
		//頂点数分繰り返し
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++)
		{
			//頂点番号取得
			int index = mSkinWeights[i]->mpIndex[j];
			//重み取得
			float weight = mSkinWeights[i]->mpWeight[j];
			//頂点と法線を更新する
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
	}
	//法線を正規化する
	for (int i = 0; i < mNormalNum; i++)
	{
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalized();
	}
}

void CMesh::SetSkinWeightFrameIndex(CModelX* model)
{
	//スキンウェイト分繰り返し
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//フレーム名のフレームを取得する
		CModelXFrame* frame = model->FinedFrame(mSkinWeights[i]->mpFrameName);
		//フレーム番号を設定する
		mSkinWeights[i]->mFrameIndex = frame->Index();
	}
}

/*
Init
Meshのデータを読み込む
*/
void CMesh::Init(CModelX* model, bool dontDelete)
{
	model->GetToken();	//{ or 名前
	if (!strchr(model->mToken, '{'))
	{
		//名前がの場合、次が{
		model->GetToken();	// {
	}
	//頂点数の取得
	mVertexNum = atoi(model->GetToken());
	//頂点数分エリア確保
	mpVertex = new CVector[mVertexNum];
	mpAnimateVertex = new CVector[mVertexNum];
	//頂点数分データを取り込む
	for (int i = 0; i < mVertexNum; i++)
	{
		mpVertex[i].X(atof(model->GetToken()));
		mpVertex[i].Y(atof(model->GetToken()));
		mpVertex[i].Z(atof(model->GetToken()));
	}
	//面数の読み込み
	mFaceNum = atoi(model->GetToken());
	//頂点数は1面に3頂点
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		model->GetToken(); //頂点数読み飛ばし
		mpVertexIndex[i] = atoi(model->GetToken());
		mpVertexIndex[i + 1] = atoi(model->GetToken());
		mpVertexIndex[i + 2] = atoi(model->GetToken());
	}

	//単語がある間繰り返し
	while (!model->EOT())
	{
		model->GetToken(); //MeshNormals
		// }かっこの場合は終了
		if (strchr(model->Token(), '}'))
			break;
		if (strcmp(model->Token(), "MeshNormals") == 0)
		{
			model->GetToken(); // {
			//法線データを取得
			mNormalNum = atoi(model->GetToken());
			//法線データを配列に取り込む
			CVector* pNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i++)
			{
				pNormal[i].X(atof(model->GetToken()));
				pNormal[i].Y(atof(model->GetToken()));
				pNormal[i].Z(atof(model->GetToken()));
			}
			//法線数 = 面積 ×　3
			mNormalNum = atoi(model->GetToken()) * 3; //FaceNum
			int ni;
			//頂点毎に法線データを設定する
			mpNormal = new CVector[mNormalNum];
			mpAnimateNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i += 3)
			{
				model->GetToken(); //3
				ni = atoi(model->GetToken());
				mpNormal[i] = pNormal[ni];

				ni = atoi(model->GetToken());
				mpNormal[i + 1] = pNormal[ni];

				ni = atoi(model->GetToken());
				mpNormal[i + 2] = pNormal[ni];

			}
			delete[] pNormal;
			model->GetToken(); // }
		} //End of MeshNormal

		//MeshMaterialListのとき
		else if (strcmp(model->Token(), "MeshMaterialList") == 0)
		{
			model->GetToken();  // {
			//Materialの数
			mMaterialNum = atoi(model->GetToken());
			//FaceNum
			mMaterialIndexNum = atoi(model->GetToken());
			//マテリアルインデックスの作成
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++)
			{
				mpMaterialIndex[i] = atoi(model->GetToken());
			}
			//マテリアルデータの作成
			for (int i = 0; i < mMaterialNum; i++)
			{
				model->GetToken();  //Material
				if (strcmp(model->Token(), "Material") == 0)
				{
					mMaterial.push_back(new CMaterial(model, dontDelete));
				}
				else
				{//既出
					model->GetToken();	//MaterialName
					mMaterial.push_back(model->FindMaterial(model->Token()));
					model->GetToken();	// }
				}
			}
			model->GetToken();  // } End of MeshMaterialList
		}// End of MaterialList
		//SkinWeghtsのとき
		else if (strcmp(model->Token(), "SkinWeights") == 0)
		{
			//CSkinWeghtsクラスのインスタンスを作成し、配列に追加
			mSkinWeights.push_back(new CSkinWeights(model));
		}
		//テクスチャ座標の時
		else if (strcmp(model->Token(), "MeshTextureCoords") == 0)
		{
			model->GetToken();	// {
			//テクスチャ座標数を取得
			int textureCoordsNum = atoi(model->GetToken()) * 2;
			//テクスチャ座標のデータを配列に取り込む
			mpTextureCoords = new float[textureCoordsNum];
			for (int i = 0; i < textureCoordsNum; i++)
			{
				mpTextureCoords[i] = atof(model->GetToken());
				//OpenGLで読み込むためY座標は反転
				if (i % 2 == 1)
				{
					mpTextureCoords[i] = 1.0f - mpTextureCoords[i];
				}
			}
			model->GetToken();	// }
		}
		else
		{
			//以外のノードは読み飛ばし
			model->SkipNode();
		}
	}
#ifdef _DEBUG
	////頂点データの出力
	//printf("VertexNum:%d\n", mVertexNum);
	//for (int i = 0; i < mVertexNum; i++)
	//{
	//printf("%f	%f	%f	\n", mpVertex[i].X(), mpVertex[i].Y(), mpVertex[i].Z());
	//}
	////面のデータの出力
	//printf("FaceNum:%d\n", mFaceNum);
	//for (int i = 0; i < mFaceNum * 3; i += 3)
	//{
	//	printf("%d	%d	%d	\n",
	//		mpVertexIndex[i], mpVertexIndex[i + 1], mpVertexIndex[i + 2]);
	//}
	////法線データの出力
	//printf("NormalNum:%d\n", mNormalNum);
	//for (int i = 0; i < mNormalNum; i++)
	//{
	//	printf("%f	%f	%f	\n",
	//		mpNormal[i].X(), mpNormal[i].Y(), mpNormal[i].Z());
	//}
#endif
}

/*
Render
画面に描画する
*/
void CMesh::Render(const CColor& color)
{
	/*頂点データ,法線データの配列を有効にする*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//テクスチャマッピングの配列を有効にする
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/*頂点データ,法線データの場所を指定する*/
	glVertexPointer(3, GL_FLOAT, 0, mpAnimateVertex);
	glNormalPointer(GL_FLOAT, 0, mpAnimateNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoords);

	/*頂点インデックスの場所を指定して図形を描画する*/
	for (int i = 0; i < mFaceNum; i++)
	{
		//マテリアルを適用する
		mMaterial[mpMaterialIndex[i]]->Enabled(color);
		/*頂点のインデックスの場所を指定して図形を描画する*/
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
		mMaterial[mpMaterialIndex[i]]->Disabled();
	}

	/*頂点データ,法線データの配列を無効にする*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

/*
CSkinWeights
スキンウェイトの読み込み
*/
CSkinWeights::CSkinWeights(CModelX* model)
	: mpFrameName(0)
	, mFrameIndex(0)
	, mIndexNum(0)
	, mpIndex(nullptr)
	, mpWeight(nullptr)
{
	model->GetToken();	// {
	model->GetToken();	// Framename
	//フレーム名エリア確保、設定
	mpFrameName = new char[strlen(model->Token()) + 1];
	strcpy(mpFrameName, model->Token());

	//頂点番号数取得
	mIndexNum = atoi(model->GetToken());
	//頂点番号が0を超える
	if (mIndexNum > 0)
	{
		//頂点番号と頂点ウェイトのエリア確保
		mpIndex = new int[mIndexNum];

		mpWeight = new float[mIndexNum];
		//頂点番号取得
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = atoi(model->GetToken());
		//頂点ウェイト取得
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = atof(model->GetToken());
	}
	//オフセット行列取得
	for (int i = 0; i < 16; i++)
	{
		mOffset.M()[i] = atof(model->GetToken());
	}
	model->GetToken();	// }
#ifdef _DEBUG
	////SkinWeghts　フレーム名の出力
	//printf("SKinWeights %s\n",mpFrameName);
	////頂点番号　頂点ウェイトの出力
	//for (int i = 0; i < mIndexNum; i++)
	//{
	//	printf("%d  %f\n",mpIndex[i],mpWeight[i]);
	//}
	////オフセット行列の出力
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	", mOffset.M(i,j));
	//	}
	//	printf("\n");
	//}
#endif
}

CSkinWeights::~CSkinWeights()
{
	SAFE_DELETE_ARRAY(mpFrameName);
	SAFE_DELETE_ARRAY(mpIndex);
	SAFE_DELETE_ARRAY(mpWeight);
}

//コンストラクタ
CAnimationSet::CAnimationSet()
{

}

/*
CAnimationSet
*/
CAnimationSet::CAnimationSet(CModelX* model)
	: mpName(nullptr)
	, mTimeLocation(0)
	, mWeight(0)
	, mMaxTime(0)
{
	model->mAnimationSet.push_back(this);
	model->GetToken();	//Animation Name
	//アニメーションセット名を退避
	mpName = new char[strlen(model->Token()) + 1];
	strcpy(mpName, model->Token());
	model->GetToken();	// {
	while (!model->EOT())
	{
		model->GetToken();	// } or Animation
		if (strchr(model->Token(), '}'))
			break;
		if (strcmp(model->Token(), "Animation") == 0)
		{
			//Animation要素の読み込み
			mAnimation.push_back(new CAnimation(model));
		}
	}
	//終了時間設定
	mMaxTime = mAnimation[0]->mpKey[mAnimation[0]->mKeyNum - 1].mTimeLocation;
#ifdef _DEBUG
	//printf("AnimationSet:%s\n", mpName);
#endif
}

//デストラクタ
CAnimationSet::~CAnimationSet()
{
	//アニメーション要素の削除
	for (size_t i = 0; i < mAnimation.size(); i++)
	{
		delete mAnimation[i];
	}
}


float CAnimationSet::Time()
{
	return mTimeLocation;
}

float CAnimationSet::MaxTime()
{
	return mMaxTime;
}

std::vector<CAnimation*>& CAnimationSet::Animation()
{
	return mAnimation;
}

void CAnimationSet::AnimateMarix(CModelX* model)
{
	//重みが0は飛ばす
	if (mWeight == 0) return;
	//フレーム分(Animation分)繰り返す
	for (size_t j = 0; j < mAnimation.size(); j++)
	{
		//フレームを取得する
		CAnimation* animation = mAnimation[j];
		//キーが無い場合は次のアニメーションへ
		if (animation->mpKey == nullptr) continue;
		//該当するフレームの取得
		CModelXFrame* frame = model->mFrame[animation->mFrameIndex];
		//最初の時間より小さい場合
		if (mTimeLocation < animation->mpKey[0].mTimeLocation)
		{
			//変換行列を0コマ目の行列で更新
			frame->mTransformMatrix += animation->mpKey[0].mMatrix * mWeight;
		}
		//最後の時間より大きい場合
		else if (mTimeLocation >= animation->mpKey[animation->mKeyNum - 1].mTimeLocation)
		{
			//変換行列を最後のコマの行列で更新
			frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * mWeight;
		}
		else
		{
			//時間の途中の場合
			for (int k = 1; k < animation->mKeyNum; k++)
			{
				//変換行列を、線形補間にて更新
				if (mTimeLocation < animation->mpKey[k].mTimeLocation &&
					animation->mpKey[k - 1].mTimeLocation != animation->mpKey[k].mTimeLocation)
				{
					float r = (animation->mpKey[k].mTimeLocation - mTimeLocation) /
						(animation->mpKey[k].mTimeLocation - animation->mpKey[k - 1].mTimeLocation);

					frame->mTransformMatrix +=
						(animation->mpKey[k - 1].mMatrix * r +
							animation->mpKey[k].mMatrix * (1 - r)) * mWeight;
					break;
				}
			}
		}
	}
}

void CAnimationSet::Time(float time)
{
	mTimeLocation = Math::Clamp(time, 0.0f, mMaxTime);
}

void CAnimationSet::TimeProgress(float progress)
{
	mTimeLocation = mMaxTime * Math::Clamp01(progress);
}

void CAnimationSet::Weight(float weight)
{
	mWeight = weight;
}

//コンストラクタ
CAnimation::CAnimation()
{

}

/*
CAnimation
アニメーションクラス
*/
CAnimation::CAnimation(CModelX* model)
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)
{
	model->GetToken(); // { or Animation Name
	if (strchr(model->Token(), '{'))
	{
		model->GetToken(); // {
	}
	else
	{
		model->GetToken(); // {
		model->GetToken(); // {
	}
	model->GetToken(); //FrameName
	mpFrameName = new char[strlen(model->Token()) + 1];
	strcpy(mpFrameName, model->Token());
	mFrameIndex = 
		model->FinedFrame(model->Token())->Index();
	model->GetToken();	// }
	//キーの配列を保存しておく配列
	CMatrix* key[4] = { 0,0,0,0 };
	//時間の配列を保存しておく配列
	float* time[4] = { 0,0,0,0 };
	while (!model->EOT())
	{
		model->GetToken();	// { or AnimationKey
		if (strchr(model->Token(), '}'))
			break;
		if (strcmp(model->Token(), "AnimationKey") == 0)
		{
			model->GetToken();	// {
			//データのタイプ取得
			int type = atoi(model->GetToken());
			//時間数取得
			mKeyNum = atoi(model->GetToken());
			switch (type)
			{
			case 0: // Rotation Quaternion
				//行列の配列を時間数分確保
				key[type] = new CMatrix[mKeyNum];
				//時間の配列を時間数分確保
				time[type] = new float[mKeyNum];
				//時間数分繰り返す
				for(int i = 0; i < mKeyNum; i++)
				{
					//時間取得
					time[type][i] = atof(model->GetToken());
					model->GetToken();	//4を読み飛ばし
					//w,x,y,zを取得
					float w = atof(model->GetToken());
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//クオータニオンから回転行列に変換
					key[type][i].Quaternion(x, y, z, w);
				}
				break;
			case 1:	//拡大・縮小の行列作成
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++)
				{
					time[type][i] = atof(model->GetToken());
					model->GetToken();	//3
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					key[type][i].Scale(x, y, z);
				}
				break;
			case 2:	//移動の行列作成
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++)
				{
					time[type][i] = atof(model->GetToken());
					model->GetToken();	//3
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					key[type][i].Translate(x, y, z);
				}
				break;
			case 4:	//行列データを取得
				mpKey = new CAnimationKey[mKeyNum];
				for (int i = 0; i < mKeyNum; i++)
				{
					mpKey[i].mTimeLocation = atof(model->GetToken());	//Time
					model->GetToken();	//16
					for (int j = 0; j < 16; j++)
					{
						mpKey[i].mMatrix.M()[j] = atof(model->GetToken());
					}
				}
				break;
			}
			model->GetToken();	// }
		}
		else
		{
			model->SkipNode();
		}
	}
	//行列のデータではない時
	if (mpKey == 0)
	{
		//時間数分キーを作成
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++)
		{
			//時間設定
			mpKey[i].mTimeLocation = time[2][i];	//Time
			//行列作成 Size * Rotation * Position
			mpKey[i].mMatrix = key[1][i] * key[0][i] * key[2][i];
		}
	}
	//確保したエリア解放
	for (int i = 0; i < ARRAY_SIZE(key);i++)
	{
		SAFE_DELETE_ARRAY(time[i]);
		SAFE_DELETE_ARRAY(key[i]);
	}
#ifdef _DEBUG
	//printf("Animation::%s\n", mpFrameName);
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	",mpKey[0].mMatrix.M(i,j));
	//	}
	//	printf("\n");
	//}
#endif
}

//デストラクタ
CAnimation::~CAnimation()
{
	SAFE_DELETE_ARRAY(mpFrameName);
	SAFE_DELETE_ARRAY(mpKey);
}