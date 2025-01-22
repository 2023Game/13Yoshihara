#pragma once

class CResident;
class CModelX;

// �Z�l�̊Ǘ��N���X
class CResidentManager
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CResidentManager* Instance();
	// �R���X�g���N�^
	CResidentManager();
	// �f�X�g���N�^
	~CResidentManager();
private:
	// �Z�l��S�Đ���
	void CreateResident();
	// �o�H�T���p�̃m�[�h���쐬
	void CreateNavNodes();
	// �Z�l�Ǘ��N���X�̃C���X�^���X
	static CResidentManager* spInstance;

	// �Z�l�̃��f��
	CModelX* mpResidentModel;
	// �Z�l�̃��X�g
	std::vector<CResident*> mpResidents;
	// ���̕����̏���|�C���g
	std::vector<CNavNode*> mpPatrolPointsLeft;
	// �E�̕����̏���|�C���g
	std::vector<CNavNode*> mpPatrolPointsRight;

	// �t�B�[���h���ɂ���S�~�܂̐�
	int mFieldBagNum;
};