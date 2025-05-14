#pragma once

class CConnectPoint;
class CConnectTarget;

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

	// �������鏈�������s
	void Pull();

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

	/// <summary>
	/// �ڑ������m���q�������C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
	/// �Փ˂��Ă�����폜����
	/// </summary>
	void RayPoint();

	// �Փ˔�����s���R���C�_�[�����X�g�ɒǉ�
	void AddCollider(CCollider* col);
	// �Փ˔�����s���R���C�_�[�����X�g�����菜��
	void RemoveCollider(CCollider* col);
	// �Փ˔�����s���R���C�_�[�����Z�b�g
	void ResetCollider();

	// �ڑ����𐶐�
	void CreateConnectPoint(CConnectTarget* connectTarget);

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

	// ��Ɛڑ����̃^�[�Q�b�g��ݒ�
	void SetConnectWandTarget(CConnectTarget* connectTarget);
	// ��Ɛڑ����̃^�[�Q�b�g���擾
	CConnectTarget* GetConnectWandTarget();

	// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋�����ݒ�
	void SetWandConnectDistance();
	// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋������擾
	float GetWandConnectDistance();

	/// <summary>
	/// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋�����ݒ�
	/// </summary>
	/// <param name="sign">����</param>
	void SetWandConnectDistance(int sign);

	// ��Ɛڑ����Ă���I�u�W�F�N�g���󒆂̐ڑ��I�u�W�F�N�g��
	bool IsWandConnectAirObject();

private:
	// �C���X�^���X
	static CConnectPointManager* spInstance;

	// �ڑ����̃��X�g
	std::vector<CConnectPoint*> mPoints;
	// ��p�̐ڑ���
	CConnectPoint* mpPoint;
	// ��Ɛڑ����̃^�[�Q�b�g
	CConnectTarget* mpConnectWandTarget;

	// ���_�����̃��C�ŏՓ˔��肷��R���C�_�[�̃��X�g
	std::list<CCollider*> mColliders;

	// �ڑ��ł��鐔�̍ő�l
	int mConnectMaxNum;

	// ��̐�̐ڑ����̈ʒu�����
	void WandPos();

	// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋���
	float mWandConnectDistance;
};