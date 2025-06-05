#pragma once
#include "CObjectBase.h"
#include "ConnectObjectTag.h"

// ���������
#define PULL_POWER 60.0f

class CConnectTarget;
class CModel;

// �q�����I�u�W�F�N�g�̃x�[�X�N���X
class CConnectObject : public CObjectBase
{
public:
	// �R���X�g���N�^
	CConnectObject(float weight = 0.0f, 
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// �f�X�g���N�^
	~CConnectObject();

	// �I�u�W�F�N�g�폜��`����֐�
	void DeleteObject(CObjectBase* obj) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂��������̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�\</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// ��������ꂽ���̏���

	/// <summary>
	/// ��������ꂽ�Ƃ��̏���
	/// </summary>
	/// <param name="pullDir">�������������</param>
	/// <param name="opponentWeight">����̏d��</param>
	virtual void Pull(CVector pullDir, float opponentWeight);

	/// <summary>
	/// �q�������Ƃ��̏���
	/// </summary>
	/// <param name="other">����</param>
	virtual void Connect(CConnectObject* other);

	// �ڑ��^�[�Q�b�g�̍쐬
	void CreateTarget(CVector pos);

	// �ڑ��^�[�Q�b�g���擾
	std::vector<CConnectTarget*> GetTargets();

	// �d�����擾
	float GetWeight();

	// �d�͂��|���邩��ݒ�
	void SetGravity(bool isGravity);

	// �ڑ��I�u�W�F�N�g�̃^�O��ݒ�
	void SetConnectObjTag(EConnectObjTag tag);
	// �ڑ��I�u�W�F�N�g�̃^�O���擾
	EConnectObjTag GetConnectObjTag();

protected:
	// �ڑ��^�[�Q�b�g
	std::vector<CConnectTarget*> mTargets;

	// �d��
	float mWeight;

	CCollider* mpCol;	// �R���C�_�[

	CModel* mpModel;	// ���f��

	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �㉺�����̈ړ����x

	bool mIsGrounded;	// �ڒn���Ă��邩
	bool mIsGravity;	// �d�͂��|���邩

	// �ڑ��I�u�W�F�N�g�̃^�O
	EConnectObjTag mConnectObjTag;
};