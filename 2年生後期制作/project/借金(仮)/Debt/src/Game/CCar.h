#pragma once
#include "CVehicleBase.h"
#include "CCarStatus.h"

class CModel;

// �Ԃ̃N���X
class CCar : public CVehicleBase , public CCarStatus
{
public:
	// �R���X�g���N�^
	CCar(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road);
	// �f�X�g���N�^
	~CCar();

	// �X�V
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

private:
	// �ړ�����
	void UpdateMove();
	// ��~����
	void UpdateStop();
	// ��ꂽ����
	void UpdateBroken();
	// �Ԑ��ύX����
	void UpdateChangeRoad();

	// �Ԃ̏��
	enum class EState
	{
		eMove,		// �ړ�
		eStop,		// ��~
		eBroken,	// ����
		eChangeRoad,// �Ԑ��ύX
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �Ԃ̏��

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
#endif
};