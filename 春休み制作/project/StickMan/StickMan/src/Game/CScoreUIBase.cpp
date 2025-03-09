#include "CScoreUIBase.h"
#include "CTextUI2D.h"

// �X�R�AUI�̃I�t�Z�b�g���W
#define SCORE_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.35f,WINDOW_HEIGHT - 100.0f,0.0f)

// �X�R�A�\��UI�̃C���X�^���X
CScoreUIBase* CScoreUIBase::spInstance = nullptr;

// �X�R�A�\��UI�̃C���X�^���X���擾
CScoreUIBase* CScoreUIBase::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CScoreUIBase::CScoreUIBase()
{
	// �C���X�^���X��ݒ�
	spInstance = this;
	// �X�R�A��UI�𐶐�
	mpScoreUI = new CTextUI2D();
	// �����̑����̊��ݒ�
	mpScoreUI->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// ���W��ݒ�
	mpScoreUI->Position(SCORE_UI_OFFSET_POS);
}

// �f�X�g���N�^
CScoreUIBase::~CScoreUIBase()
{
	SAFE_DELETE(spInstance);
}

// �X�V
void CScoreUIBase::Update()
{
	// �`�悷�镶�����ݒ�
	mpScoreUI->ChangeToStr("Score�F%d\n", GetScore());
}
