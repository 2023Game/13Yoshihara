#pragma once
#include "CEnemyBase.h"
#include "CEnemyStatus.h"
#include "CCastSpellStr.h"
#include "CEnemyContext.h"

class CEnemyStateBase;

// �G�̊��N���X�ƃX�e�[�^�X�N���X
// ���p�������G�N���X
class CEnemy : public CEnemyBase, public CEnemyStatus, public CCastSpellStr
{
	friend CEnemyStateBase;
public:
	// �R���X�g���N�^
	CEnemy(ESpellElementalType elemental = ESpellElementalType::eNeutral);
	// �f�X�g���N�^
	~CEnemy();

	// �X�V
	void Update() override;

	// ���C�������̕������ݒ�
	void SetMainElementalStr(ESpellElementalType elemental);
	// ���C�������̕�������擾
	std::string GetMainElementalStr() const;

	// �r������`�̕������ݒ�
	void SetCastShapeStr(ESpellShapeType shape);
	// �r������`�̕�������擾
	std::string GetCastShapeStr() const;

	// ��ԓ��̃X�e�b�v��ݒ�
	void SetStateStep(int step);
	// ��ԓ��̃X�e�b�v���擾
	int GetStateStep() const;
	// �o�ߎ��Ԃ�ݒ�
	void SetElapsedTime(float time);
	// �o�ߎ��Ԃɉ��Z
	void AddElapsedTime(float addTime);
	// �o�ߎ��Ԃ��擾
	float GetElapsedTime() const;

	// ���������ł��Ă��邩��ݒ�
	void SetSpellComing(bool enable, ESpellShapeType shape = ESpellShapeType::eError, float score = 0.0f, CVector moveDir = CVector::zero, float dist = 0.0f);
	// ���ł��Ă�������̈ړ��������擾
	CVector GetComingSpellMoveDir() const;

	// �œK�ȍs���ɕύX����
	void ChangeBestState();

	// �󋵂��擾
	CEnemyContext::EnemyContext GetContext();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ���S����
	void Death() override;

	// ��Ԑ؂�ւ�
	void ChangeState(CEnemyStateBase* state);
	CEnemyStateBase* mState;	// �G�̏��
	// ��ԓ��̃X�e�b�v�Ǘ��p
	int mStateStep;
	// �o�ߎ���
	float mElapsedTime;

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(CEnemyStateBase* state) const;
#endif

	// ���C�������̕�����
	std::string mMainElementalStr;
	// �r������`�̕�����
	std::string mCastShapeStr;

	// ���ł�������T�m�p
	CCollider* mpSpellSearch;
	// ���������ł��Ă��邩
	bool mIsSpellComing;
	// ���ł��Ă�������̌`
	ESpellShapeType mComingSpellShape;
	// �����̉��D��x�X�R�A
	float mPriorityScore;
	// �����̈ړ�����
	CVector mSpellMoveDir;
	// ���ł��Ă�������Ƃ̋���
	float mSpellDist;

	// MP�Q�[�W
	CGaugeUI3D* mpMpGauge;
};