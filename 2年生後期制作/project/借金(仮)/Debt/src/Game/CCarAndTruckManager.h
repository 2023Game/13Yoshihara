#pragma once
#include "CModel.h"
class CCar;
class CGarbageTruck;

class CCarAndTruckManager
{
public:
	CCarAndTruckManager();
	~CCarAndTruckManager();
	
	void Update();
	// �g�p����Ԃƃg���b�N��S�Đ���
	void CreateCarAndTruck(CModel* car, CModel* garbageTruck);
	// �X�e�[�W�͈͊O�̎ԁA�g���b�N�̍X�V�A�`����~�߂�
	void HideCarAndTruck();
private:
	std::vector<CCar*> mCars;	// �Ԃ̃��X�g
	CGarbageTruck* mpGarbageTruck;// �ʏ�g���b�N
	CGarbageTruck* mpBlackTruck;	// ���d�u���g���b�N
};