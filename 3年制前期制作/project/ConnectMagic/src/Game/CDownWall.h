#pragma once
#include "CSwitchObject.h"

class CSwitch;

// �������Ă�����
class CDownWall : public CSwitchObject
{
public:
	// �R���X�g���N�^
	CDownWall(CVector defaultPos, std::vector<CSwitch*> switchs);
	// �f�X�g���N�^
	~CDownWall();

	// �I���I�t��؂�ւ���
	void SetOnOff(bool isOnOff) override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ��p���Ă��Ȃ����̏���
	void UpdateOff() override;
	// ��p���Ă��鎞�̏���
	void UpdateOn() override;

	// �f�t�H���g��Y���W
	float mDefaultPosY;

	// ��p����X�C�b�`
	std::vector<CSwitch*> mSwitchs;
	// �I���̃X�C�b�`�̐�
	int mOnSwitchNum;
};