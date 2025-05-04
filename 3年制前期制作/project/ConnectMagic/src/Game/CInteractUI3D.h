#pragma once
#include "CObjectBase.h"

class CImage3D;

// 3D��Ԃɔz�u����C���^���N�gUI�̃C���[�W�N���X
class CInteractUI3D : public CObjectBase
{
public:
	// �R���X�g���N�^
	CInteractUI3D(CObjectBase* owner);
	// �f�X�g���N�^
	~CInteractUI3D();

	// �������ݒ�
	void SetOwner(CObjectBase* owner);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;		// �C���^���N�gUI�̎�����

	CImage3D* mpInteractUIImg;	// �C���^���N�gUI�̃C���[�W
};