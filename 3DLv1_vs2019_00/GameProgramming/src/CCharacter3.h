#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//�ϊ��s��N���X�̃C���N���[�h
#include "CTransform.h"
//���f���N���X�̃C���N���[�h
#include "CModel.h"
#include "CTask.h"
class CCollider;
/*
�L�����N�^�[�N���X
�Q�[���L�����N�^�̊�{�I�ȋ@�\���`����
*/
class CCharacter3 :public CTransform ,public CTask
{
public:
	//�R���X�g���N�^
	CCharacter3(int priority);
	//�Փˏ���
	virtual void Collision(CCollider* m, CCollider* o) {}
	//�R���X�g���N�^
	CCharacter3();
	//�f�X�g���N�^
	~CCharacter3();
	//���f���̐ݒ�
	//Model(CModel*m);
	void Model(CModel* m);
	//�`�揈��
	void Render();
	//�L�����N�^�̃^�O
	enum class ETag {
		EPLAYER,//�v���C���[
		EENEMY,//�G
	};
	ETag Tag();//�^�O�̎擾
protected:
	CModel* mpModel;//���f���̃|�C���^
	ETag mTag;//�L�����N�^�̃^�O
};
#endif