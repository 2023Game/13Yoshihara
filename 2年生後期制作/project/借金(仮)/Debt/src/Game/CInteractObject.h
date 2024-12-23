#pragma once
#include "CRideableObject.h"
#include "CColliderSphere.h"
class CBillBoardUI;
class CObjectBase;

// ���ׂ�I�u�W�F�N�g�̃x�[�X�N���X
class CInteractObject : public CObjectBase
{
public:
	CInteractObject(ETaskPriority prio=ETaskPriority::eDefault,
		int sortOrder=0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	virtual ~CInteractObject();

	// ���ׂ���e�̃e�L�X�g��Ԃ�
	std::string GetInteractStr() const;

	// ���ׂ����Ԃ��ǂ���
	virtual bool CanInteract() const;
	// ���ׂ�i�p����Ŏ����j
	virtual void Interact() = 0;

	// �C���^���N�g����
	void Interact();
	
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
	CBillBoardUI* mpBillBoardUI;
	CObjectBase* mpPlayer;
	bool mIsInteract;		// �C���^���N�g���Ă��邩�ǂ���
	bool mIsInteractArea;	// �C���^���N�g�G���A�����ǂ���
	std::vector<std::string> mMenuItemPathList;	// ���j���[�̃A�C�e���p�摜�̃p�X���X�g
	std::string mMenuSelectPath;	// ���j���[�̃Z���N�g�摜�̃p�X
#if _DEBUG
	std::string mDebugName;		// �f�o�b�O�\���p�̖��O
#endif
};