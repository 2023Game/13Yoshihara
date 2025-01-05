#pragma once
#include "CCharaBase.h"
#include "CVehicleStatus.h"

class CModel;
class CNavNode;

// �ԗ��̊��N���X
class CVehicleBase : public CCharaBase , public CVehicleStatus
{
public:
	// �ǂ̓��ɂ����Ԃ�
	enum class ERoadType
	{
		None = -1,

		eLeft1,		// �������Ԗڂ̓�
		eLeft2,		// �������Ԗڂ̓�
		eRight1,	// �E�����Ԗڂ̓�
		eRight2,	// �E�����Ԗڂ̓�

	};

	// �R���X�g���N�^
	CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road);
	// �f�X�g���N�^
	~CVehicleBase();

	// �X�V
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// �`��
	void Render();

	/// <summary>
	/// �ړ������ǂ���
	/// </summary>
	/// <returns>true�Ȃ�΁A�ړ���</returns>
	bool IsMove() const;

	/// <summary>
	/// �Ԑ���ύX����
	/// </summary>
	/// <param name="isEnd">true�Ȃ�΁A�I������</param>
	void ChangeRoad(bool& isEnd);

	// �ǂ̓��ɂ����Ԃ���ύX����
	void ChangeRoadType(ERoadType roadType);
	// ���ǂ̓��ɂ��邩�擾����
	ERoadType GetRoadType() const;

	// �{�̃R���C�_�\���擾����
	CCollider* GetBodyCol() const;
	// �o�H�T���p�R���C�_�\���擾����
	CCollider* GetNavCol() const;
protected:
	// �ړ�����
	void UpdateMove();
	// ��~����
	void UpdateStop();
	// ��ꂽ����
	void UpdateBroken();
	// �Ԑ��ύX����
	void UpdateChangeRoad();

	// �g���b�N�̏��
	enum class EState
	{
		//	����
		eMove,		// �ړ�
		eStop,		// ��~
		eBroken,	// ����
		eChangeRoad,// �Ԑ��ύX

		// �g���b�N
		eCollect,	// ���
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �ԗ��̏��
#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
#endif

	// �ǂ̓��ɂ����Ԃ�
	ERoadType mRoadType;
	// ���̓��̐i�ޕ���
	CVector mCurrentRoadRotation;
	// �ړ����Ă��邩
	bool mIsMove;

	// �ړ����x
	CVector mMoveSpeed;

	CModel* mpModel;

	// �ԗ��̎���̃m�[�h
	CNavNode* mpNode0;
	CNavNode* mpNode1;
	CNavNode* mpNode2;
	CNavNode* mpNode3;

	// �{�̂̃R���C�_�\
	CCollider* mpBodyCol;
	// �o�H�T���p�̃R���C�_�\
	CCollider* mpNavCol;
};