#pragma once

// �S�~�܂̃X�e�[�^�X
class CTrashBagStatus
{
public:
	// �R���X�g���N�^
	CTrashBagStatus(bool gold);
	// �f�X�g���N�^
	~CTrashBagStatus();

	// �S�[���h���ǂ������擾����
	bool GetIsGold() const;
	// �S�[���h���ǂ�����ݒ肷��
	void SetIsGold(bool isGold);

	// ���ł܂ł̎��Ԃ��擾����
	float GetDeleteTime() const;
	// ���ł܂ł̎��Ԃ������l�ɐݒ肷��
	void SetDeleteTime();
	// ���ł܂ł̎��Ԃ��o�߂������ǂ���
	bool IsElapsedDeleteTime() const;
	// ���ł܂ł̎��Ԃ��J�E���g�_�E��
	void CountDeleteTime();

	// ���_���擾����
	int GetPoint(bool gold) const;

private:
	// �S�[���h�܂��ǂ���
	bool mIsGold;
	// ���ł܂ł̎���
	float mDeleteTime;
};