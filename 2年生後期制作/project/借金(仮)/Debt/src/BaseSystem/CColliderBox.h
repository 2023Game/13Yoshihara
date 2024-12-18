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
	/// <param name="min">�ŏ����W</param>
	/// <param name="max">�ő���W</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�\�̏d��</param>
	CColliderBox(CObjectBase* owner, ELayer layer,
		CVector min, CVector max,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// �{�b�N�X�R���C�_���\�����钸�_�̐ݒ�
	/// </summary>
	/// <param name="owner">�R���C�_�\�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="min">�ŏ����W</param>
	/// <param name="max">�ő���W</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�\�̏d��</param>
	void Set(CObjectBase* owner, ELayer layer,
		CVector min, CVector max);

	/// <summary>
	/// �{�b�N�X�̒��_���擾
	/// </summary>
	/// <param name="v0">�O�ʁ@�E��</param>
	/// <param name="v1">�O�ʁ@����</param>
	/// <param name="v2">�O�ʁ@����</param>
	/// <param name="v3">�O�ʁ@�E��</param>
	/// <param name="v4">�w�ʁ@�E��</param>
	/// <param name="v5">�w�ʁ@����</param>
	/// <param name="v6">�w�ʁ@����</param>
	/// <param name="v7">�w�ʁ@�E��</param>
	void Get(CVector* v0, CVector* v1, CVector* v2, CVector* v3,
		CVector* v4, CVector* v5, CVector* v6, CVector* v7) const;

	/// <summary>
	/// �{�b�N�X���\������l�p�`�R���C�_�̒��_���擾
	/// </summary>
	/// <param name="v0"></param>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="v3"></param>
	/// <param name="num"></param>
	void GetRectangle(CVector* v0, CVector* v1, CVector* v2, CVector* v3,
		int num);

	//�R���C�_�\�`��
	void Render() override;

protected:
	// �R���C�_�\�̏����X�V
	void UpdateCol() override;

private:
	CVector mMin;	// �ŏ����W
	CVector mMax;	// �ő���W
	CVector mV[8];	// �{�b�N�X�̒��_�̔z��
	CVector mWV[8];	// �{�b�N�X�̒��_�̃��[���h���W
};