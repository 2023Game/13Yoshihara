#include "CFade.h"

// フェードクラスのインスタンスdeskNum
CFade* CFade::ms_instance = nullptr;

const char* const BLACKOUT_PATH =	"UI/white.png";
const char* const VORTEX_PATH =		"Effect/Portal.png";

// 画像の回転速度
constexpr float ROT_SPEED = 2000.0f;

// コンストラクタ
CFade::CFade(EFadeType type)
	: CTask(ETaskPriority::eFade, 0, ETaskPauseType::eSystem, true, true)
	, mFadeColor(CColor::black)
	, mFadeTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsFadeIn(false)
	, mIsFading(false)
	, mType(type)
{
	std::string path;
	switch (type)
	{
		// 暗転するフェードの場合
	case EFadeType::eBlackOut:
		path = BLACKOUT_PATH;
		break;

		// 渦状フェードの場合
	case EFadeType::eVortex:
		path = VORTEX_PATH;
		break;
	}
	// フェード画像を読み込み
	mpFadeImage = new CImage
	(
		path.c_str(),
		ETaskPriority::eFade, 0,
		ETaskPauseType::eSystem,
		true, false // フェード画像はタスクリストに登録しない
	);
	// フェードの種類を変更
	// サイズやカラー等の変更のために処理
	ChangeFadeType(type);
}

// フェードの種類を変更する
void CFade::ChangeFadeType(EFadeType type)
{
	// 指定の種類のフェードの初期設定処理をする
	switch (type)
	{
		// 暗転するフェードの場合
	case EFadeType::eBlackOut:	InitBlackOut();	break;
		// 渦状のフェードの場合
	case EFadeType::eVortex:	InitVortex();	break;
	}

	// フェードの種類を変更
	mType = type;
}

// 暗転フェードの初期設定
void CFade::InitBlackOut()
{		
	// 暗転するフェードでないなら
	if (mType != EFadeType::eBlackOut)
	{
		// 画像を読み込む
		mpFadeImage->Load(BLACKOUT_PATH, true);
	}
	// フェード画像を画面全体のサイズに変更
	mpFadeImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// フェードカラーを設定
	mpFadeImage->SetColor(mFadeColor);
}

// 渦状フェードの初期設定
void CFade::InitVortex()
{		
	// 渦状のフェードでないなら
	if (mType != EFadeType::eVortex)
	{
		// 画像を読み込む
		mpFadeImage->Load(VORTEX_PATH, true);
	}
	// フェード画像を画面幅の2倍のサイズに変更
	float size = WINDOW_WIDTH * 2.0f;
	mpFadeImage->SetSize(size, size);
	// フェード画像の位置を真ん中に設定
	CVector2 pos = CVector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
	mpFadeImage->SetPos(pos - CVector2(size / 2.0f, size / 2.0f));
}

// デストラクタ
CFade::~CFade()
{
	// フェード画像をタスクリストに登録していないので、
	// デストラクタで自分で破棄する
	SAFE_DELETE(mpFadeImage);

	ms_instance = nullptr;
}

// インスタンスを取得
CFade* CFade::Instance(EFadeType type)
{
	if (ms_instance == nullptr)
	{
		ms_instance = new CFade(type);
	}
	return ms_instance;
}

// フェードカラーを設定
void CFade::SetFadeColor(const CColor& color)
{
	CFade* inst = Instance();
	inst->mFadeColor.Set(color.R(), color.G(), color.B());
	inst->mpFadeImage->SetColor(inst->mFadeColor);
}

// フェードイン開始
void CFade::FadeIn(float time)
{
	Instance()->StartFade(time, true);
}

// フェードアウト開始
void CFade::FadeOut(float time)
{
	Instance()->StartFade(time, false);
}

// フェード開始
void CFade::StartFade(float time, bool isFadeIn)
{
	// 各パラメータを設定
	mFadeTime = time;
	mElapsedTime = 0.0f;
	mIsFadeIn = isFadeIn;

	switch(mType)
	{
		// 暗転フェード
	case EFadeType::eBlackOut:
	{
		// フェード時間が0かどうかで初期状態のアルファ値を変更
		float alpha = mFadeTime <= 0.0f ? 1.0f : 0.0f;
		// 初期状態のアルファ値を設定
		mFadeColor.A(mIsFadeIn ? 1.0f - alpha : alpha);
		mpFadeImage->SetColor(mFadeColor);
		break;
	}

		// 渦状のフェード
	case EFadeType::eVortex:
		// 回転する
		mpFadeImage->SetRot(true);
		// 速度設定
		mpFadeImage->SetRotSpeed(mIsFadeIn ? ROT_SPEED : -ROT_SPEED);
		break;
	}

	// フェード時間が0より長い場合は、フェードフラグを立てる
	mIsFading = mFadeTime > 0.0f;
}

// フェード中かどうか
bool CFade::IsFading()
{
	return Instance()->mIsFading;
}

// 更新
void CFade::Update()
{
	// フェード中でなければ、処理しない
	if (!mIsFading) return;

	// フェード時間が経過していない
	if (mElapsedTime < mFadeTime)
	{
		// フェード中の処理
		Fade();
	}
	// フェード時間を経過した
	else
	{
		// フェード終了の処理
		FadeEnd();
	}
}

// 描画
void CFade::Render()
{
	if (mFadeColor.A() == 0.0f) return;
	mpFadeImage->Render();
}

// フェード中の処理
void CFade::Fade()
{
	// フェードが進んだ割合
	float ratio = mElapsedTime / mFadeTime;
	// フェードインかどうかで割合を決定
	ratio = (mIsFadeIn ? 1.0f - ratio : ratio);

	switch (mType)
	{
		// 暗転フェード
	case EFadeType::eBlackOut:
		// フェードイメージのアルファ値を変更
		mFadeColor.A(ratio);
		mpFadeImage->SetColor(mFadeColor);
		break;

		// 渦状のフェード
	case EFadeType::eVortex:
		// フェードイメージのサイズを変更
		float size = WINDOW_WIDTH * 2.0f * ratio;
		mpFadeImage->SetSize(size, size);
		// フェード画像の位置を真ん中に設定
		CVector2 pos = CVector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
		mpFadeImage->SetPos(pos - CVector2(size / 2.0f, size / 2.0f));
		break;
	}

	// 経過時間加算
	mElapsedTime += Times::DeltaTime();
}

// フェード終了の処理
void CFade::FadeEnd()
{
	mIsFading = false;
	mElapsedTime = mFadeTime;
	
	switch (mType)
	{
		// 暗転フェード
	case EFadeType::eBlackOut:
		mFadeColor.A(mIsFadeIn ? 0.0f : 1.0f);
		mpFadeImage->SetColor(mFadeColor);
		break;

		// 渦状のフェード
	case EFadeType::eVortex:
	{
		// フェードイメージのサイズを変更
		float size = (mIsFadeIn ? 0.0f : WINDOW_WIDTH * 2.0f);
		mpFadeImage->SetSize(size, size);
		// サイズが0ならここで終了
		if (size == 0.0f) return;
		// フェード画像の位置を真ん中に設定
		CVector2 pos = CVector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
		mpFadeImage->SetPos(pos - CVector2(size / 2.0f, size / 2.0f));
		break;
	}
	}
}
