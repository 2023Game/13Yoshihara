#include "CBullet.h"

#define VELOCITY CVector(0.0f,0.0f,1.0f)

//���Ɖ��s���̐ݒ�
//Set�i���A���s���j
void CBullet::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//�O�p�`�̒��_�ݒ�
	mT.Vertex(CVector(w, 0, 0), CVector(0, 0, -d), CVector(-w, 0, 0));
	//�O�p�`�̖@���ݒ�
	mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//�X�V
void CBullet::Update()
{
	CTransform::Update();
	//�ʒu�X�V
	mPosition = mPosition + VELOCITY * mMatrixRotate;
}

//�`��
void CBullet::Render()
{
	//DIFFUSE���F�ݒ�
	float c[] = { 1.0f,1.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//�O�p�`�`��
	mT.Render(mMatrix);
}



