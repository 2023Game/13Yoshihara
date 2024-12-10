#pragma once
#include "CCharaBase.h"
#include "CCarStatus.h"

class CColliderCapsule;
class CModel;

// �Ԃ̃N���X
class CCar : public CCharaBase
{
public:
	CCar(CModel* model, const CVector& pos, const CVector& rotation);
	~CCar();

	void Update();
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	void Render();

protected:
	// �ړ�����
	void UpdateMove();
	// ��ꂽ����
	void UpdateBroken();

	CCarStatus mCarStatus;	// �Ԃ̃X�e�[�^�X
	CModel* mpModel;
	CColliderCapsule* mpColliderCapsule;

	enum class EState
	{
		eMove,		// �ړ�
		eStop,		// ��~
		eBroken,	// ����
		eCollect,	// ���
	};
	// �X�e�[�g��ύX
	void ChangeState(EState state);
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
	EState mState;
};