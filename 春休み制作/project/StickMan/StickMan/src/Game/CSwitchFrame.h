#pragma once
#include "CObjectBase.h"

class CSwitch;
class CModel;

// �X�C�b�`�̃t���[������
class CSwitchFrame : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSwitchFrame(CVector pos, CSwitch* owner);
	// �f�X�g���N�^
	~CSwitchFrame();

	// �`��
	void Render() override;

private:
	CSwitch* mpOwner;	// ������

	CModel* mpModel;
};