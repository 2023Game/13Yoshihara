#pragma once
#include "CObjectBase.h"
#include "CTrashBagStatus.h"

class CModel;

class CTrashBag : public CObjectBase , public CTrashBagStatus
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="gold">�S�[���h���ǂ���</param>
	CTrashBag(bool gold);
	// �f�X�g���N�^
	~CTrashBag();

	// �X�V
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// �`��
	void Render() override;

	// �L�������ɂ���
	void SetOnOff(bool isOnOff);

	// �S�[���h������
	void SetGold(bool gold);

	// ���ł������x��ݒ�
	void SetThrowSpeed(CVector speed, float speedY);
	// �d�͂������邩��ݒ�
	void SetGravity(bool gravity);

private:
	// �{�̃R���C�_�\
	CCollider* mpBodyCol;
	// 3D���f��
	CModel* mpModel;

	// �O�㍶�E�̈ړ����x
	CVector mMoveSpeed;
	// �㉺�̈ړ����x
	float mMoveSpeedY;

	CTransform* mpRideObject;

	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��
	bool mIsGrounded;		// �ڒn���Ă��邩�ǂ���

	bool mIsGravity;	// �d�͂��|���邩�ǂ���
};