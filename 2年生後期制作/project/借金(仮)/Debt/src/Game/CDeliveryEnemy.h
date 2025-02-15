#pragma once
#include "CEnemyBase.h"
#include "CDeliveryEnemyStatus.h"
#include "RoadType.h"

class CDeliveryHpUI3D;

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
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();

	// �L��������؂�ւ���
	void SetOnOff(bool setOnOff);
	// ���݂̎ԓ���ݒ�
	void SetRoadType(ERoadType roadType);
	// ���݂̎ԓ����擾
	ERoadType GetRoadType() const;

	// ����ł��邩�ǂ���
	bool IsDeath() const override;

	// �����댯����ݒ�
	void SetRoadDanger(ERoadType roadType, bool danger);

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

	/// <summary>
	/// �w�肵���ʒu�܂ňړ����� 
	/// </summary>
	/// <param name="targetPos">�ړI�n</param>
	/// <param name="speed">�ړ����x</param>
	/// <param name="rotateSpeed">��]���x</param>
	/// <returns>��������</returns>
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	
	// �_���[�W�̓_�łƖ��G���Ԃ̏���
	void HitFlash();
	
	// �����𖞂������Ƃ��Ɏˌ�����
	void Shoot();

	// 3d���f��
	CModel* mpModel;
	// HpUI
	CDeliveryHpUI3D* mpHpUI;

	// �T�m�R���C�_�\
	CCollider* mpSearchCol;

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

	// �����Ƃɂ��̂܂܂��Ɗ댯��
	bool mIsLeft1Danger;	// ��1
	bool mIsLeft2Danger;	// ��2
	bool mIsRight1Danger;	// �E1
	bool mIsRight2Danger;	// �E2
	// �����ƂɃA�C�e�������邩
	bool mIsLeft1Item;	// ��1
	bool mIsLeft2Item;	// ��2
	bool mIsRight1Item;	// �E1
	bool mIsRight2Item;	// �E2

	// �����铹���댯��
	bool GetNowRoadDanger() const;
	// �w�肵�������댯��
	bool GetRoadDanger(ERoadType roadType) const;

	// �w�肵�����ɃA�C�e�������邩��ݒ肷��
	void SetRoadItem(ERoadType roadType, bool isItem);
	// �w�肵�����ɃA�C�e�������邩
	bool GetRoadItem(ERoadType roadType) const;
	// ���E�ɃA�C�e��������ꍇ�ɎԐ���ύX����
	void ChangeRoadToItem();
};