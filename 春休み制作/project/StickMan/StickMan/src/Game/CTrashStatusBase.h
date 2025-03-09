#pragma once
#include "CCharaStatusBase.h"

class CTrashStatusBase : public CCharaStatusBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="maxHp">			�ő�HP</param>
	/// <param name="baseMoveSpeed">	��b�ړ����x</param>
	/// <param name="jumpSpeed">		�W�����v���x</param>
	/// <param name="knockbackDealt">	�^����m�b�N�o�b�N���x</param>
	/// <param name="attackPower">		�U����</param>
	/// <param name="power">			�S�~�𗎂Ƃ���</param>
	CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
		float knockbackDealt, int attackPower, int power);
	~CTrashStatusBase();

	/// <summary>
	/// �S�ẴX�e�[�^�X��ݒ�
	/// </summary>
	/// <param name="maxHp">			�ő�HP</param>
	/// <param name="baseMoveSpeed">	��b�ړ����x</param>
	/// <param name="jumpSpeed">		�W�����v���x</param>
	/// <param name="knockbackDealt">	�^����m�b�N�o�b�N���x</param>
	/// <param name="attackPower">		�U����</param>
	/// <param name="power">			�S�~�𗎂Ƃ���</param>
	void SetAllStatus(int maxHp, float baseMoveSpeed, float jumpSpeed,
		float knockbackDealt, int attackPower, int power);

	// �^����m�b�N�o�b�N���x���擾
	float GetKnockbackDealt() const;
	// �^����m�b�N�o�b�N���x��ݒ�
	void SetKnockbackDealt(float knockbackDealt);

	// �󂯂�m�b�N�o�b�N���擾
	CVector GetKnockbackReceived() const;
	// �󂯂�m�b�N�o�b�N��ݒ�
	void SetKnockbackReceived(CVector knockbackReceived);

	// �S�~�𗎂Ƃ��͂��擾
	int GetPower() const;
	// �S�~�𗎂Ƃ��͂�ݒ�
	void SetPower(int power);

	// �S�~�܂̐������Z����
	void SetTrashBag(int num);
	// �S�~�܂̐����擾����
	int GetTrashBag() const;

	// �S�[���h�S�~�܂̐������Z����
	void SetGoldTrashBag(int num);
	// �S�[���h�S�~�܂̐����擾����
	int GetGoldTrashBag() const;

	/// <summary>
	/// �S�~�܂𗎂Ƃ�����
	/// </summary>
	/// <param name="power">���Ƃ���</param>
	/// <param name="pos">�����̍��W</param>
	/// <param name="front">���ʕ���</param>
	/// <param name="side">������</param>
	/// <param name="offsetPos">�S�~�܂𗎂Ƃ��I�t�Z�b�g���W</param>
	void DropTrashBag(int power,
		CVector pos, CVector front, CVector side,
		CVector offsetPos);
private:
	float mKnockbackDealt;		// �^����m�b�N�o�b�N���x
	CVector mKnockbackReceived;	// �󂯂�m�b�N�o�b�N
	int mPower;					// �S�~�𗎂Ƃ���

	// �����Ă���S�~�܂̐�
	int mTrashBagNum;
	// �����Ă���S�[���h�S�~�܂̐�
	int mGoldTrashBagNum;
};