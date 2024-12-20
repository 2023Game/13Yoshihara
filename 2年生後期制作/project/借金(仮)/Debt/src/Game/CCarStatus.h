#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	// �R���X�g���N�^
	CCarStatus();
	/// <summary>
	/// �R���X�g���N�^
	/// ���̃N���X���p�������L�����̃X�e�[�^�X��ݒ肷��p
	/// </summary>
	/// <param name="maxHp">�ő�HP</param>
	/// <param name="baseMoveSpeed">��b�ړ����x</param>
	/// <param name="jumpSpeed">�W�����v���x</param>
	/// <param name="knockback">�m�b�N�o�b�N���x</param>
	/// <param name="power">�S�~�𗎂Ƃ���</param>
	CCarStatus(int maxHp, float baseMoveSpeed,
		float jumpSpeed, float knockback, int power);
	~CCarStatus();

	// ���ł܂ł̎��Ԃ��擾����
	float GetDeleteTime() const;
	// ���ł܂ł̎��Ԃ�ݒ肷��
	void SetDeleteTime(float deleteTime);
	// ���ł܂ł̎��Ԃ��o�߂������ǂ���
	bool IsElapsedDeleteTime() const;
	// ���ł܂ł̎��Ԃ��J�E���g
	void CountDeleteTime();
private:
	float mDeleteTime;	// ���ł܂ł̎���
};