#pragma once
#include "CColBase.h"
#include "CColliderRectangle.h"

class CColRectangle : public CColBase
{
public:
	// �R���X�g���N�^
	CColRectangle(const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
		const CColor& defaultColor, bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColRectangle();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self,CCollider* other,const CHitInfo& hit) override;

	// �`��
	void Render() override;

private:
	// �l�p�`�̃R���C�_�\
	CColliderRectangle* mpCollider;
	// �l�p�`���\������4���_
	CVector mVertices[4];
};