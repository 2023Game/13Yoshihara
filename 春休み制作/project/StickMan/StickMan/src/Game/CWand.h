#pragma once
#include "CWeapon.h"

class CModel;
class CConnectPoint;

// ��̃N���X
class CWand : public CWeapon
{
public:
	// �R���X�g���N�^
	CWand(CObjectBase* owner, ETag tag);
	// �f�X�g���N�^
	~CWand();

	// �X�V
	void Update() override;

	// �`��
	void Render() override;

	// �Փ˔�����s���R���C�_�[�����X�g�ɒǉ�
	void AddCollider(CCollider* col);
	// �Փ˔�����s���R���C�_�[�����X�g�����菜��
	void RemoveCollider(CCollider* col);
	
	/// <summary>
	/// �ݒ肳��Ă���R���C�_�[�ƏՓ˔�����s��
	/// </summary>
	/// <param name="hitPos">�Փ˂�����ԋ߂��ʒu</param>
	/// <returns>�Փ˂�����</returns>
	bool Ray(CVector& hitPos);

	// �ڑ����̗L��������ݒ�
	void SetConnect(bool isOnOff);
	// �ڑ�����Ă��邩
	bool GetConnect();

private:
	CModel* mpModel;		// ��̃��f��
	CConnectPoint* mpPoint;	// �ڑ���

	// ���_�����̃��C�ŏՓ˔��肷��R���C�_�[�̃��X�g
	std::list<CCollider*> mColliders;
};
