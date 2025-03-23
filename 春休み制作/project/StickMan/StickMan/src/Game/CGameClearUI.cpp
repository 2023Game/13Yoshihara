#include "CGameClearUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// ���j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// ���j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.5f

// �{�^���̃I�t�Z�b�g���W
#define BUTTON_OFFSET_POS1 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.5f)
// �{�^���̃T�C�Y
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// �w�i�摜�̃p�X
#define BG_PATH "UI/game_clear_bg.png"

// �R���X�g���N�^
CGameClearUI::CGameClearUI()
	: CEndUIBase(BG_PATH)
{
	// �e�L�X�g��ݒ�
	mpText->SetStr("�Q�[���N���A");

	// [�I��]�{�^���𐶐�
	CExpandButton* btn = new CExpandButton
	(
		BUTTON_OFFSET_POS1,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn->LoadButtonImage("UI/title_quit0.png", "UI/title_quit1.png");
	btn->SetOnClickFunc(std::bind(&CGameClearUI::OnClickQuit, this));
	btn->SetEnable(false);
	btn->SetScale(0.0f);
	mButtons.push_back(btn);
}

// �f�X�g���N�^
CGameClearUI::~CGameClearUI()
{
}

// �Q�[�����I�����邩
bool CGameClearUI::IsExitGame() const
{
	// �I�����ڂ�1�ڂȂ�΁A�Q�[���I��
	return mSelectIndex == 0;
}

// �X�V
void CGameClearUI::Update()
{
	CEndUIBase::Update();
}

// �`��
void CGameClearUI::Render()
{
	CEndUIBase::Render();
}

// [�I��]�N���b�N���̃R�[���o�b�N�֐�
void CGameClearUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}
