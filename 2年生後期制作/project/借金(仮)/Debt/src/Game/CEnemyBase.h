#pragma once
// �L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CRideableObject.h"

// ����͈͂̃f�o�b�O�\���N���X�̑O�錾
class CDebugFieldOfView;
class CGaugeUI3D;
class CNavNode;
/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemyBase : public CXCharacter
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="fovAngle">����͈͂̊p�x</param>
	/// <param name="fovLength">����͈͂̋���</param>
	/// <param name="patrolPoints">����|�C���g�̃��X�g</param>
	/// <param name="eyeHeight">���_�̍���</param>
	CEnemyBase(float fovAngle, float fovLength, 
		std::vector<CVector> patrolPoints,
		float eyeHeight);
	// �f�X�g���N�^
	~CEnemyBase();

	// �I�u�W�F�N�g�폜����
	void DeleteObject(CObjectBase* obj) override;
	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

protected:
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(int type, bool restart = false);

	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsWall;		// �ǂɐڐG���Ă��邩�ǂ���

	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��
	CVector mWallNormal;	// �ǂɐڐG���Ă��鎞�̖@��

	CCollider* mpBodyCol;	// �{�̂̃R���C�_�[

	CTransform* mpRideObject;

	// �U�����󂯂Ă��邩
	bool mIsDamage;

	float mFovAngle;	// ����͈͂̊p�x
	float mFovLength;	// ����͈͂̋���
	CDebugFieldOfView* mpDebugFov;	// ����͈͂̃f�o�b�O�\��

	float mEyeHeight;	// ���_�̍���

	CNavNode* mpLostPlayerNode;	// �v���C���[�����������ʒu�̃m�[�h

	// �v���C���[������͈͓��ɓ��������ǂ���
	bool IsFoundPlayer() const;
	// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
	bool IsLookPlayer() const;
	// �v���C���[���U���ł��邩�ǂ���
	bool CanAttackPlayer(float attackRange) const;

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	// ���ɏ��񂷂�|�C���g��ύX
	void ChangePatrolPoint(float nearDist);
	
	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex;	// ���ɏ��񂷂�|�C���g�̔ԍ�

	std::vector<CNavNode*> mMoveRoute;	// ���߂��ŒZ�o�H�L���p
	int mNextMoveIndex;					// ���Ɉړ�����m�[�h�̃C���f�b�N�X�l
};