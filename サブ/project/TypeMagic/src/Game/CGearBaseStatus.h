#pragma once

#define TEST_NAME "TEST"

// �����̃X�e�[�^�X���N���X
class CGearBaseStatus
{
private:
	enum class ERare
	{
		eCommon,	// ��
		eUnCommon,	// ��
		eRare,		// ��
		eEpic,		// ��
		eLegendary	// ��
	};
	// ���A�xs
	ERare mRare;

	// ������
	std::string mName;

	// �X�e�[�^�X
	std::map<std::string, int> mStatus;

public:
	// �R���X�g���N�^
	CGearBaseStatus(ERare rare = ERare::eCommon, std::string name = TEST_NAME);
	// �f�X�g���N�^
	~CGearBaseStatus();

	// ���A�x���擾����
	ERare GetRare();
	// ���������擾����
	std::string GetName();
	// �X�e�[�^�X���擾����
	std::map<std::string, int> GetStatus();

	// TODO�F��
	// �X�e�[�^�X��\��
	void PrintStatus()
	{
		// �X�e�[�^�X���擾
		std::map<std::string, int> statusList = GetStatus();
		// �S�ẴX�e�[�^�X��\��
		for (auto status : statusList)
		{
			// �X�e�[�^�X�̐��l���擾
			int statusNum = status.second;

			// ���l��0�Ȃ玟��
			if (statusNum == 0)
			{
				continue;
			}

			// �\��
			printf("%s:%d\n", status.first, statusNum);
		}
	}
};
