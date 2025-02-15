#pragma once
#include "CObjectBase.h"

class CImage;
class CTextUI2D;

// 2D��Ԃɔz�u����z�B��HPUI�N���X
class CDeliveryHpUI2D : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDeliveryHpUI2D(CObjectBase* owner);
	// �f�X�g���N�^
	~CDeliveryHpUI2D();

	// �������ݒ�
	void SetOwner(CObjectBase* owner);

	// ���ݒl��ݒ�
	void SetCurrPoint(int point);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;	// HPUI�̎�����

	CImage* mpDeliveryItemImg;		// �z�B���̃C���[�W
	CTextUI2D* mpNumText;			// �����Ă���z�B���̐��̃e�L�X�g

	int mCurrPoint;	// ���ݒl
};