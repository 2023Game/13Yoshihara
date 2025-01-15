#pragma once
#include "CVehicleBase.h"
#include "CVehicleStatus.h"

class CModel;

// �Ԃ̃N���X
class CCar : public CVehicleBase, public CVehicleStatus
{
public:
	// �R���X�g���N�^
	CCar(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints);
	// �f�X�g���N�^
	~CCar();

	// �X�V
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// �`��
	void Render() override;

	// �ϐ������Z�b�g
	void Reset() override;
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
	EState mState;	// �ԗ��̏��
	int mStateStep;	// ��ԓ��̃X�e�b�v��
	float mElapsedTime;	// �o�ߎ���

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
#endif
};