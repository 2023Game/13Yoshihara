#include "CHomeUIBase.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CMoneyManager.h"

// 所持金量によって変える背景のパス
#define POOR_PATH	"UI/text_poor_bg.png"
#define NORMAL_PATH	"UI/text_normal_bg.png"
#define RICH_PATH	"UI/text_rich_bg.png"

// 背景のアルファ
#define BG_ALPHA 0.75f

// コンストラクタ
CHomeUIBase::CHomeUIBase()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mBgPath(POOR_PATH)
	, mpTextUI(nullptr)
	, mpBackground(nullptr)
{
	// テキストUIを生成
	mpTextUI = new CTextUI2D(ETaskPauseType::eGame, false, nullptr);
	// 文字の揃いの基準を設定
	mpTextUI->SetFontAligment(FTGL::TextAlignment::ALIGN_LEFT);

	// 背景のパスを設定
	SetBgPath();
	// 背景を生成
	mpBackground = new CImage
	(
		mBgPath.c_str(),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	// アルファを設定
	mpBackground->SetAlpha(BG_ALPHA);
}

// デストラクタ
CHomeUIBase::~CHomeUIBase()
{
	SAFE_DELETE(mpTextUI);
	SAFE_DELETE(mpBackground);
}

// 更新
void CHomeUIBase::Update()
{
	// 背景画像を設定
	SetBgPath();

	if (mpTextUI != nullptr)
	{
		mpTextUI->Update();
	}
	if (mpBackground != nullptr)
	{
		mpBackground->Update();
	}
}

// 描画
void CHomeUIBase::Render()
{
	if (mpBackground != nullptr)
	{
		mpBackground->Render();
	}
	if (mpTextUI != nullptr)
	{
		mpTextUI->Render();
	}
}

// 所持金の量によって背景を設定
void CHomeUIBase::SetBgPath()
{
	// 所持金を取得
	auto* moneyMgr = CMoneyManager::Instance();
	int money = moneyMgr->GetMoney();
	// 条件の数値を超えなければ貧乏背景
	std::string path = POOR_PATH;
	// 富裕の数値より多いなら
	if (money > RICH_MONEY)
	{
		// 富裕背景
		path = RICH_PATH;
	}
	// 貧乏の数値より多いなら
	else if (money > POOR_MONEY)
	{
		// 通常背景
		path = NORMAL_PATH;
	}
	// 画像のパスが違うなら
	if (mBgPath != path &&
		mpBackground != nullptr)
	{
		// サイズを保存
		CVector2 size = mpBackground->GetSize();
		// 背景の画像を設定
		mpBackground->Load(path.c_str());
		// サイズを設定
		mpBackground->SetSize(size);
		mBgPath = path;
	}
}
