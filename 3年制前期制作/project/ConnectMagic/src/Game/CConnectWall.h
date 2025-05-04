#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

// �����q���ǃN���X
class CConnectWall : public CObjectBase
{
public:
	// �R���X�g���N�^
	CConnectWall();
	// �f�X�g���N�^
	~CConnectWall();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// ������]�l��ݒ�
	void SetDefaultRot(CVector rot);

	// �J����
	void OpenClose();

private:
	// �J��
	void Open();
	// ����
	void Close();

	// �q���ǂ̏��
	enum class EState
	{
		eIdle,	// �ҋ@���
		eOpen,	// �J�����
		eClose,	// ������
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p

	// �R���C�_�\�𐶐�
	void CreateCol();

	// �R���C�_�\
	CCollider* mpMeshCol;
	// ���f��
	CModel* mpModel;

	// �J���Ă��邩
	bool mIsOpen;
	// ������]�l�i�����Ă����ԁj
	CVector mDefaultRot;
	// ����]��
	CVector mTotalRot;
};