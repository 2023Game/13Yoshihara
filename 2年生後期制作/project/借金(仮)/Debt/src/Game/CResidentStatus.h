#pragma once
#include "CCharaStatusBase.h"

class CResidentStatus : public CCharaStatusBase
{
public:
	// �R���X�g���N�^
	CResidentStatus();
	// �f�X�g���N�^
	~CResidentStatus();

	// �S�~�܂𓊂��鑬�x��ݒ肷��
	void SetThrowSpeed(float throwSpeed);
	// �S�~�܂𓊂��鑬�x���擾����
	float GetThrowSpeed() const;

	// �S�~�𓊂���܂ł̎��Ԃ��擾
	float GetThrowTime() const;
	// �S�~�𓊂���܂ł̎��Ԃ������l�Ƀ����_�����l���|�������ɐݒ�
	void SetThrowTime();
	// �S�~�𓊂���܂ł̎��Ԃ��o�߂������ǂ���
	bool IsElapsedThrowTime() const;
	// �S�~�𓊂���܂ł̎��Ԃ��J�E���g�_�E��
	void CountThrowTime();
private:
	float mThrowSpeed;	// �S�~�܂𓊂��鑬�x
	float mThrowTime;	// �S�~�܂𓊂���܂ł̎���
};