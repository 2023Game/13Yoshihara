#pragma once
#include "CSwitchObject.h"

class CModel;

// �X�C�b�`�ō�p����h�A
class CDoor : public CSwitchObject
{
public:
	// �R���X�g���N�^
	CDoor(CVector rot);
	// �f�X�g���N�^
	~CDoor();

	// �`��
	void Render() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ��p���Ă��Ȃ����̏���
	void UpdateOff() override;
	// ��p���Ă��鎞�̏���
	void UpdateOn() override;

	CModel* mpModel;

	CCollider* mpCol;

	CVector mDefaultRotation;	// �����̉�]
};