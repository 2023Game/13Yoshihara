#include "CSwitch.h"
#include "CSwitchFrame.h"
#include "CSwitchButton.h"
#include "CSwitchObject.h"

// �R���X�g���N�^
CSwitch::CSwitch(CVector pos)
	: mpActionObject(nullptr)
{
	mpFrame = new CSwitchFrame(pos, this);
	mpButton = new CSwitchButton(pos, this);
}

// �R���X�g���N�^
CSwitch::~CSwitch()
{
}

// ��p����I�u�W�F�N�g��ݒ肷��
void CSwitch::SetActionObj(CSwitchObject* obj)
{
	mpActionObject = obj;
}

// ��p����I�u�W�F�N�g�ɃI���I�t��m�点��
void CSwitch::SetOnOff(bool isOnOff)
{
	if (mpActionObject == nullptr) return;

	// �I���I�t��؂�ւ���
	mpActionObject->SetOnOff(isOnOff);
}

// �X�C�b�`�̃t���[�����擾
CSwitchFrame* CSwitch::GetFrame()
{
	return mpFrame;
}

// �X�C�b�`�̃{�^�����擾
CSwitchButton* CSwitch::GetButton()
{
	return mpButton;
}
