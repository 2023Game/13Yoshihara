#include "CManualMenu.h"
#include "CTextUI2D.h"
#include "CInput.h"

// テキスト座標
#define TEXT_POS CVector2(0.0f, WINDOW_HEIGHT * 0.925f)

// 開いた後の待機時間
#define OPENED_WAIT_TIME 0.1f

// サイズの倍率
#define SIZE CVector2(WINDOW_WIDTH, WINDOW_HEIGHT)

// コンストラクタ
CManualMenu::CManualMenu(std::string bgPath)
	: CGameMenuBase(std::vector<std::string>{})
{
	mpCloseText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	mpCloseText->SetStr("クリックで閉じる");
	mpCloseText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpCloseText->Position(TEXT_POS);
	mpCloseText->SetBlink(true);
	mpCloseText->SetFontSize(30);
	mpCloseText->SetFontColor(CColor(1.0f, 1.0f, 0.5f));
	mpCloseText->SetShadow(true, CColor(0.25f, 0.125f, 0.0f));
	mpCloseText->SetOutline(true, CColor(0.0f, 0.0f, 0.0f));

	mpBackground->Load(bgPath.c_str());
	mpBackground->SetSize(SIZE);
	mpBackground->SetPos(SIZE * 0.4f);
	mpBackground->SetAlpha(1.0f);
}

// デストラクタ
CManualMenu::~CManualMenu()
{
	SAFE_DELETE(mpCloseText);
}

// 更新
void CManualMenu::Update()
{
	CGameMenuBase::Update();

	mpCloseText->Update();
}

// 描画
void CManualMenu::Render()
{
	CGameMenuBase::Render();

	mpCloseText->Render();
}

// メニューを開く
void CManualMenu::UpdateOpen()
{
	// 待機
	if (mElapsedTime < OPENED_WAIT_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	// 待機時間が終了したら
	else
	{
		// 左クリックすると、メニューを閉じる
		if (CInput::PushKey(VK_LBUTTON))
		{
			Close();
			// 一つ前のメニューがあるなら
			if (mpPreMenu != nullptr)
			{
				mpPreMenu->Open();
			}
		}
	}
}
