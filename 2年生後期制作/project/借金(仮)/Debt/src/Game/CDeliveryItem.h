#pragma once
#include "CObjectBase.h"
#include "CDeliveryItemStatus.h"

class CModel;
class CSound;

// ���˂���z�B��
class CDeliveryItem : public CObjectBase, public CDeliveryItemStatus
{
public:
	// �R���X�g���N�^
	CDeliveryItem(CObjectBase* owner);
	// �f�X�g���N�^
	~CDeliveryItem();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;


	// �ړ���ݒ�
	void SetMoveSpeed(CVector moveSpeed);

	// ��������擾����
	CObjectBase* GetOwner();

private:
	// �R���C�_�\�𐶐�
	void CreateCol();
	// 3d���f��
	CModel* mpModel;
	// �{�̃R���C�_�\
	CCollider* mpBodyCol;
	// ������
	CObjectBase* mpOwner;
	// ������̍��W
	CVector mOwnerPos;
	// �S�[������
	CSound* mpGoalSE;
	// �q�b�g��
	CSound* mpHitSE;
	// �_���[�W��
	CSound* mpDamageSE;

	// �ړ�
	CVector mMoveSpeed;
	// �ړ����邩
	bool mIsMove;
	// �o�ߎ���
	float mElapsedTime;
	// �����傪�v���C���[��
	bool mIsPlayer;
};