#pragma once
#include "CConnectObject.h"

class CFlamethrower;

// ���̃X�P�[��
#define FIRE_SCALE 0.7f

// ���̈ړ����x
#define MOVE_SPEED 2.0f

// �ڑ��^�[�Q�b�g�̃I�t�Z�b�g���W
#define TARGET_OFFSET_POS CVector(0.0f,2.0f,0.0f)

// �������ڑ��I�u�W�F�N�g
class CFire : public CConnectObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="fireOffsetPos">���̃I�t�Z�b�g���W</param>
	/// <param name="fireScale">���̃X�P�[��</param>
	/// <param name="fireSpeed">���̈ړ����x</param>
	/// <param name="targetOffsetPos">�ڑ��^�[�Q�b�g�̃I�t�Z�b�g���W</param>
	CFire(CVector fireOffsetPos = CVector::zero,
		float fireScale = FIRE_SCALE, float fireSpeed = MOVE_SPEED,
		CVector targetOffsetPos = TARGET_OFFSET_POS);
	// �f�X�g���N�^
	~CFire();

	// �q�������Ƃ��̏���
	void Connect(CVector wandPointPos, CVector targetPointPos) override;

	// �������Ă��邩��ݒ�
	void SetFire(bool isEnable);
	// �������Ă��邩���擾
	bool GetFire() const;

protected:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �R�������̏���
	virtual void Burning();

	// ���̃G�t�F�N�g�����p
	CFlamethrower* mpFlamethrower;

	// �������Ă��邩
	bool mIsFire;
};