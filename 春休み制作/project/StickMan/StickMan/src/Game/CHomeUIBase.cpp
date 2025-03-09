#include "CHomeUIBase.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CMoneyManager.h"

// ŠŽ‹à—Ê‚É‚æ‚Á‚Ä•Ï‚¦‚é”wŒi‚ÌƒpƒX
#define POOR_PATH	"UI/text_poor_bg.png"
#define NORMAL_PATH	"UI/text_normal_bg.png"
#define RICH_PATH	"UI/text_rich_bg.png"

// ”wŒi‚ÌƒAƒ‹ƒtƒ@
#define BG_ALPHA 0.75f

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
CHomeUIBase::CHomeUIBase()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mBgPath(POOR_PATH)
	, mpTextUI(nullptr)
	, mpBackground(nullptr)
{
	// ƒeƒLƒXƒgUI‚ð¶¬
	mpTextUI = new CTextUI2D(ETaskPauseType::eGame, false, nullptr);
	// •¶Žš‚Ì‘µ‚¢‚ÌŠî€‚ðÝ’è
	mpTextUI->SetFontAligment(FTGL::TextAlignment::ALIGN_LEFT);

	// ”wŒi‚ÌƒpƒX‚ðÝ’è
	SetBgPath();
	// ”wŒi‚ð¶¬
	mpBackground = new CImage
	(
		mBgPath.c_str(),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	// ƒAƒ‹ƒtƒ@‚ðÝ’è
	mpBackground->SetAlpha(BG_ALPHA);
}

// ƒfƒXƒgƒ‰ƒNƒ^
CHomeUIBase::~CHomeUIBase()
{
	SAFE_DELETE(mpTextUI);
	SAFE_DELETE(mpBackground);
}

// XV
void CHomeUIBase::Update()
{
	// ”wŒi‰æ‘œ‚ðÝ’è
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

// •`‰æ
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

// ŠŽ‹à‚Ì—Ê‚É‚æ‚Á‚Ä”wŒi‚ðÝ’è
void CHomeUIBase::SetBgPath()
{
	// ŠŽ‹à‚ðŽæ“¾
	auto* moneyMgr = CMoneyManager::Instance();
	int money = moneyMgr->GetMoney();
	// ðŒ‚Ì”’l‚ð’´‚¦‚È‚¯‚ê‚Î•n–R”wŒi
	std::string path = POOR_PATH;
	// •x—T‚Ì”’l‚æ‚è‘½‚¢‚È‚ç
	if (money > RICH_MONEY)
	{
		// •x—T”wŒi
		path = RICH_PATH;
	}
	// •n–R‚Ì”’l‚æ‚è‘½‚¢‚È‚ç
	else if (money > POOR_MONEY)
	{
		// ’Êí”wŒi
		path = NORMAL_PATH;
	}
	// ‰æ‘œ‚ÌƒpƒX‚ªˆá‚¤‚È‚ç
	if (mBgPath != path &&
		mpBackground != nullptr)
	{
		// ƒTƒCƒY‚ð•Û‘¶
		CVector2 size = mpBackground->GetSize();
		// ”wŒi‚Ì‰æ‘œ‚ðÝ’è
		mpBackground->Load(path.c_str());
		// ƒTƒCƒY‚ðÝ’è
		mpBackground->SetSize(size);
		mBgPath = path;
	}
}
