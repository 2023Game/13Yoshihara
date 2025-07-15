#pragma once
#include "CConnectObject.h"

// ��
class CBridge : public CConnectObject
{
public:
	// �R���X�g���N�^
	CBridge(float defaultAngle = 0.0f);
	// �f�X�g���N�^
	~CBridge();

	// �X�V����
	void Update() override;

	// �q�������Ƃ��̏���
	void Connect(CVector wandPointPos) override;

	// �w��p�x�ɐݒ�
	void SetAngle(float angle);

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	
	/// <summary>
	/// �p�x�������X����
	/// </summary>
	/// <param name="angle">�X����p�x</param>
	/// <param name="dir">�͂̉�����Ă������</param>
	void RotateBridge(float angle, CVector dir);

	// ���݂̌X��
	float mCurrentAngle;
};