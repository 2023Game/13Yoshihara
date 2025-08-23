#pragma once
#include "CSwitchObject.h"
#include "MoveState.h"

// �X�C�b�`�œ�����
class CSwitchMoveFloor : public CSwitchObject
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
	CSwitchMoveFloor(CModel* model, CModel* col,
		const CVector& pos, 
		const CVector& scale,
		const CVector& move,
		float moveTime, bool isCrushed = false);
	// �f�X�g���N�^
	~CSwitchMoveFloor();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// �v���C���[�����܂ꂽ���p�̃R���C�_�[
	CColliderMesh* mpCrushedCol;

	// ��p���Ă��鎞�̏���
	void UpdateOn() override;

	CVector mDefaultPos;	// �������W
	CVector mMoveVec;		// �ړ��x�N�g��
	float mMoveTime;		// �ړ�����

	// ��Ԃ�ύX
	void ChangeMoveState(EMoveState state);
	EMoveState mMoveState;		// �ړ����
	EMoveState mPreMoveState;	// ��O�̈ړ����
	float mElapsedTime;

	// �~�܂��Ă���Ƃ��̍X�V
	void UpdateStop();
	// �i��ł���Ƃ��̍X�V
	void UpdateGo();
	// �߂��Ă���Ƃ��̍X�V
	void UpdateBack();
};