#include "CSelectJobUI.h"
#include "CTextUI2D.h"
#include "CJobStatusManager.h"
#include "JobType.h"
#include "CImage.h"

// �I�𒆂̎d���̕�����
#define TRASH		"�I�𒆁F�S�~�E��"
#define DELIVERY	"�I�𒆁F�z�B"
#define NONE		"�I�𒆁F���I��"

// �e�L�X�gUI�̃I�t�Z�b�g���W
#define TEXT_OFFSET_POS CVector2(WINDOW_WIDTH * 0.7f, 50.0f)

// �R���X�g���N�^
CSelectJobUI::CSelectJobUI()
	: CHomeUIBase()
{
	// �e�L�X�g�̍��W��ݒ�
	mpTextUI->Position(TEXT_OFFSET_POS);

	// �w�i�폜
	SAFE_DELETE(mpBackground);
}

// �f�X�g���N�^
CSelectJobUI::~CSelectJobUI()
{
}

// �X�V
void CSelectJobUI::Update()
{
	// �I�𒆂̎d�����擾
	EJobType selectJob = CJobStatusManager::Instance()->GetSelectJob();
	// �I�𒆂̎d�����S�~�E���Ȃ�
	if (selectJob == EJobType::eTrash)
	{
		// �`�悷�镶�����ݒ�
		mpTextUI->SetStr(TRASH);
	}
	// �I�𒆂̎d�����z�B�Ȃ�
	else if (selectJob == EJobType::eDelivery)
	{
		// �`�悷�镶�����ݒ�
		mpTextUI->SetStr(DELIVERY);
	}
	// �I������Ă��Ȃ��Ȃ�
	else
	{
		// �`�悷�镶�����ݒ�
		mpTextUI->SetStr(NONE);
	}

	CHomeUIBase::Update();
}

// �`��
void CSelectJobUI::Render()
{
	CHomeUIBase::Render();
}
