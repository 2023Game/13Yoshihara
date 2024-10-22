#pragma once
#include "CRideableObject.h"
#include "CColliderSphere.h"

class CInteractObject : public CRideableObject
{
public:
	CInteractObject(float radius, std::string objectName);
	virtual ~CInteractObject();

	// �C���^���N�g����
	void Interact();
	
	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂��������̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�\</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

protected:
	CColliderSphere* mpColliderSphere;
	bool mIsInteract;		// �C���^���N�g���Ă��邩�ǂ���
	bool mIsInteractArea;	// �C���^���N�g�G���A�����ǂ���
};