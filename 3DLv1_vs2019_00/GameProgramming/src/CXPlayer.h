#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CActionCamera.h"

class CXPlayer : public CXCharacter
{
public:
	//�R���C�_�̏�����
	CXPlayer();
	//�X�V
	void Update();
	//�����s��̐ݒ�
	void Init(CModelX* model);
	//�Փ˔���
	void Collision(CCollider* m, CCollider* o);
private:
	CInput mInput;
	//�R���C�_�̐錾
	CCollider mColSphereHead;//��
	CCollider mColSphereBody;//��
	CCollider mColSphereSword;//��
	CColliderCapsule mColBody;//��
};
#endif