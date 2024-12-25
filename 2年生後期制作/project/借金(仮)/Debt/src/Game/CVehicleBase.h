#pragma once
#include "CCharaBase.h"

class CModel;

// �ԗ��̊��N���X
class CVehicleBase : public CCharaBase
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
	/// <param name="moveSpeed">�ړ����x</param>
	/// <param name="isEnd">true�Ȃ�΁A�I������</param>
	void ChangeRoad(float moveSpeed, bool& isEnd);

	// �ǂ̓��ɂ����Ԃ���ύX����
	void ChangeRoadType(ERoadType roadType);
	// ���ǂ̓��ɂ��邩�擾����
	ERoadType GetRoadType() const;
protected:
	// �ǂ̓��ɂ����Ԃ�
	ERoadType mRoadType;

	// �ړ����x
	CVector mMoveSpeed;

	CModel* mpModel;

	// �{�̂̃R���C�_�\
	CCollider* mpBodyCol;
	// �O������R���C�_�\
	CCollider* mpFrontCol;
	// ������R���C�_�\
	CCollider* mpSideCol;
};