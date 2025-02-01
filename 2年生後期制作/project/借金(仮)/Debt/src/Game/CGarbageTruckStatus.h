#pragma once
#include "CVehicleStatus.h"

// �S�~���W�Ԃ̃X�e�[�^�X
class CGarbageTruckStatus : public CVehicleStatus
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="punisher">true�Ȃ�A���d�u���p</param>
	CGarbageTruckStatus(bool punisher);
	// �f�X�g���N�^
	~CGarbageTruckStatus();

	// �P�ނ܂ł̎��Ԃ��擾
	float GetReturnTime() const;
	// �P�ނ܂ł̎��Ԃ������l�ɐݒ�
	void SetReturnTime();
	// �P�ނ܂ł̎��Ԃ��o�߂������ǂ���
	bool IsElapsedReturnTime() const;
	// �P�ނ܂ł̎��Ԃ��J�E���g�_�E��
	void CountReturnTime();

	// ������̐l�����擾����
	int GetCollectorsNum() const;
	// ������̐l���������l�ɐݒ肷��
	void SetCollectorsNum();
	// ������̐l����1���炷
	void DecreaseCollectors();

	// �S�~�܂̐��̏����l���擾����
	int GetDefaultBagNum() const;
	// �S�[���h�S�~�܂̐��̏����l���擾����
	int GetDefaultGoldBagNum() const;

	// ���d�u���p���ǂ������擾
	bool GetPunisher() const;
private:
	float mReturnTime;		// �P�ނ܂ł̎���
	int mCollectorsNum;		// ������̐l��
	bool mIsPunisher;		// ���d�u���p���ǂ���
	int mDefaultBagNum;		// �S�~�܂̐��̏����l
	int mDefaultGoldBagNum;	// �S�[���h�S�~�܂̐��̏����l
};