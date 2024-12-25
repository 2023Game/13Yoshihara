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
	/// <param name="power">			�S�~�𗎂Ƃ���</param>
	CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
		float knockbackDealt, int power);
	~CTrashStatusBase();

	// �^����m�b�N�o�b�N���x���擾
	float GetKnockbackDealt() const;

	// �󂯂�m�b�N�o�b�N���x���擾
	float GetKnockbackReceived() const;
	// �󂯂�m�b�N�o�b�N���x��ݒ�
	void SetKnockbackReceived(float knockbackReceived);

	// �S�~�𗎂Ƃ��͂��擾
	int GetPower() const;
private:
	float mKnockbackDealt;		// �^����m�b�N�o�b�N���x
	float mKnockbackReceived;	// �󂯂�m�b�N�o�b�N���x
	int mPower;					// �S�~�𗎂Ƃ���
};