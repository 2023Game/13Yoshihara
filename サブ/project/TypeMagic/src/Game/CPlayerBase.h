#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CRideableObject.h"

class CGaugeUI2D;

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayerBase : public CXCharacter
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CPlayerBase* Instance();

	// �R���X�g���N�^
	CPlayerBase();
	// �f�X�g���N�^
	~CPlayerBase();

	// �I�u�W�F�N�g�폜����
	void DeleteObject(CObjectBase* obj) override;
	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

	// �`��
	void Render();

	// ���S�ɂ���ăQ�[�����I�����邩���擾����
	bool GetGameEnd();
	// ���S�ɂ���ăQ�[�����I�����邩��ݒ肷��
	void SetGameEnd(bool isEnd);

protected:
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(int type, bool restart = false);
	// �A�j���[�V�����u�����h�؂�ւ�
	bool ChangeAnimationBlend(int type, float blendSpeed, bool restart = false);

	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec();

	// �v���C���[�̃C���X�^���X
	static CPlayerBase* spInstance;

	CCollider* mpBodyCol;	// �{�̂̃R���C�_�\
	CCollider* mpAttackCol;	// �U���̃R���C�_�\

	// ���[�V�����u���[���|����c�莞��
	float mMotionBlurRemainTime;

	// Hp�Q�[�W
	CGaugeUI2D* mpHpGauge;

	// ���S���ăQ�[�����I�����鎞��true�ɂ���
	bool mIsGameEnd;

	// TODO�F�e�X�g���f��
	CModel* mpTestModel;
};
