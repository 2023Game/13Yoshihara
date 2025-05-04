#include "CSwitchFrame.h"
#include "CSwitch.h"
#include "CModel.h"

// �R���X�g���N�^
CSwitchFrame::CSwitchFrame(CVector pos, CSwitch* owner)
	: CObjectBase(ETag::eSwitch, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
{
	mpModel = CResourceManager::Get<CModel>("SwitchFrame");
	// ���W�ݒ�
	Position(pos);
}

// �f�X�g���N�^
CSwitchFrame::~CSwitchFrame()
{
}

// �`��
void CSwitchFrame::Render()
{
	mpModel->Render(Matrix());
}
