#pragma once
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
	/// <param name="v0">�O�ʍ���@�{�b�N�X�̒��_ 0</param>
	/// <param name="v1">�O�ʍ����@�{�b�N�X�̒��_ 1</param>
	/// <param name="v2">�O�ʉE���@�{�b�N�X�̒��_ 2</param>
	/// <param name="v3">�O�ʉE��@�{�b�N�X�̒��_ 3</param>
	/// <param name="v4">�w�ʍ���@�{�b�N�X�̒��_ 4</param>
	/// <param name="v5">�w�ʍ����@�{�b�N�X�̒��_ 5</param>
	/// <param name="v6">�w�ʉE���@�{�b�N�X�̒��_ 6</param>
	/// <param name="v7">�w�ʉE��@�{�b�N�X�̒��_ 7</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�\�̏d��</param>
	CColliderBox(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
		const CVector& v4, const CVector& v5, const CVector& v6, const CVector& v7,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// �{�b�N�X�R���C�_���\������l�p�`�R���C�_�̐ݒ�
	/// </summary>
	/// <param name="owner">�R���C�_�\�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�O�ʍ���@�{�b�N�X�̒��_ 0</param>
	/// <param name="v1">�O�ʍ����@�{�b�N�X�̒��_ 1</param>
	/// <param name="v2">�O�ʉE���@�{�b�N�X�̒��_ 2</param>
	/// <param name="v3">�O�ʉE��@�{�b�N�X�̒��_ 3</param>
	/// <param name="v4">�w�ʍ���@�{�b�N�X�̒��_ 4</param>
	/// <param name="v5">�w�ʍ����@�{�b�N�X�̒��_ 5</param>
	/// <param name="v6">�w�ʉE���@�{�b�N�X�̒��_ 6</param>
	/// <param name="v7">�w�ʉE��@�{�b�N�X�̒��_ 7</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
		const CVector& v4, const CVector& v5, const CVector& v6, const CVector& v7,
		bool isKinematic, float weight);
	// �l�p�`�R���C�_�[�̔z���Ԃ�
	const std::vector<CColliderRectangle>& Get() const;

	//�R���C�_�\�`��
	void Render() override;

protected:
	// �R���C�_�\�̏����X�V
	void UpdateCol() override;

private:
	// �l�p�`�R���C�_�\�̔z��쐬
	std::vector<CColliderRectangle> mRectangles;
};