#pragma once
#include "CCharaBase.h"
#include "CGarbageTruckStatus.h"
#include "CVehicleBase.h"
#include "CGarbageTruckStatus.h"

class CModel;
class CCollector;
class CGaugeUI3D;

// �S�~���W�Ԃ̃N���X
// �Ԃ̃N���X���p��
// �S�~���W�Ԃ̃X�e�[�^�X���p��
class CGarbageTruck : public CVehicleBase , public CGarbageTruckStatus
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="pos">���W</param>
	/// <param name="rotation">��]</param>
	/// <param name="road">���̎��</param>
	/// <param name="patrolPoints">���񂷂�|�C���g�̃��X�g</param>
	/// <param name="punisher">true�Ȃ�΁A���d�u���p</param>
	CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints, bool punisher);
	// �f�X�g���N�^
	~CGarbageTruck();

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	/// <param name="causer">�U�����Ă�������</param>
	void TakeDamage(int damage, CObjectBase* causer) override;
	/// <summary>
	/// �N���e�B�J���_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	/// <param name="causer">�U�����Ă�������</param>
	void TakeCritical(int damage, CObjectBase* causer) override;

	// �I�u�W�F�N�g�폜����
	void DeleteObject(CObjectBase* obj) override;

	// �X�V
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// �`��
	void Render() override;

	// �ԗ��̗L��������؂�ւ���
	void SetOnOff(bool setOnOff) override;
	// �ϐ������Z�b�g
	void Reset() override;

	// ������ł���Z�͈͓̔��ɂ��邩�ǂ���
	bool CanCollectPosZ();

protected:
	// �S�~�܂𗎂Ƃ�����
	void DropTrashBag(int power);

	// ������̃��X�g
	std::vector<CCollector*> mpCollectors;
	// �v���C���[�ƓG�̒T�m�p
	CCollider* mpSearchCol;

	// �P�ނ̈ړ������ǂ���
	bool mIsReturn;

	// �ړ�����
	void UpdateMove();
	// ��~����
	void UpdateStop();
	// ��ꂽ����
	void UpdateBroken();
	// �Ԑ��ύX����
	void UpdateChangeRoad();
	// �������
	void UpdateCollect();
	// ���S(��ꂽ)
	void Death() override;

	// �Ԃ̏��
	enum class EState
	{
		eMove,		// �ړ�
		eStop,		// ��~
		eBroken,	// ����
		eChangeRoad,// �Ԑ��ύX
		eCollect,	// ���
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �ԗ��̏��
	int mStateStep;	// ��ԓ��̃X�e�b�v��
	float mElapsedTime;	// �o�ߎ���

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
#endif

	// Hp�Q�[�W
	CGaugeUI3D* mpHpGauge;
};