#pragma once
#include "CSwitchObject.h"

// �X�C�b�`�œ�����
class CSwitchMoveFloor : public CSwitchObject
{
public:
	// �R���X�g���N�^
	CSwitchMoveFloor(CModel* model,
		const CVector& pos, 
		const CVector& scale,
		const CVector& move,
		float moveTime);
	// �f�X�g���N�^
	~CSwitchMoveFloor();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ��p���Ă��鎞�̏���
	void UpdateOn() override;

	// �ړ��̏���
	void Move();

	CVector mDefaultPos;	// �������W
	CVector mMoveVec;		// �ړ��x�N�g��
	float mMoveTime;		// �ړ�����

	enum class EMoveState
	{
		eStop,	// �~�܂��Ă���
		eGo,	// �i��ł���
		eBack,	// �߂��Ă���
	};
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