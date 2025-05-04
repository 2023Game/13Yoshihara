#pragma once
#include "CColBase.h"
#include "CColliderMesh.h"

class CColMesh : public CColBase
{
public:
	// �R���X�g���N�^
	CColMesh(CModel* model, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColMesh();

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�\</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render() override;
private:
	// ���b�V���R���C�_�\
	CColliderMesh* mpCollider;
	// �O�p�R���C�_�̔z��
	std::list<STVertexData> mVertices;
};