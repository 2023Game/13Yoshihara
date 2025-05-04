#pragma once
#include "CCollider.h"

/// <summary>
/// �l�p�`�R���C�_�\
/// </summary>
class CColliderRectangle : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�\�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�l�p�`�̒��_ 0</param>
	/// <param name="v1">�l�p�`�̒��_ 1</param>
	/// <param name="v2">�l�p�`�̒��_ 2</param>
	/// <param name="v3">�l�p�`�̒��_ 3</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�\�̏d��</param>
	CColliderRectangle(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, 
		const CVector& v2, const CVector& v3,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// �l�p�`�R���C�_�\�̐ݒ�
	/// </summary>
	/// <param name="owner">�R���C�_�\�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�l�p�`�̒��_ 0</param>
	/// <param name="v1">�l�p�`�̒��_ 1</param>
	/// <param name="v2">�l�p�`�̒��_ 2</param>
	/// <param name="v3">�l�p�`�̒��_ 3</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, 
		const CVector& v2, const CVector& v3);

	/// <summary>
	/// �l�p�`�̒��_���擾
	/// </summary>
	/// <param name="v0">���_1�i�[�p</param>
	/// <param name="v1">���_2�i�[�p</param>
	/// <param name="v2">���_3�i�[�p</param>
	/// <param name="v3">���_4�i�[�p</param>
	void Get(CVector* v0, CVector* v1,
		CVector* v2, CVector* v3) const;

	// �R���C�_�\�`��
	void Render() override;
protected:
	// �R���C�_�\�̏����X�V
	void UpdateCol() override;
private:
	CVector mV[4];	// �l�p�`�̒��_�̔z��
	CVector mWV[4];	// �l�p�`�̒��_�̃��[���h���W
};