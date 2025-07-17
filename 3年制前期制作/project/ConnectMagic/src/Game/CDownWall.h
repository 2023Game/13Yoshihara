#pragma once
#include "CSwitchObject.h"

// �������Ă�����
class CDownWall : public CSwitchObject
{
public:
	// �R���X�g���N�^
	CDownWall(CVector defaultPos);
	// �f�X�g���N�^
	~CDownWall();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ��p���Ă��Ȃ����̏���
	void UpdateOff() override;
	// ��p���Ă��鎞�̏���
	void UpdateOn() override;

	// �f�t�H���g��Y���W
	float mDefaultPosY;
};