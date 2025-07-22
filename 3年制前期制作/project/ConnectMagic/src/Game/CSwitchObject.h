#pragma once
#include "CObjectBase.h"

class CModel;

// �X�C�b�`�ō�p����I�u�W�F�N�g�̃x�[�X�N���X
class CSwitchObject : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSwitchObject(ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// �f�X�g���N�^
	virtual ~CSwitchObject();

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

	// �I���I�t��؂�ւ���
	virtual void SetOnOff(bool isOnOff);

protected:
	// �I���I�t�̏��
	enum class EState
	{
		eOff,
		eOn,
	};
	EState mState;
	// ��Ԃ̕ύX
	void ChangeState(EState state);	
	
	// ��p���Ă��Ȃ����̏���
	virtual void UpdateOff();
	// ��p���Ă��鎞�̏���
	virtual void UpdateOn();

	CModel* mpModel;

	CCollider* mpCol;
};