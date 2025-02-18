#include "CHomeUIBase.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CMoneyManager.h"

// �������ʂɂ���ĕς���w�i�̃p�X
#define POOR_PATH	"UI/text_poor_bg.png"
#define NORMAL_PATH	"UI/text_normal_bg.png"
#define RICH_PATH	"UI/text_rich_bg.png"

// �w�i�̃A���t�@
#define BG_ALPHA 0.75f

// �R���X�g���N�^
CHomeUIBase::CHomeUIBase()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mBgPath(POOR_PATH)
	, mpTextUI(nullptr)
	, mpBackground(nullptr)
{
	// �e�L�X�gUI�𐶐�
	mpTextUI = new CTextUI2D(ETaskPauseType::eGame, false, nullptr);
	// �����̑����̊��ݒ�
	mpTextUI->SetFontAligment(FTGL::TextAlignment::ALIGN_LEFT);

	// �w�i�̃p�X��ݒ�
	SetBgPath();
	// �w�i�𐶐�
	mpBackground = new CImage
	(
		mBgPath.c_str(),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	// �A���t�@��ݒ�
	mpBackground->SetAlpha(BG_ALPHA);
}

// �f�X�g���N�^
CHomeUIBase::~CHomeUIBase()
{
	SAFE_DELETE(mpTextUI);
	SAFE_DELETE(mpBackground);
}

// �X�V
void CHomeUIBase::Update()
{
	// �w�i�摜��ݒ�
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

// �`��
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

// �������̗ʂɂ���Ĕw�i��ݒ�
void CHomeUIBase::SetBgPath()
{
	// ���������擾
	auto* moneyMgr = CMoneyManager::Instance();
	int money = moneyMgr->GetMoney();
	// �����̐��l�𒴂��Ȃ���Εn�R�w�i
	std::string path = POOR_PATH;
	// �x�T�̐��l��葽���Ȃ�
	if (money > RICH_MONEY)
	{
		// �x�T�w�i
		path = RICH_PATH;
	}
	// �n�R�̐��l��葽���Ȃ�
	else if (money > POOR_MONEY)
	{
		// �ʏ�w�i
		path = NORMAL_PATH;
	}
	// �摜�̃p�X���Ⴄ�Ȃ�
	if (mBgPath != path &&
		mpBackground != nullptr)
	{
		// �T�C�Y��ۑ�
		CVector2 size = mpBackground->GetSize();
		// �w�i�̉摜��ݒ�
		mpBackground->Load(path.c_str());
		// �T�C�Y��ݒ�
		mpBackground->SetSize(size);
		mBgPath = path;
	}
}
