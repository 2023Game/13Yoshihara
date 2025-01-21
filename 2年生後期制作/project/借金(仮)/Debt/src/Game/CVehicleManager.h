#pragma once
#include "CModel.h"
#include "CColliderMesh.h"
#include "CVehicleBase.h"

class CCar;
class CGarbageTruck;
class CNavNode;

// �ʒu
#define VEHICLE_LEFT_POS1  CVector(-300.0f,0.0f, 500.0f)	// ������1��
#define VEHICLE_LEFT_POS2  CVector( 300.0f,0.0f, 580.0f)	// ������2��
#define VEHICLE_RIGHT_POS1 CVector( 300.0f,0.0f,-500.0f)	// �E����1��
#define VEHICLE_RIGHT_POS2 CVector(-300.0f,0.0f,-580.0f)	// �E����2��

// ��]
#define VEHICLE_LEFT_ROTATION  CVector(0.0f,270.0f,0.0f)	// �E����
#define VEHICLE_RIGHT_ROTATION CVector(0.0f, 90.0f,0.0f)	// ������

// �ԗ��Ǘ��N���X
class CVehicleManager
{
public:
	// �ԗ��Ǘ��N���X�̃C���X�^���X���擾
	static CVehicleManager* Instance();
	// �R���X�g���N�^
	CVehicleManager();
	// �f�X�g���N�^
	~CVehicleManager();

	/// <summary>
	/// ���C�ƑS�Ă̎ԗ��Ƃ̏Փ˔���
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <param name="alreadyHit">���ɏՓ˂��Ă��邩</param>
	/// <returns>�Փ˂��Ă�����Atrue��Ԃ�</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit, bool alreadyHit);

	/// <summary>
	/// ���C�ƑS�Ă̎ԗ��Ƃ̏Փ˔���i�o�H�T���p�j
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <param name="alreadyHit">���ɏՓ˂��Ă��邩</param>
	/// <returns>�Փ˂��Ă�����Atrue��Ԃ�</returns>
	bool NavCollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit, bool alreadyHit);
	
	// �X�V
	void Update();

	/// <summary>
	/// �w�肵�����ɐ����ł��邩
	/// </summary>
	/// <param name="roadType">�ǂ̓��ɂ��邩</param>
	/// <returns>true�Ȃ�΁A�����ł���</returns>
	bool IsSpawn(CVehicleBase::ERoadType roadType);

	/// <summary>
	/// ����X���W���擾
	/// </summary>
	/// <param name="road">���̎w��
	/// </param>
	/// <returns></returns>
	float GetRoadPosX(CVehicleBase::ERoadType road);

	/// <summary>
	/// �w�肵�����̏���|�C���g���擾
	/// </summary>
	/// <param name="road">���̎w��
	/// </param>
	/// <returns></returns>
	std::vector<CNavNode*> GetPatrolPoints(CVehicleBase::ERoadType road);
private:
	// �g�p����g���b�N��S�Đ���
	void CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck);
	// �o�H�T���p�̃m�[�h���쐬
	void CreateNavNodes();
	
	// �X�e�[�W�͈͊O�̎ԁA�g���b�N�̍X�V�A�`����~�߂�
	void HideVehicle();
	// ��蕨���o��������
	void SpawnVehicle();

	// �Ԃ̏o���܂ł̎��Ԃ��J�E���g�_�E��
	void CountCarPopTime();
	// �g���b�N�̏o���܂ł̎��Ԃ��J�E���g�_�E��
	void CountTruckPopTime();
	// ���d�u���g���b�N�̏o���܂ł̎��Ԃ��J�E���g�_�E��
	void CountBlackTruckPopTime();

	// ���ꂼ��̓��ɏo���\�ɂȂ�܂ł��J�E���g�_�E��
	void CountLeft1CanPopTime();	// ������1�ԓ��H
	void CountLeft2CanPopTime();	// ������2�ԓ��H
	void CountRight1CanPopTime();	// �E����1�ԓ��H
	void CountRight2CanPopTime();	// �E����2�ԓ��H

	/// <summary>
	/// �����_���Ŏԗ����o��������ꏊ�����߂�
	/// </summary>
	/// <param name="roadType">�ǂ̓��ɂ��邩</param>
	/// <param name="popPos">�o������ꏊ</param>
	/// <returns>true�Ȃ�΁A�����ꏊ������</returns>
	bool RandomDecidePopPosition(CVehicleBase::ERoadType& roadType, CVector& popPos);


	// �ԗ��Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CVehicleManager* spInstance;

	CModel* mpCarModel;	// �Ԃ̃��f��

	std::list<CCar*> mCars;		// �Ԃ̃��X�g
	CGarbageTruck* mpGarbageTruck;	// �ʏ�g���b�N
	CGarbageTruck* mpPunishTruck;	// ���d�u���g���b�N

	// �o���܂ł̎���
	float mCarPopTime;			// ��
	float mTruckPopTime;		// �g���b�N
	float mPunishTruckPopTime;	// ���d�u���g���b�N

	// �����̓��ɏo���\�ɂȂ�܂ł̎���
	float mLeft1CanPopTime;	 // ������1�ԓ��H
	float mLeft2CanPopTime;  // ������2�ԓ��H
	float mRight1CanPopTime; // �E����1�ԓ��H
	float mRight2CanPopTime; // �E����2�ԓ��H

	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPointsL1;	// ������1�ԓ��H
	std::vector<CNavNode*> mPatrolPointsL2; // ������2�ԓ��H
	std::vector<CNavNode*> mPatrolPointsR1; // �E����1�ԓ��H
	std::vector<CNavNode*> mPatrolPointsR2; // �E����2�ԓ��H
};