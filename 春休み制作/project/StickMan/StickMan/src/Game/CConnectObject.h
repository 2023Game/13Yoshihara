#pragma once
#include "CObjectBase.h"

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
	virtual ~CConnectObject();

	// �I�u�W�F�N�g�폜��`����֐�
	void DeleteObject(CObjectBase* obj) override;

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
	virtual void Pull();

	// �ڑ��^�[�Q�b�g�̍쐬
	void CreateTarget(CVector pos);

protected:
	// �ڑ��^�[�Q�b�g
	std::vector<CConnectTarget*> mTargets;

	// �d��
	float mWeight;

	CCollider* mpCol;	// �R���C�_�[

	CModel* mpModel;	// ���f��

	float mMoveSpeedY;	// �㉺�����̈ړ����x
};