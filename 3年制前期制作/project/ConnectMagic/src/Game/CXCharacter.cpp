#include "CXCharacter.h"
#include "Maths.h"

//コンストラクタ
CXCharacter::CXCharacter(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CCharaBase(tag, prio, sortOrder, pause)
	, mpModel(nullptr)
	, mpCombinedMatrix(nullptr)
	, mAnimationLoopFlg(false)
	, mAnimationIndex(0)
	, mAnimationFrame(0.0f)
	, mAnimationFrameSize(0.0f)
	, mAnimationSpeed(1.0f)
	, mElapsedBlend(0.0f)
	, mBlendIndex(-1)
{
}

//デストラクタ
CXCharacter::~CXCharacter()
{
	SAFE_DELETE_ARRAY(mpCombinedMatrix)
}

/*
Init
初期化処理
*/
void CXCharacter::Init(CModelX* model)
{
	mpModel = model;
	//合成行列退避エリアの確保
	mpCombinedMatrix = new CMatrix[model->Frames().size()];
	//最初のアニメーションにする
	mAnimationIndex = 0;
	//繰り返し再生する
	mAnimationLoopFlg = true;
	//1アニメーション目の最大フレーム数
	mAnimationFrameSize = model->AnimationSet()[0]->MaxTime();
	//アニメーションのフレームを最大にする
	mAnimationFrame = 0.0f;
	mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
	//アニメーションの重みを1.0(100%)にする
	mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f);
}

// アニメーションとモデルの初期化処理
void CXCharacter::InitAnimationModel(std::string path, const std::vector<AnimData>* pAnimData)
{
	// アニメーションデータテーブルを設定
	mpAnimData = pAnimData;

	// モデルデータを読み込み
	CModelX* model = CResourceManager::Get<CModelX>(path);

	// テーブル内のアニメーションデータを読み込み
	int size = mpAnimData->size();
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = (*mpAnimData)[i];
		if (data.path.empty())continue;
		model->AddAnimationSet(data.path.c_str());
	}

	// モデルの初期化
	Init(model);
}


/*
ChangeAnimation
アニメーションを切り替える
index:アニメーションの番号
loop:true:繰り返す
framesize:最後まで再生するのに使用されるフレーム数
*/
void CXCharacter::ChangeAnimation(int index, bool loop,
	float framesize, bool restart)
{
	// ブレンドの進行度を0にする
	mElapsedBlend = 0.0f;
	if (mBlendIndex != -1)
	{
		// ブレンドのアニメーションの重みを0.0(0%)にする
		mpModel->AnimationSet()[mBlendIndex]->Weight(0.0f);
		mBlendIndex = -1;
	}
	//最初から開始しない場合かつ、同じアニメーションの場合は切り替えない
	if (!restart && mAnimationIndex == index) return;
	//今のアニメーションの重みを0.0(0%)にする
	mpModel->AnimationSet()[mAnimationIndex]->Weight(0.0f);
	//番号、繰り返し、フレーム数を指定
	mAnimationIndex = index % mpModel->AnimationSet().size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//アニメーションのフレームを最初にする
	mAnimationFrame = 0.0f;
	mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
	//アニメーションの重みを1.0(100%)にする
	mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f);
}

/*
ChangeAnimationBlend
アニメーションをブレンドしながら切り替える
index:アニメーションの番号
loop:true:繰り返す
framesize:最後まで再生するのに使用されるフレーム数
restart:最初から開始するか
blendSpeed:ブレンドしながら切り替える速度
*/
bool CXCharacter::ChangeAnimationBlend(int index, bool loop, 
	float framesize, bool restart, float blendSpeed)
{
	//最初から開始しない場合かつ、同じアニメーションの場合は切り替えない
	if (!restart && mAnimationIndex == index) return true;

	// ブレンドするアニメーション番号と違うなら処理する
	if (mBlendIndex != index);
	{
		if (mBlendIndex != -1)
		{
			// 現在のブレンドするアニメーションのフレームを最初にする
			mpModel->AnimationSet()[mBlendIndex]->Time(0.0f);
			// 現在のブレンドするアニメーションの重みを0にする
			mpModel->AnimationSet()[mBlendIndex]->Weight(0.0f);
		}
		// ブレンドするアニメーション番号を設定
		mBlendIndex = index % mpModel->AnimationSet().size();
		// 次のブレンドするアニメーションのフレームを最初にする
		mpModel->AnimationSet()[mBlendIndex]->Time(0.0f);
	}
	// ブレンド進行度を進める
	mElapsedBlend += blendSpeed * Times::DeltaTime();

	// 進行度が1.0より小さい場合
	if (mElapsedBlend < 1.0f)
	{
		// 今のアニメーションの重みを1.0−進行度にする
		mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f - mElapsedBlend);
		// 次のアニメーションの重みを進行度にする
		mpModel->AnimationSet()[mBlendIndex]->Weight(mElapsedBlend);

		// ブレンドが終了していない
		return false;
	}
	// 進行度が1.0以上の場合
	else
	{
		// アニメーションを変更
		ChangeAnimation(index, loop, framesize, restart);

		// ブレンド終了
		return true;
	}
}

