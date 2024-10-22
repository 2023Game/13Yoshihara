#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

class CFlamethrower;

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer : public CXCharacter
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();

	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();

	//// mIsInteract�̒l��ݒ肷��
	//// CInteractObject�Őݒ�
	//void SetInteract(bool interact);
	////mIsInteract�̒l��Ԃ�
	//bool GetInteract();

	//// �C���^���N�g�I�u�W�F�N�g��ݒ肷��
	//// CInteractObject�Őݒ�
	//void SetInteractObject(std::string interactName);
	//// �v���C���[�̃C���^���N�g�I�u�W�F�N�g��Ԃ�
	//std::string GetInteractObject();
private:
	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec() const;

	// �ҋ@���
	void UpdateIdle();
	// �U��
	void UpdateAttack();
	// �U���I���҂�
	void UpdateAttackWait();
	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
	//// �C���^���N�g��
	//void UpdateInteract();

	// �ړ��̍X�V����
	void UpdateMove();

	// ���[�V�����u���[�̍X�V����
	void UpdateMotionBlur();

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eWalk,		// ���s
		eAttack,	// �U��
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �v���C���[�̃C���X�^���X
	static CPlayer* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eAttack,	// �U��
		eAttackWait,// �U���I���҂�
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
		//eInteract,	//�C���^���N�g��
	};
	EState mState;	// �v���C���[�̏��

	//// �C���^���N�g���̃I�u�W�F�N�g
	//enum class EInteractObject
	//{
	//	None = -1,

	//	eComputer,	//�p�\�R��
	//	eDoor,		//�h�A
	//};
	//EInteractObject mInteractObject;	//�C���^���N�g���̃I�u�W�F�N�g

	//bool mIsInteract; // �C���^���N�g�͈͓����ǂ���

	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��

	CColliderCapsule* mpColliderCapsule;
	CColliderLine* mpColliderLine;
	CColliderSphere* mpInteractColliderSphere;
	CTransform* mpRideObject;

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	// �Ή����˃G�t�F�N�g
	CFlamethrower* mpFlamethrower;

	// ���[�V�����u���[���|����c�莞��
	float mMotionBlurRemainTime;
};
