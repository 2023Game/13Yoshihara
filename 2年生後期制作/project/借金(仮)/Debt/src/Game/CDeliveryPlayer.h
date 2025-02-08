#pragma once
#include "CPlayerBase.h"
#include "CCharaStatusBase.h"

/*
�z�B�Q�[���̃v���C���[�N���X
*/
class CDeliveryPlayer : public CPlayerBase , public CCharaStatusBase
{
public:
	// �R���X�g���N�^
	CDeliveryPlayer();
	// �f�X�g���N�^
	~CDeliveryPlayer();

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);// override;

	// �`��
	void Render();

private:
	// ���
	enum class EState
	{
		eMove,			// �ړ�
		eChangeRoad,	// �Ԑ��ύX
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �v���C���[�̏��
	int mStateStep;				// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;			// �o�ߎ��Ԍv���p

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
#endif
	// �ړ��̍X�V����
	void UpdateMove();
	// �Ԑ��ύX�̍X�V����
	void UpdateChangeRoad();

	// �R���C�_�\�𐶐�
	void CreateCol();
	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	// �A�N�V�����̃L�[����
	void ActionInput();

	// 3d���f��
	CModel* mpModel;
	// �Ԑ��ύX�̖ړI�n
	CVector mTargetPos;
};