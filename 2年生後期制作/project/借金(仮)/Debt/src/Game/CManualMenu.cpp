#include "CManualMenu.h"
#include "CTextUI2D.h"
#include "CInput.h"

// �e�L�X�g���W
#define TEXT_POS CVector2(0.0f, WINDOW_HEIGHT * 0.925f)

// �J������̑ҋ@����
#define OPENED_WAIT_TIME 0.1f

// �T�C�Y�̔{��
#define SIZE CVector2(WINDOW_WIDTH, WINDOW_HEIGHT)

// �R���X�g���N�^
CManualMenu::CManualMenu(std::string bgPath)
	: CGameMenuBase(std::vector<std::string>{})
{
	mpCloseText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	mpCloseText->SetStr("�N���b�N�ŕ���");
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

// �f�X�g���N�^
CManualMenu::~CManualMenu()
{
	SAFE_DELETE(mpCloseText);
}

// �X�V
void CManualMenu::Update()
{
	CGameMenuBase::Update();

	mpCloseText->Update();
}

// �`��
void CManualMenu::Render()
{
	CGameMenuBase::Render();

	mpCloseText->Render();
}

// ���j���[���J��
void CManualMenu::UpdateOpen()
{
	// �ҋ@
	if (mElapsedTime < OPENED_WAIT_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	// �ҋ@���Ԃ��I��������
	else
	{
		// ���N���b�N����ƁA���j���[�����
		if (CInput::PushKey(VK_LBUTTON))
		{
			Close();
			// ��O�̃��j���[������Ȃ�
			if (mpPreMenu != nullptr)
			{
				mpPreMenu->Open();
			}
		}
	}
}
