#pragma once
#include "CSwitchObject.h"

// �X�C�b�`�œ�����
class CSwitchMoveWall : public CSwitchObject
{
public:
	// �R���X�g���N�^
	CSwitchMoveWall(CModel* model, CModel* col,
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime, bool isCrushed = false);
	// �f�X�g���N�^
	~CSwitchMoveWall();

	// �J���Ă��邩
	bool IsOpen() const;
	// �J���Ă��邩��ݒ�
	void SetIsOpen(bool enable);

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// �v���C���[�����܂ꂽ���p�̃R���C�_�[
	CColliderMesh* mpCrushedCol;

	// ��p���Ă��Ȃ����̏���
	void UpdateOff() override;
	// ��p���Ă��鎞�̏���
	void UpdateOn() override;

	// �ړ��̏���
	void Move();

	CVector mDefaultPos;	// �������W
	CVector mMoveVec;		// �ړ��x�N�g��
	float mMoveTime;		// �ړ�����
	float mElapsedTime;		// �o�ߎ���

	// �J���Ă��邩
	bool mIsOpen;
};