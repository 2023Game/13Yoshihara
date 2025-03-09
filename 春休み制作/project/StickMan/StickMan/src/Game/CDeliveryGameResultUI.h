#pragma once
#include "CResultUIBase.h"

// �z�B�̃��U���gUI�N���X
class CDeliveryGameResultUI : public CResultUIBase
{
public:
	// �R���X�g���N�^
	CDeliveryGameResultUI();
	// �f�X�g���N�^
	~CDeliveryGameResultUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �z�B���̉摜
	CImage* mpDeliveryItemImg;
	// �g���b�N�̉摜
	CImage* mpDeliveryTruckImg;
};