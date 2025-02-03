#include "CSelectJobUI.h"
#include "CTextUI2D.h"
#include "CJobStatusManager.h"
#include "JobType.h"

// �I�𒆂̎d���̕�����
#define TRASH		"�I�𒆁F�S�~�E��"
#define DELIVERY	"�I�𒆁F�z�B"
#define NONE		"�I�𒆁F���I��"

// �e�L�X�gUI�̃I�t�Z�b�g���W
#define TEXT_OFFSET_POS CVector(WINDOW_WIDTH * 0.725f,0.0f,0.0f)

// �R���X�g���N�^
CSelectJobUI::CSelectJobUI()
{
	// UI�𐶐�
	mpSelectJobUI = new CTextUI2D(ETaskPauseType::eMenu, true, nullptr);
	mpSelectJobUI->Position(TEXT_OFFSET_POS);
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
		mpSelectJobUI->SetStr(TRASH);
	}
	// �I�𒆂̎d�����z�B�Ȃ�
	else if (selectJob == EJobType::eDelivery)
	{
		// �`�悷�镶�����ݒ�
		mpSelectJobUI->SetStr(DELIVERY);
	}
	// �I������Ă��Ȃ��Ȃ�
	else
	{
		// �`�悷�镶�����ݒ�
		mpSelectJobUI->SetStr(NONE);
	}
}
