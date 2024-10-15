#pragma once
#include "CRideableObject.h"
#include "CColliderSphere.h"

class CInteractObject : public CRideableObject
{
public:
	CInteractObject();
	virtual ~CInteractObject();
	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂��������̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�\</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
private:
	bool interact;//�C���^���N�g���ꂽ���ǂ���
	CColliderSphere* mpColliderSphere;
};