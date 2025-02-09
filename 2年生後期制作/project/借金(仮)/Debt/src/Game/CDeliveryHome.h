#pragma once
#include "CObjectBase.h"

class CModel;

class CDeliveryHome : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDeliveryHome();
	// �f�X�g���N�^
	~CDeliveryHome();

	// �X�V
	void Update();
	// �`��
	void Render();
private:
	// 3d���f��
	CModel* mpModel;
	// �ǂ̃R���C�_�\
	CCollider* mpWallCol;
	// �S�[������̃R���C�_�[
	CCollider* mpGoalCol;
};