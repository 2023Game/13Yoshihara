#include "CScrollUIBase.h"
#include "CExpandButton.h"
#include "CImage.h"

// �摜�̃p�X
#define PATH "UI/white.png"

// �R���X�g���N�^
CScrollUIBase::CScrollUIBase(const CVector2& pos, const CVector2& size,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: CTask(prio, sortOrder, pause, dontDelete, addTaskList)
	, mPosition(pos)
	, mSize(size)
{
	for (int i = 0; i < 10; i++)
	{
		CExpandButton* btn = new CExpandButton
		(
			mPosition + CVector2(0.0f, 200.0f * i),
			CVector2(200.0f, 100.0f),
			ETaskPriority::eUI, 0,
			ETaskPauseType::eGame,
			false, false
		);
		// �摜��ǂݍ���
		btn->LoadButtonImage(PATH, PATH);
		mButtons.push_back(btn);
	}

	//CImage* img = new CImage(
	//	"UI/white.png"
	//);
	//img->SetPos(pos - size * 0.5f);
	//img->SetSize(size);
}

// �f�X�g���N�^
CScrollUIBase::~CScrollUIBase()
{
	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// �X�V
void CScrollUIBase::Update()
{
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// �`��
void CScrollUIBase::Render()
{
	// �N���b�s���O�̈��ݒ�
	glEnable(GL_SCISSOR_TEST);
	glScissor(
		mPosition.X() - mSize.X() * 0.5f,
		mPosition.Y() - mSize.Y() * 0.5f,
		mSize.X(), mSize.Y());

	// �{�^���`��
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}

	glDisable(GL_SCISSOR_TEST);
}
