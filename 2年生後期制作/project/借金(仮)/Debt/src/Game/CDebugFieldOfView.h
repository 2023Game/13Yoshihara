#pragma once
#include "CObjectBase.h"

// ����͈͂�\������f�o�b�O�@�\
class CDebugFieldOfView : public CObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�\������Ώ�</param>
	/// <param name="fovAngle">����͈͂̊p�x</param>
	/// <param name="fovLength">����͈͂̋���</param>
	CDebugFieldOfView(CObjectBase* owner,
		float fovAngle, float fovLength);
	// �f�X�g���N�^
	~CDebugFieldOfView();

	// ����͈͂�\������Ώۂ�ݒ�
	void SetOwner(CObjectBase* owner);
	// ����͈͂�ݒ�
	void Set(float angle, float length);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;	// �\������Ώۂ̃|�C���^
	float mFovAngle;		// ����͈͂̊p�x�i�f�B�O���[�j
	float mFovLength;		// ����͈͂̋���
};