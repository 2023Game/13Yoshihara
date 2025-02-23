#pragma once
#include "CPlayerBase.h"
#include "CDeliveryPlayerStatus.h"
#include "RoadType.h"

class CDeliveryHpUI2D;
class CSound;

/*
�z�B�Q�[���̃v���C���[�N���X
*/
class CDeliveryPlayer : public CPlayerBase , public CDeliveryPlayerStatus
{
public:
	// �R���X�g���N�^
	CDeliveryPlayer();
	// �f�X�g���N�^
	~CDeliveryPlayer();

	// �I�u�W�F�N�g�폜����
	void DeleteObject(CObjectBase* obj) override;

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
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);// override;

	// �`��
	void Render();

	// �z�B��������1���₷
	void IncreaseDeliveryNum();
	// �z�B���������擾����
	int GetDeliveryNum() const;
	// �󂵂��g���b�N�̐���1���₷
	void IncreaseDestroyEnemyNum();
	// �󂵂��g���b�N�̐����擾����
	int GetDestroyEnemyNum() const;
	// ���˂�������1���₷
	void IncreaseShotNum();
	// ���˂��������擾����
	int GetShotNum() const;
	// ������������1���₷
	void IncreaseHitNum();
	// �������������擾����
	int GetHitNum() const;

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
	EState mState;	// �v���C���[�̏��
	int mStateStep;				// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	float mInvincibleTime;		// ���G���Ԍv���p
	float mHitBlinkTime;		// �_�ŊԊu�v���p
	float mLeftShootTime;		// ���Ɍ��Ă�Ԋu�v���p
	float mRightShootTime;		// �E�Ɍ��Ă�Ԋu�v���p
	float mBackShootTime;		// ��Ɍ��Ă�Ԋu�v���p

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
	// �A�N�V�����̃L�[����
	void ActionInput();
	// �_���[�W�̓_�łƖ��G���Ԃ̏���
	void HitBlink();

	// 3d���f��
	CModel* mpModel;
	// HpUI
	CDeliveryHpUI2D* mpHpUI;

	// �Ԑ��ύX�̖ړI�n
	CVector mTargetPos;
	// �ړI�n�̎ԓ��̎��
	ERoadType mTargetRoadType;
	// ���ړ���
	bool mIsLeftMove;

	// ���݂̎ԓ�
	ERoadType mRoadType;
	
	/// <summary>
	/// �Ԑ��ύX��̍��W�����߂�
	/// </summary>
	/// <param name="isLeft">����</param>
	/// <returns></returns>
	CVector GetTargetPos(bool isLeft);

	// �z�B������
	int mDeliveryNum;
	// �G��j�󂵂���
	int mDestroyEnemyNum;
	// ���˂�����
	int mShotNum;
	// ����������
	int mHitNum;

	// �ԓ��m�̏Փˉ�
	CSound* mpClashSE;
};