#pragma once
#include "CCharaStatusBase.h"

class CTrashStatusBase : public CCharaStatusBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="maxHp">�ő�HP</param>
	/// <param name="baseMoveSpeed">��b�ړ����x</param>
	/// <param name="jumpSpeed">�W�����v���x</param>
	/// <param name="knockback">�m�b�N�o�b�N���x</param>
	/// <param name="power">�S�~�𗎂Ƃ���</param>
	CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
		float knockback, int power);
	~CTrashStatusBase();

	// �^����m�b�N�o�b�N���x���擾
	float GetKnockback() const;

	// �S�~�𗎂Ƃ��͂��擾
	int GetPower() const;

	// �󂯂�m�b�N�o�b�N���x���擾
	float GetTakeKnockback() const;
	// �󂯂�m�b�N�o�b�N���x��ݒ�
	void SetTakeKnockback(float knockback);
private:
	float mKnockback;		// �m�b�N�o�b�N���x
	int mPower;				// �S�~�𗎂Ƃ���
	float mTakeKnockback;	// �󂯂�m�b�N�o�b�N���x
};