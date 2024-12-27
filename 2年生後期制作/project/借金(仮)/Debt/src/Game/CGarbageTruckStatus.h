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
	void SetBackTime();
	// �P�ނ܂ł̎��Ԃ��o�߂������ǂ���
	bool IsElapsedBackTime() const;
	// �P�ނ܂ł̎��Ԃ��J�E���g�_�E��
	void CountBackTime();

	// ����͈͂̔��a���擾����
	float GetCollectRadius() const;

	// ������̐l�����擾����
	int GetCollectors() const;
	// ������̐l���������l�ɐݒ肷��
	void SetCollectors();
	// ������̐l����1���炷
	void DecreaseCollectors();
private:
	float mBackTime;		// �P�ނ܂ł̎���
	float mCollectRadius;	// ����͈͂̔��a
	int mCollectors;		// ������̐l��
};