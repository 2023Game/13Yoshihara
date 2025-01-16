#pragma once
#include "CVehicleStatus.h"

// �S�~���W�Ԃ̃X�e�[�^�X
class CGarbageTruckStatus : public CVehicleStatus
{
public:
	// �R���X�g���N�^
	CGarbageTruckStatus();
	// �f�X�g���N�^
	~CGarbageTruckStatus();

	// �P�ނ܂ł̎��Ԃ��擾
	float GetBackTime() const;
	// �P�ނ܂ł̎��Ԃ������l�ɐݒ�
	void SetWithdrawTime();
	// �P�ނ܂ł̎��Ԃ��o�߂������ǂ���
	bool IsElapsedWithdrawTime() const;
	// �P�ނ܂ł̎��Ԃ��J�E���g�_�E��
	void CountWithdrawTime();


	// ������̐l�����擾����
	int GetCollectors() const;
	// ������̐l���������l�ɐݒ肷��
	void SetCollectors();
	// ������̐l����1���炷
	void DecreaseCollectors();
private:
	float mWithdrawTime;		// �P�ނ܂ł̎���
	int mCollectors;		// ������̐l��
};