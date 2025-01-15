#pragma once
#include "CCharaBase.h"

class CModel;
class CNavNode;
class CVehicleManager;

// �O���R���C�_�\�̍��W
#define FRONT_COL_POS CVector(0.0f,0.0f,60.0f)
// ���R���C�_�[�̍��W
#define LEFT_COL_POS	CVector(-40.0f,0.0f,0.0f)	// ��
#define RIGHT_COL_POS	CVector( 40.0f,0.0f,0.0f)	// �E

// �ԗ��̊��N���X
class CVehicleBase : public CCharaBase
{
	friend CVehicleManager;
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
	CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints);
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
	// �Ō�̏���|�C���g�܂ł̈ړ����I���������ǂ���
	bool GetMoveEnd() const;

	// ���Ă��邩
	bool IsBroken() const;

	// �O���ɒ�~���Ă���ԗ������邩�ǂ���
	bool GetIsFrontVehicle() const;

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

	// ����|�C���g�̃��X�g��ݒ肷��
	void SetPatrolPoints(std::vector<CNavNode*> patrolPoints);

	// �ԗ��̗L��������؂�ւ���
	void SetOnOff(bool setOnOff);
	// �ϐ������Z�b�g
	virtual void Reset();
protected:
	// �ǂ̓��ɂ����Ԃ�
	ERoadType mRoadType;
	// ���̓��̐i�ޕ���
	CVector mCurrentRoadRotation;
	// �ړ����Ă��邩
	bool mIsMove;
	// ���Ă��邩
	bool mIsBroken;

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
	// �O���ɎԂ����邩�̊m�F�p�R���C�_�\
	CCollider* mpFrontCol;
	// ���ɎԂ����邩�̊m�F�p�R���C�_�\
	CCollider* mpSideCol;
	// �o�H�T���p�̃R���C�_�\
	CCollider* mpNavCol;

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	// ���ɏ��񂷂�|�C���g��ύX
	void ChangePatrolPoint();
	/// <summary>
	/// �Ԑ��ύX�ňړ�����m�[�h�̍��W��ݒ肷��
	/// </summary>
	/// <param name="frontVehicle">�O�ɂ���Ԃ̃|�C���^</param>
	void SetChangeRoadPoint(CVehicleBase* frontVehicle);

	// �Ԑ��ύX���̈ړ���̃m�[�h
	CNavNode* mpChangeRoadPoint;

	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex;	// ���ɏ��񂷂�|�C���g�̔ԍ�

	std::vector<CNavNode*> mMoveRoute;	// ���߂��ŒZ�o�H�L���p
	int mNextMoveIndex;					// ���Ɉړ�����m�[�h�̃C���f�b�N�X�l

	bool mIsMoveEnd;	// �Ō�܂ňړ��������ǂ���
	bool mIsMovePause;	// �ړ��̒��f�����ǂ���

	bool mIsFrontVehicle;	// �O���ɒ�~���Ă���ԗ������邩�ǂ���
	bool mIsSideVehicle;	// ���Ɏԗ������邩�ǂ���
};