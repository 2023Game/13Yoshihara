#pragma once
#include "CConnectObject.h"

class CBox : public CConnectObject
{
public:
	// �R���X�g���N�^
	CBox(CVector defaultPos, float scaleRatio = 1.0f);
	// �f�X�g���N�^
	~CBox();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂��������̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�\</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �L�����ƏՓ˔��������R���C�_�[
	CCollider* mpCharaCol;

	// �������W
	CVector mDefaultPos;
	// ���X�|�[������
	bool mIsRespawn;
	// �o�ߎ���
	float mElapsedTime;
};