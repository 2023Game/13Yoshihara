#pragma once
#include "CModel.h"
#include "CColliderMesh.h"
#include "CVehicleBase.h"

class CCar;
class CGarbageTruck;
class CTrashVehicleSpawnZone;

// �ʒu
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,300.0f)	// ������1��
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,300.0f)	// ������2��
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-300.0f)	// �E����1��
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-300.0f)	// �E����2��

// ��]
#define CAR_LEFT_ROTATION CVector(0.0f,180.0f,0.0f)	// �����H�̕���
#define CAR_RIGHT_ROTATION CVector(0.0f,0.0f,0.0f)	// �E���H�̕���

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
	// �g�p����g���b�N��S�Đ���
	void CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck);
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

	// �����_���Ŏԗ����o��������ꏊ�����߂�

	/// <summary>
	/// �����_���Ŏԗ����o��������ꏊ�����߂�
	/// </summary>
	/// <param name="roadType">�ǂ̓��ɂ��邩</param>
	/// <param name="popPos">�o������ꏊ</param>
	void RandomDecidePopPosition(CVehicleBase::ERoadType& roadType, CVector& popPos);

	/// <summary>
	/// �w�肵�����ɒ�~���Ă���ԗ������邩�`�F�b�N����
	/// </summary>
	/// <param name="roadType">�ǂ̓��ɂ��邩</param>
	/// <returns>true�Ȃ�΁A����</returns>
	bool IsVehicle(CVehicleBase::ERoadType roadType);
	/// <summary>
	/// �w�肵�����̐����ꏊ�Ɏԗ������邩�`�F�b�N����
	/// </summary>
	/// <param name="roadType">�ǂ̓��ɂ��邩</param>
	/// <returns>true�Ȃ�΁A����</returns>
	bool IsSpawnZone(CVehicleBase::ERoadType roadType);

private:
	// �ԗ��Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CVehicleManager* spInstance;
	// �����ꏊ�ɎԂ����邩���f����N���X
	CTrashVehicleSpawnZone* mpSpawnZone;

	CModel* mpCarModel;	// �Ԃ̃��f��

	std::list<CCar*> mCars;		// �Ԃ̃��X�g
	CGarbageTruck* mpGarbageTruck;	// �ʏ�g���b�N
	CGarbageTruck* mpPunishTruck;	// ���d�u���g���b�N

	// �o���܂ł̎���
	float mCarPopTime;			// ��
	float mTruckPopTime;		// �g���b�N
	float mPunishTruckPopTime;	// ���d�u���g���b�N
};