#pragma once
#include "CObjectBase.h"

class CImage3D;
class CTextUI3D;

// 3D��Ԃɔz�u����z�B��HPUI�N���X
class CDeliveryHpUI3D : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDeliveryHpUI3D(CObjectBase* owner);
	//�f�X�g���N�^
	~CDeliveryHpUI3D();

	// �������ݒ�
	void SetOwner(CObjectBase* owner);

	// ���ݒl��ݒ�
	void SetCurrPoint(int point);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;		// HPUI�̎�����

	CImage3D* mpDeliveryItemImg;// �z�B���̉摜
	CTextUI3D* mpNumText;		// �����Ă���z�B���̐��̃e�L�X�g 

	int mCurrPoint;		// ���ݒl
};