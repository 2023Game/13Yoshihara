#pragma once
#include "CCharaStatusBase.h"

// �z�B�̓G�X�e�[�^�X�N���X
class CDeliveryEnemyStatus : public CCharaStatusBase
{
public:
	// �R���X�g���N�^
	CDeliveryEnemyStatus();
	// �f�X�g���N�^
	~CDeliveryEnemyStatus();

	// �󂵂���Ⴆ�链�_���擾
	int GetScore() const;

	// ��b�ړ����x���擾
	float GetBaseMoveSpeed() const override;
	// �����鑬�x���擾
	float GetThrowSpeed() const;
private:
	// �󂵂���Ⴆ�链�_
	int mScore;
	// �����鑬�x
	float mThrowSpeed;
};