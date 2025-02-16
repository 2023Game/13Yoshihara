#pragma once

/*
�����̊Ǘ��N���X
*/
class CMoneyManager
{
public:
	// �C���X�^���X���擾
	static CMoneyManager* Instance();
	// �C���X�^���X��j��
	static void ClearInstance();

	// �R���X�g���N�^
	CMoneyManager();
	// �f�X�g���N�^
	~CMoneyManager();

	// ���������擾����
	int GetMoney() const;
	// �����������Z����
	void SetMoney(int money);

private:
	// �����̊Ǘ��N���X�̃C���X�^���X
	static CMoneyManager* spInstance;

	// ������
	int mMoney;
};