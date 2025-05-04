#pragma once
#include <array>

class CDesk;
class CClassRoom;

// ���̍s�Ɨ�̐�
#define DESK_ROW 5
#define DESK_COL 5

// �t�B�[���h�Ǘ��N���X
class CFieldManager
{
public:
	// �t�B�[���h�Ǘ��N���X�̃C���X�^���X���擾
	static CFieldManager* Instance();
	// �R���X�g���N�^
	CFieldManager();
	// �f�X�g���N�^
	~CFieldManager();

	// �n�ʂ̃R���C�_�\�擾
	CCollider* GetGroundCol();
	// �ǂ̃R���C�_�\�擾
	CCollider* GetWallCol();
	// �I�u�W�F�N�g�̃R���C�_�\�擾
	CCollider* GetObjCol();

	// ���X�|�[���n�_���W���擾
	CVector GetSpawnPos();

private:
	// �t�B�[���h�Ǘ��N���X�̃C���X�^���X
	static CFieldManager* spInstance;

	// �v���C���[�̃X�|�[���n�_�ɂ���s��
	CVector2 mSpawnColRow;

	// ���̃��X�g
	std::vector<CDesk*> mpDesks;

	// ���̔z�u�̍s��
	// 0���������Ȃ�
	// 1�������\��
	// 2�������ς�
	int mMap[DESK_ROW][DESK_COL] = {};

	// ����
	CClassRoom* mpClassRoom;

	// �}�b�v�𐶐�
	void CreateField();
	// ���𐶐�

	/// <summary>
	/// ���𐶐�
	/// </summary>
	/// <param name="row">�s</param>
	/// <param name="col">��</param>
	void CreateDesk(int row, int col);
	// ���X�g�͈͓̔���
	bool InList(int row, int col);
};