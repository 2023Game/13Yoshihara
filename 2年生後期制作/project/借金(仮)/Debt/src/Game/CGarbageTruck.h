#pragma once
#include "CCharaBase.h"
#include "CGarbageTruckStatus.h"
#include "CVehicleBase.h"
#include "CGarbageTruckStatus.h"

class CModel;

// �S�~���W�Ԃ̃N���X
// �Ԃ̃N���X���p��
class CGarbageTruck : public CVehicleBase , public CGarbageTruckStatus
{
public:
	// �R���X�g���N�^
	CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation, 
		ERoadType road, std::vector<CNavNode*> patrolPoints);
	// �f�X�g���N�^
	~CGarbageTruck();

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
	// �������
	void UpdateCollect();

	// �Ԃ̏��
	enum class EState
	{
		eMove,		// �ړ�
		eStop,		// ��~
		eBroken,	// ����
		eChangeRoad,// �Ԑ��ύX
		eCollect,	// ���
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