#pragma once

// �z�B���̃X�e�[�^�X�N���X
class CDeliveryItemStatus
{
public:
	// �R���X�g���N�^
	CDeliveryItemStatus();
	// �f�X�g���N�^
	~CDeliveryItemStatus();

	// �������̓��_���擾
	int GetScore() const;
	// �^����_���[�W���擾
	int GetDamage() const;

private:
	// �������̓��_
	int mScore;
	// �^����_���[�W
	int mDamage;
};