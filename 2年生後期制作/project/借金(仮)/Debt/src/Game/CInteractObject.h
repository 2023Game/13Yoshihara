#pragma once
#include "CRideableObject.h"
#include "CObjectBase.h"

class CInteractUI3D;

// ���ׂ�I�u�W�F�N�g�̃x�[�X�N���X
class CInteractObject : public CObjectBase
{
public:
	// �R���X�g���N�^
	CInteractObject(ETaskPriority prio=ETaskPriority::eDefault,
		int sortOrder=0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// �f�X�g���N�^
	virtual ~CInteractObject();

	// �I�u�W�F�N�g�폜��`����֐�
	void DeleteObject(CObjectBase* obj) override;

	// �X�V
	void Update();

	// ���ׂ���e�̃e�L�X�g��Ԃ�
	std::string GetInteractStr() const;

	// ���ׂ����Ԃ��ǂ���
	virtual bool CanInteract() const;
	// ���ׂ�i�p����Ŏ����j
	virtual void Interact() = 0;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂��������̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�\</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

#if _DEBUG
	// �f�o�b�O�\���p�̖��O���擾
	std::string GetDebugName() const;
	// �f�o�b�O�\���p�̖��O��ݒ�
	void SetDebugName(std::string name);
#endif

protected:
	std::string mInteractStr;	// ���ׂ���e�̃e�L�X�g
	CInteractUI3D* mpInteractUI3D;	// �C���^���N�gUI
	CVector mUIOffsetPos;			// UI�̃I�t�Z�b�g���W

#if _DEBUG
	std::string mDebugName;		// �f�o�b�O�\���p�̖��O
#endif
};