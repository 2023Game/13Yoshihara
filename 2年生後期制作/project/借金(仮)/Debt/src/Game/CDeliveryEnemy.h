#pragma once
#include "CEnemyBase.h"
#include "CDeliveryEnemyStatus.h"
#include "RoadType.h"

/*
�z�B�Q�[���̓G�N���X
�G���N���X���p��
*/
class CDeliveryEnemy : public CEnemyBase, public CDeliveryEnemyStatus
{
public:
	// �R���X�g���N�^
	CDeliveryEnemy();
	// �f�X�g���N�^
	~CDeliveryEnemy();

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	/// <param name="causer">�U�����Ă�������</param>
	/// <param name="causer">�����̎ˌ��ɂ�錸����</param>
	void TakeDamage(int damage, CObjectBase* causer, bool isShot = false);

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();

	// �L��������؂�ւ���
	void SetOnOff(bool setOnOff);
	// ���݂̎ԓ���ݒ�
	void SetRoadType(ERoadType roadType);
	// ���݂̎ԓ����擾
	ERoadType GetRoadType() const;

private:
	// ���
	enum class EState
	{
		eMove,			// �ړ�
		eChangeRoad,	// �Ԑ��ύX
		eDeath,			// ���S
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �G�̏��
	int mStateStep;				// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	float mInvincibleTime;		// ���G���Ԍv���p
	float mHitFlashTime;		// �_�ŊԊu�v���p

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
#endif
	// �ړ��̍X�V����
	void UpdateMove();
	// �Ԑ��ύX�̍X�V����
	void UpdateChangeRoad();
	// ���S�̍X�V����
	void UpdateDeath();
	// ���S
	void Death() override;

	// �R���C�_�\�𐶐�
	void CreateCol();
	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	// �_���[�W�̓_�łƖ��G���Ԃ̏���
	void HitFlash();

	// 3d���f��
	CModel* mpModel;

	// �Ԑ��ύX�̖ړI�n
	CVector mTargetPos;
	// �ړI�n�̎ԓ��̎��
	ERoadType mTargetRoadType;

	// ���݂̎ԓ�
	ERoadType mRoadType;
};