#pragma once
#include "CWeapon.h"

class CModel;
class CConnectPoint;

// ��̃N���X
class CWand : public CWeapon
{
public:
	// �R���X�g���N�^
	CWand(CObjectBase* owner, ETag tag);
	// �f�X�g���N�^
	~CWand();

	// �X�V
	void Update() override;

	// �`��
	void Render() override;

private:
	CModel* mpModel;		// ��̃��f��
};
