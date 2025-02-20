#pragma once
#include "CTrashEnemy.h"

class CPunisherTrashEnemy : public CTrashEnemy
{
public:
	// �R���X�g���N�^
	CPunisherTrashEnemy();
	// �f�X�g���N�^
	~CPunisherTrashEnemy();

private:
	// �R���C�_�[���쐬����
	void CreateCol() override;
	// �����Ԃ��瑼�̏�Ԃֈڍs����������`�F�b�N
	// ���d�u���p�͂����ǂ�������
	bool ChangePatrolToOther() override;
	// �ǐՏ�Ԃ��瑼�̏�Ԃֈڍs����������`�F�b�N
	// ���d�u���p�͒ǂ�����������
	bool ChangeChaseToOther() override;
	// Hp�Q�[�W�̍X�V
	void UpdateHpGauge() override;

};