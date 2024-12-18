#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

class CFlamethrower;

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayerBase : public CXCharacter
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CPlayerBase* Instance();

	// �R���X�g���N�^
	CPlayerBase(float capsuleRadius, float playerHeight);
	// �f�X�g���N�^
	~CPlayerBase();

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

	// �`��
	void Render();

protected:
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(int type, bool restart = false);

	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec();

	// ���[�V�����u���[�̍X�V����
	void UpdateMotionBlur();

	// �v���C���[�̃C���X�^���X
	static CPlayerBase* spInstance;

	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsWall;		// �ǂɐڐG���Ă��邩�ǂ���

	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��
	CVector mWallNormal;	// �ǂɐڐG���Ă��鎞�̖@��

	CColliderCapsule* mpColliderCapsule;

	CTransform* mpRideObject;

	// ���[�V�����u���[���|����c�莞��
	float mMotionBlurRemainTime;

	// �U�����󂯂Ă��邩
	bool mIsDamage;
};
