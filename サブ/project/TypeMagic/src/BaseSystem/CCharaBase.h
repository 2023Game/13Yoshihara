#pragma once
#include "CObjectBase.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	virtual ~CCharaBase();

	// �X�V
	void Update() override;

	// �ړ��������擾����
	CVector GetMoveDir();

protected:
	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsGravity;	// �d�͂��|���邩�ǂ���
	bool mIsMoveDir;	// �ړ��������������ǂ���

	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��

	// �ړ�����
	CVector mMoveDir;

	CTransform* mpRideObject;
};
