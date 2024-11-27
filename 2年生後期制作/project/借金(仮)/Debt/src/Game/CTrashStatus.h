#pragma once
#include "CJobStatusBase.h"

// �S�~�E���Q�[���Ŏg�p����X�e�[�^�X�ꗗ
class CTrashStatus : public CJobStatusBase
{
public:
	CTrashStatus();
	~CTrashStatus();
private:
	// �v���C���[�̃X�e�[�^�X
	int mPlayerMaxHp;		// �ő�HP
	float mPlayerMoveSpeed;	// �ړ����x
	float mPlayerJumpSpeed;	// �W�����v���x
	float mPlayerKnockBack;	// �m�b�N�o�b�N���x
	int mPlayerPower;		// �S�~�𗎂Ƃ���
	float mPlayerPointRatio;// ���_�{��

	// �G�̃X�e�[�^�X
	int mEnemyMaxHp;		// �ő�HP
	float mEnemyMoveSpeed;	// �ړ����x
	float mEnemyJumpSpeed;	// �W�����v���x
	float mEnemyKnockBack;	// �m�b�N�o�b�N���x
	int mEnemyPower;		// �S�~�𗎂Ƃ���

	// �Ԃ̃X�e�[�^�X
	float mCarMoveSpeed;	// �ړ����x
	float mCarKnockBack;	// �m�b�N�o�b�N���x
	int mCarPower;			// �S�~�𗎂Ƃ���
	float mCarDeleteTime;	// ���ł܂ł̎���

	// �S�~���W�Ԃ̃X�e�[�^�X
	int mTruckMaxHp;			// �ő�HP
	float mTruckMoveSpeed;		// �ړ����x
	float mTruckPopTime;		// �o���܂ł̎���
	float mTruckBackTime;		// �P���܂ł̎���
	float mTruckCollectRadius;	// ����͈͂̔��a 
	int mTruckMens;				// 
	// TODO�F�X�e�[�^�X�̒ǉ�
};