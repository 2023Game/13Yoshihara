#pragma once
#include "CCharaStatusBase.h"

class CTrashStatusBase : CCharaStatusBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="mMaxHp">�ő�HP</param>
	/// <param name="moveSpeed">�ړ����x</param>
	/// <param name="jumpSpeed">�W�����v���x</param>
	/// <param name="knockback">�m�b�N�o�b�N���x</param>
	/// <param name="power">�S�~�𗎂Ƃ���</param>
	CTrashStatusBase(int mMaxHp, float moveSpeed, float jumpSpeed,
		float knockback, int power);
	~CTrashStatusBase();

	// �^����m�b�N�o�b�N���x���擾
	float GetKnockback() const;
	// �^����m�b�N�o�b�N���x��ݒ�
	void SetKnockback(float knockback);

	// �S�~�𗎂Ƃ��͂��擾
	int GetPower() const;
	// �S�~�𗎂Ƃ��͂�ݒ�
	void SetPower(int power);

	// �󂯂�m�b�N�o�b�N���x���擾
	float GetTakeKnockback() const;
	// �󂯂�m�b�N�o�b�N���x��ݒ�
	void SetTakeKnockback(float knockback);
private:
	float mKnockback;		// �m�b�N�o�b�N���x
	int mPower;				// �S�~�𗎂Ƃ���
	float mTakeKnockback;	// �󂯂�m�b�N�o�b�N���x
};