#pragma once

class CConnectPoint;
class CConnectTarget;
class CConnectObject;

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
	void Connect();

	/// <summary>
	/// ���_����̃��C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
	/// </summary>
	/// <param name="hitPos">�Փ˂�����ԋ߂��ʒu</param>
	/// <returns>�Փ˂�����</returns>
	bool Ray(CVector& hitPos);

	/// <summary>
	/// 2�_���q�������C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
	/// </summary>
	/// <param name="start">�n�܂�̓_</param>
	/// <param name="end">�I���̓_</param>
	/// <param name="hit">�Փˏ��</param>
	/// <returns></returns>
	bool Ray(const CVector& start, const CVector& end, CHitInfo* hit);

	/// <summary>
	/// ���_����^�[�Q�b�g�܂ł̃��C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
	/// </summary>
	/// <param name="targetPos">�^�[�Q�b�g�̍��W</param>
	/// <returns>�Փ˂�����</returns>
	bool RayTarget(CVector targetPos);

	/// <summary>
	/// �ڑ����ƌq�������C�Ɛݒ肳��Ă���R���C�_�[�Ƃ̏Փ˔�����s��
	/// �Փ˂��Ă����疳��
	/// </summary>
	void RayPoint();

	// �ڑ����Ƃ̋������ő�l��艓����
	// �ŏ��l���߂���ΐڑ��𖳌��ɂ���
	void FarOrNearDist();

	// �Փ˔�����s���R���C�_�[�����X�g�ɒǉ�
	void AddCollider(CCollider* col);
	// �Փ˔�����s���R���C�_�[�����X�g�����菜��
	void RemoveCollider(CCollider* col);
	// �Փ˔�����s���R���C�_�[�����Z�b�g
	void ResetCollider();

	// �ڑ���L��
	void EnableConnect(CConnectTarget* connectTarget);

	// �ڑ��𖳌�
	void DisableConnect(CConnectTarget* connectTarget);

	// ��̐ڑ����̗L��������ݒ�
	void SetWandConnect(bool isOnOff, CConnectTarget* target);
	// �񂪐ڑ�����Ă��邩
	bool GetWandConnect();

	// ��Ɛڑ����̃^�[�Q�b�g��ݒ�
	void SetConnectWandTarget(CConnectTarget* connectTarget);
	// ��Ɛڑ����̃^�[�Q�b�g���擾
	CConnectTarget* GetConnectWandTarget();

	// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋�����ݒ�
	void SetConnectDistance();
	// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋������擾
	float GetConnectDistance();

	// ���݂̏񂪐ڑ����Ă���ڑ����ƃv���C���[�̋������擾
	float GetNowConnectDistance();

	// ��Ɛڑ����Ă���I�u�W�F�N�g���󒆂̐ڑ��I�u�W�F�N�g��
	bool IsWandConnectAirObject();

private:
	// �C���X�^���X
	static CConnectPointManager* spInstance;

	// �ڑ���̐ڑ���
	CConnectPoint* mpConnectPoint;
	// ��p�̐ڑ���
	CConnectPoint* mpWandPoint;
	// ��Ɛڑ����̃^�[�Q�b�g
	CConnectTarget* mpConnectWandTarget;

	// ���_�����̃��C�ŏՓ˔��肷��R���C�_�[�̃��X�g
	std::list<CCollider*> mColliders;

	// ��̐�̐ڑ����̈ʒu�����
	void WandPos();

	// �񂪐ڑ����Ă���ڑ����ƃv���C���[�̋���
	float mConnectDistance;
};