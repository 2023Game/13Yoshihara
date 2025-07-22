#pragma once

class CSwitchFrame;
class CSwitchButton;
class CSwitchObject;

// �X�C�b�`�̃N���X
class CSwitch
{
public:
	// �R���X�g���N�^
	CSwitch(CVector pos);
	// �f�X�g���N�^
	~CSwitch();

	// ��p����I�u�W�F�N�g��ݒ肷��
	void SetActionObj(CSwitchObject* obj);

	// ��p����I�u�W�F�N�g�ɃI���I�t��m�点��
	void SetOnOff(bool isOnOff);
	// �X�C�b�`���쓮�������擾
	bool GetOnOff() const;

	// �X�C�b�`�̃t���[�����擾
	CSwitchFrame* GetFrame();
	// �X�C�b�`�̃{�^�����擾
	CSwitchButton* GetButton();

private:
	CSwitchFrame* mpFrame;	// �t���[��
	CSwitchButton* mpButton;// �{�^��

	CSwitchObject* mpActionObject;	// ��p����I�u�W�F�N�g

	// �X�C�b�`���쓮���Ă��邩
	bool mIsOn;
};