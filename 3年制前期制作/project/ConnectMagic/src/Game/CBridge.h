#pragma once
#include "CConnectObject.h"

// ��
class CBridge : public CConnectObject
{
public:
	// �R���X�g���N�^
	CBridge();
	// �f�X�g���N�^
	~CBridge();

	// �������鏈��
	void Pull(CVector pullDir, float opponentWeight) override;

	// �X�V����
	void Update() override;

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