/*
Update
更新する
matrix:移動、回転、拡大縮小の行列
*/
void CXCharacter::Update(const CMatrix& matrix)
{
	if (mpModel == nullptr) return;

	auto& animSet = mpModel->AnimationSet();
	CAnimationSet* currAnim = animSet[mAnimationIndex];
	CAnimationSet* blendAnim;
	// ブレンドするアニメーションが設定されているなら
	if (mBlendIndex >= 0)
	{
		blendAnim = animSet[mBlendIndex];
	}

	for (size_t i = 0; i < animSet.size(); i++)
	{
		animSet[i]->mWeight = 0.0f;
		currAnim->mWeight = 1.0f - mElapsedBlend;
		// ブレンドするアニメーションが設定されているなら
		if (mBlendIndex >= 0)
		{
			blendAnim->mWeight = mElapsedBlend;
		}
	}
	//最後まで再生する
	if (0.0f <= mAnimationFrame && mAnimationFrame <= mAnimationFrameSize)
	{
		// 進むフレーム数
		float frame = mAnimationSpeed * 60.0f * Times::DeltaTime();

		//アニメーションの時間を設定
		currAnim->Time(mAnimationFrame);
		// ブレンドするアニメーションが設定されているなら
		if (mBlendIndex >= 0)
		{
			// ブレンドするアニメーションの時間
			float blendTime = blendAnim->mTimeLocation;
			blendAnim->Time(blendTime + frame);
		}

		//フレームを進める
		mAnimationFrame += frame;
	}
	else
	{
		//アニメーションを繰り返す場合
		if (mAnimationLoopFlg)
		{
			//アニメーションのフレーム数をループさせる
			float maxTime = currAnim->MaxTime();
			if (maxTime > 0.0f)
			{
				mAnimationFrame = fmodf(mAnimationFrame + maxTime, maxTime);
			}
			else
			{
				mAnimationFrame = 0.0f;
			}
			currAnim->Time(mAnimationFrame);
		}
		//アニメーションを繰り返さない場合
		else
		{
			//最終フレームに設定
			currAnim->TimeProgress(mAnimationFrame >= 0.0f ? 1.0f : 0.0f);
		}
	}
	//フレームの変換行列をアニメーションで更新する
	mpModel->AnimateFrame();
	//フレームの合成行列を計算する
	mpModel->Frames()[0]->AnimateCombined(&matrix);
	//合成行列の退避
	for (size_t i = 0; i < mpModel->Frames().size(); i++)
	{
		mpCombinedMatrix[i] =
			mpModel->Frames()[i]->CombinedMatrix();
	}
}

//更新処理
void CXCharacter::Update()
{
	//アニメーションを更新する
	Update(Matrix());
#if _DEBUG
	CDebugPrint::Print("AnimationIndex:%d\n", mAnimationIndex);
	CDebugPrint::Print("BlendIndex:%d\n", mBlendIndex);
	CDebugPrint::Print("ElapsedBlend:%f\n", mElapsedBlend);
#endif
}

/*
Render
描画する
*/
void CXCharacter::Render()
{
	if (mpModel == nullptr) return;
	mpModel->SetColor(mColor);
	mpModel->RenderShader(mpCombinedMatrix);
}

bool CXCharacter::IsAnimationFinished()
{
	return mAnimationFrame >= mAnimationFrameSize;
}

int CXCharacter::AnimationIndex()
{
	return mAnimationIndex;
}

// 再生中のアニメーションフレームを取得
float CXCharacter::GetAnimationFrame() const
{
	return mAnimationFrame;
}

// 再生中のアニメーションの進行度を取得
float CXCharacter::GetAnimationFrameRatio() const
{
	if (mAnimationFrameSize == 0.0f) return 0.0f;
	return Math::Clamp01(mAnimationFrame / mAnimationFrameSize);
}

// 指定したボーンの行列を取得
const CMatrix* CXCharacter::GetFrameMtx(std::string name) const
{
	// モデルデータが設定されていない
	if (mpModel == nullptr) return nullptr;

	// フレーム検索
	CModelXFrame* frame = mpModel->FinedFrame(name.c_str());

	//指定されたフレームが存在しなかった
	if (frame == nullptr) return nullptr;

	// フレームの行列祖返す
	return &frame->CombinedMatrix();
}

// アニメーションの再生速度を設定
void CXCharacter::SetAnimationSpeed(float speed)
{
	mAnimationSpeed = speed;
}

// アニメーションの再生速度を取得
float CXCharacter::GetAnimationSpeed() const
{
	return mAnimationSpeed;
}
