#pragma once
#include "CSwitchObject.h"

// �X�C�b�`�œ�����
class CSwitchMoveFloor : public CSwitchObject
{
public:
	// �R���X�g���N�^
	CSwitchMoveFloor(CModel* model, CModel* colModel,
		CVector& pos,
		CVector& move,
		float moveTime);
	// �f�X�g���N�^
	~CSwitchMoveFloor();

private:
	// ��p���Ă��Ȃ����̏���
	void UpdateOff() override;
	// ��p���Ă��鎞�̏���
	void UpdateOn() override;

	// �ړ��̏���
	void Move();

	CVector mDefaultPos;	// �������W
	CVector mMoveVec;		// �ړ��x�N�g��
	float mMoveTime;		// �ړ�����
	float mElapsedTime;		// �o�ߎ���
};