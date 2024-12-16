#pragma once
#include "CColBase.h"
#include "CColliderTriangle.h"

class CColTriangle : public CColBase
{
public:
	// �R���X�g���N�^
	CColTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColTriangle();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render() override;

private:
	// �O�p�`�R���C�_�[
	CColliderTriangle* mpCollider;
	CVector mVertices[3];	// �O�p�`���\������3���_
};