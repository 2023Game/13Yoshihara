#pragma once
#include "CTrashStatusBase.h"

class CVehicleStatus : public CTrashStatusBase
{
public:
	// �R���X�g���N�^
	CVehicleStatus();
	/// <summary>
	/// �R���X�g���N�^
	/// ���̃N���X���p�������L�����̃X�e�[�^�X��ݒ肷��p
	/// </summary>
	/// <param name="maxHp">�ő�HP</param>
	/// <param name="baseMoveSpeed">��b�ړ����x</param>
	/// <param name="jumpSpeed">�W�����v���x</param>
	/// <param name="knockback">�m�b�N�o�b�N���x</param>
	/// <param name="power">�S�~�𗎂Ƃ���</param>
	CVehicleStatus(int maxHp, float baseMoveSpeed,
		float jumpSpeed, float knockback, int power);
	// �f�X�g���N�^
	~CVehicleStatus();

	// ���ł܂ł̎��Ԃ��擾����
	float GetDeleteTime() const;
	// ���ł܂ł̎��Ԃ������l�ɐݒ肷��
	void SetDeleteTime();
	// ���ł܂ł̎��Ԃ��o�߂������ǂ���
	bool IsElapsedDeleteTime() const;
	// ���ł܂ł̎��Ԃ��J�E���g�_�E��
	void CountDeleteTime();
private:
	float mDeleteTime;	// ���ł܂ł̎���
};