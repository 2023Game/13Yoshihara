#pragma once
#include "CResultUIBase.h"

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
};