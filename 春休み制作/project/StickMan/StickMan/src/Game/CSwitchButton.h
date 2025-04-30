#pragma once
#include "CObjectBase.h"

class CSwitch;
class CModel;

// �X�C�b�`�̃{�^������
class CSwitchButton : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSwitchButton(CVector pos, CSwitch* owner);
	// �f�X�g���N�^
	~CSwitchButton();

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

private:
	// �R���C�_�[�𐶐�
	void CreateCol();

	enum class EState
	{
		eDefault,	// ������Ă��Ȃ�
		ePush,		// ������Ă���
	};
	EState mState;
	// ��Ԃ�ύX
	void ChangeState(EState state);

	// ������Ă��Ȃ����̍X�V����
	void UpdateDefault();
	// ������Ă��鎞�̍X�V����
	void UpdatePush();

	float mDefaultY;	// ������Y���W

	CCollider* mpPushCol;	// �����Ă邩�̓����蔻��

	CSwitch* mpOwner;	// ������

	CModel* mpModel;
};