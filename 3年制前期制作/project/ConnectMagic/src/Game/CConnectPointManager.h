#pragma once

class CConnectPoint;

// �ڑ����̊Ǘ��N���X
class CConnectPointManager : CTask
{
public:
	// �C���X�^���X���擾
	static CConnectPointManager* Instance();

	// �R���X�g���N�^
	CConnectPointManager();
	// �f�X�g���N�^
	~CConnectPointManager();

	// �X�V
	void Update();

	// �`��
	void Render();

	/// <summary>
	/// ���_����̃��C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
	/// </summary>
	/// <param name="hitPos">�Փ˂�����ԋ߂��ʒu</param>
	/// <returns>�Փ˂�����</returns>
	bool Ray(CVector& hitPos);

	/// <summary>
	/// ���_����^�[�Q�b�g�܂ł̃��C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
	/// </summary>
	/// <param name="targetPos">�^�[�Q�b�g�̍��W</param>
	/// <returns>�Փ˂�����</returns>
	bool RayTarget(CVector targetPos);

	// �Փ˔�����s���R���C�_�[�����X�g�ɒǉ�
	void AddCollider(CCollider* col);
	// �Փ˔�����s���R���C�_�[�����X�g�����菜��
	void RemoveCollider(CCollider* col);

	// �ڑ����𐶐�
	void CreateConnectPoint(CVector createPos);

	/// <summary>
	/// �ڑ���������
	/// </summary>
	/// <param name="num">��������v�f�ԍ�</param>
	void DeleteConnectPoint(int num);

	// �Ō�̗v�f����������
	void DeleteLastConnectPoint();

	// ��̐ڑ����̗L��������ݒ�
	void SetWandConnect(bool isOnOff);
	// �񂪐ڑ�����Ă��邩
	bool GetWandConnect();
private:
	// �C���X�^���X
	static CConnectPointManager* spInstance;

	// �ڑ����̃��X�g
	std::vector<CConnectPoint*> mPoints;
	// ��p�̐ڑ���
	CConnectPoint* mpPoint;

	// ���_�����̃��C�ŏՓ˔��肷��R���C�_�[�̃��X�g
	std::list<CCollider*> mColliders;

	// �ڑ��ł��鐔�̍ő�l
	int mConnectMaxNum;
};