#pragma once
#include "CCharaStatusBase.h"

class CResidentStatus : public CCharaStatusBase
{
public:
	// �R���X�g���N�^
	CResidentStatus();
	// �f�X�g���N�^
	~CResidentStatus();

	// �S�~�܂𓊂��鑬�x�������l�Ƀ����_�����l���|�����l�ɐݒ肷��
	void SetThrowSpeed();
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

	// �S�~�܂̍ő吔��ݒ�
	void SetMaxThrowNum(int maxThrowNum);
	// �S�~�܂̍ő吔���擾
	int GetMaxThrowNum() const;

	// �S�[���h�m����ݒ�
	void SetGoldChance(int goldChance);
	// �S�[���h�m�����擾
	int GetGoldChance() const;
private:
	float mThrowSpeed;	// �S�~�܂𓊂��鑬�x
	float mThrowTime;	// �S�~�܂𓊂���܂ł̎���
	int mMaxThrowNum;	// �t�B�[���h��ɓ����ɑ��݂������S�~�܂̐�
	// �S�[���h�m��
	int mGoldChance;
};