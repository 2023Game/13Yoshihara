#pragma once
#include "CModel.h"
class CCar;
class CGarbageTruck;

class CVehicleManager
{
public:
	CVehicleManager();
	~CVehicleManager();
	
	void Update();
	// �g�p����Ԃƃg���b�N��S�Đ���
	void CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck);
	// �X�e�[�W�͈͊O�̎ԁA�g���b�N�̍X�V�A�`����~�߂�
	void HideVehicle();
	// ��蕨���o��������
	void SpawnVehicle();

	// �Ԃ̏o���܂ł̎��Ԃ��J�E���g
	void CountCarPopTime();

	// �g���b�N�̏o���܂ł̎��Ԃ��J�E���g
	void CountTruckPopTime();

	// ���d�u���g���b�N�̏o���܂ł̎��Ԃ��J�E���g
	void CountBlackTruckPopTime();

private:
	std::vector<CCar*> mCars;		// �Ԃ̃��X�g
	CGarbageTruck* mpGarbageTruck;	// �ʏ�g���b�N
	CGarbageTruck* mpBlackTruck;	// ���d�u���g���b�N

	// �o���܂ł̎���
	float mCarPopTime;			// ��
	float mTruckPopTime;		// �g���b�N
	float mBlackTruckPopTime;	// ���d�u���g���b�N
};