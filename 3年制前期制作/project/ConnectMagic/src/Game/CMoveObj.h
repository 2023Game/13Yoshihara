#pragma once
#include "CRideableObject.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CMoveObj : public CRideableObject
{
public:
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

	enum class EMoveState
	{
		eStop,	// �~�܂��Ă���
		eGo,	// �i��ł���
		eBack,	// �߂��Ă���
	};
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