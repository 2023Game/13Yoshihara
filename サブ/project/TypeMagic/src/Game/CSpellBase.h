#pragma once
#include "CObjectBase.h"
#include "CSpellStatus.h"
#include "SpellDefine.h"

class CSpellCaster;
class CModel;

// �����̊��N���X
// �I�u�W�F�N�g�x�[�X�Ǝ����̃X�e�[�^�X���p��
class CSpellBase : public CObjectBase, public CSpellStatus
{
	friend CSpellCaster;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="elemental">����</param>
	/// <param name="shape">�`</param>
	/// <param name="owner">������</param>
	/// <param name="target">�ڕW</param>
	/// <param name="deleteTime">���ł���܂ł̎���</param>
	CSpellBase(ESpellElementalType elemental, ESpellShapeType shape,
		CObjectBase* owner, CObjectBase* target, float deleteTime);
	// �f�X�g���N�^
	~CSpellBase();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �����̃^�[�Q�b�g��ݒ�
	void SetTarget(CObjectBase* target);

protected:
	// �ҋ@���̍X�V
	virtual void UpdateIdle();
	// ���˒��̍X�V
	virtual void UpdateShooting();

	// ���ł���܂ł̎���
	float mDeleteTime;

	// ���
	enum class EState
	{
		eIdle,			// �ҋ@��
		eShooting,		// ���˒�
	};
	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);
	EState mState;
	int mStateStep;	// ��ԓ��̃X�e�b�v
	float mElapsedTime;	// �o�ߎ���

	// �ړ����x
	CVector mMoveSpeed;

	// ���@�̃��f��
	CModel* mpModel;

	// �R���C�_�[
	CCollider* mpSpellCol;

	// ������
	CObjectBase* mpOwner;

	// �����̃^�[�Q�b�g
	CObjectBase* mpTarget;

	// �ړ�����
	CVector mMoveDir;
};