#pragma once
#include "CRideableObject.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "MoveState.h"

class CMoveObj : public CRideableObject
{
public:
	// ��Ԃ�ݒ�
	void SetState(EMoveState state);
	// ��Ԃ��擾
	EMoveState GetState() const;
	// �O��̏�Ԃ�ݒ�
	void SetPreState(EMoveState state);
	// �O��̏�Ԃ��擾
	EMoveState GetPreState() const;
	// �o�ߎ��Ԃ�ݒ�
	void SetElapsedTime(float time);
	// �o�ߎ��Ԃ��擾
	float GetElapsedTime() const;

	CMoveObj(CModel* model, CModel* col,
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime,
		ELayer layer, bool isCrushed = false);
	~CMoveObj();

	void Update();
	void Render();

private:
	CModel* mpModel;
	// �{�̃R���C�_�\
	CColliderMesh* mpColliderMesh;
	// �v���C���[�����܂ꂽ���p�̃R���C�_�[
	CColliderMesh* mpCrushedCol;

	CVector mDefaultPos;
	CVector mMoveVec;
	float mMoveTime;

	// ��Ԃ�ύX
	void ChangeState(EMoveState state);
	EMoveState mState;		// ���
	EMoveState mPreState;	// ��O�̏��
	float mElapsedTime;

	// �~�܂��Ă���Ƃ��̍X�V
	void UpdateStop();
	// �i��ł���Ƃ��̍X�V
	void UpdateGo();
	// �߂��Ă���Ƃ��̍X�V
	void UpdateBack();
};