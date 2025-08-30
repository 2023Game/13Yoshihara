#pragma once
#include "CSwitchObject.h"
#include "MoveState.h"

// �X�C�b�`�œ�����
class CSwitchMoveWall : public CSwitchObject
{
public:
	// ��Ԃ�ݒ�
	void SetState(EMoveState state);
	// ��Ԃ��擾
	EMoveState GetState() const;
	// �O��̏�Ԃ�ݒ�
	void SetPreState(EMoveState state);
	// �O��̏�Ԃ��擾
	EMoveState GetPreState() const;
	// �o�ߎ��Ԃ�ݒ�
	void SetElapsedTime(float time);
	// �o�ߎ��Ԃ��擾
	float GetElapsedTime() const;

	// �R���X�g���N�^
	CSwitchMoveWall(CModel* model, CModel* col,
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime, bool isCrushed = false);
	// �f�X�g���N�^
	~CSwitchMoveWall();

	// �J���Ă��邩
	bool IsOpen() const;
	// �J���Ă��邩��ݒ�
	void SetIsOpen(bool enable);

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// �v���C���[�����܂ꂽ���p�̃R���C�_�[
	CColliderMesh* mpCrushedCol;

	// ��p���Ă��Ȃ����̏���
	void UpdateOff() override;
	// ��p���Ă��鎞�̏���
	void UpdateOn() override;

	// �ړ��̏���
	void Move();

	CVector mDefaultPos;	// �������W
	CVector mMoveVec;		// �ړ��x�N�g��
	float mMoveTime;		// �ړ�����

	// ��Ԃ�ύX
	void ChangeMoveState(EMoveState state);
	EMoveState mMoveState;		// �ړ����
	EMoveState mPreMoveState;	// ��O�̈ړ����
	float mElapsedTime;		// �o�ߎ���

	// �J���Ă��邩
	bool mIsOpen;
};