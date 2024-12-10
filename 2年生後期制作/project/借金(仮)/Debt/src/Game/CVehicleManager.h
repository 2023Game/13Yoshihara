#pragma once
#include "CModel.h"
#include "CColliderMesh.h"
#include "CTrashVehicleSpawnZone.h"

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

	// �����_���Ŏԗ����o��������ꏊ�����߂�
	CVector RandomDecidePopPosition();
	// �w�肵��X���W(��)�ɑ��̎ԗ������邩�`�F�b�N����
	bool IsVehicle(float xPos);
private:
	//CTrashVehicleSpawnZone mSpawnZone;	// �����ꏊ�ɎԂ����邩���f����N���X

	std::vector<CCar*> mCars;		// �Ԃ̃��X�g
	CGarbageTruck* mpGarbageTruck;	// �ʏ�g���b�N
	CGarbageTruck* mpBlackTruck;	// ���d�u���g���b�N

	// �o���܂ł̎���
	float mCarPopTime;			// ��
	float mTruckPopTime;		// �g���b�N
	float mBlackTruckPopTime;	// ���d�u���g���b�N
};