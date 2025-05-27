#pragma once
#include "CConnectObject.h"

class CFlamethrower;

// ���̃X�P�[��
#define FIRE_SCALE 0.7f

// �������ڑ��I�u�W�F�N�g
class CFire : public CConnectObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="modelName">���f���̖��O</param>
	/// <param name="fireOffsetPos">���̃I�t�Z�b�g���W</param>
	/// <param name="fireScale">���̃X�P�[��</param>
	CFire(std::string modelName = "", CVector fireOffsetPos = CVector::zero,
		float fireScale = FIRE_SCALE);
	// �f�X�g���N�^
	~CFire();

	// �q�������Ƃ��̏���
	void Connect(CConnectObject* other) override;

	// �������Ă��邩��ݒ�
	void SetFire(bool isEnable);
	// �������Ă��邩���擾
	bool GetFire() const;

protected:
	// �R���C�_�[�𐶐�
	void CreateCol(std::string modelName);

	// ���̃G�t�F�N�g�����p
	CFlamethrower* mpFlamethrower;

	// �������Ă��邩
	bool mIsFire;
};