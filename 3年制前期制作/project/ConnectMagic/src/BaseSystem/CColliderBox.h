#pragma once
#include <list>
#include "CCollider.h"

/// <summary>
/// �{�b�N�X�R���C�_
/// </summary>
class CColliderBox : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�\�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="min">�ŏ����W</param>
	/// <param name="max">�ő���W</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�\�̏d��</param>
	CColliderBox(CObjectBase* owner, ELayer layer,
		CVector min, CVector max,
		bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColliderBox();

	/// <summary>
	/// �{�b�N�X�R���C�_���\�����钸�_�̐ݒ�
	/// </summary>
	/// <param name="min">�ŏ����W</param>
	/// <param name="max">�ő���W</param>
	void Set(CVector min, CVector max);
	// �l�p�`�R���C�_�̔z����擾
	const std::vector<SRVertexData>& Get() const;

	//�R���C�_�\�`��
	void Render() override;

protected:
	// �R���C�_�\�̏����X�V
	void UpdateCol() override;

private:
	// �l�p�`�R���C�_�̔z��쐬
	std::vector<SRVertexData> mVertices;
};