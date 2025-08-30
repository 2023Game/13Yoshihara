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

	// �����X�C�b�`�ɒ���t���Ă��邩��ݒ�
	void SetIsAttach(bool enable);
	// �����X�C�b�`�ɒ���t���Ă��邩
	bool GetIsAttach() const;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �������W
	CVector mDefaultPos;
	// ���X�|�[������
	bool mIsRespawn;
	// �o�ߎ���
	float mElapsedTime;

	// �����X�C�b�`�ɒ���t���Ă��邩
	bool mIsAttach;
};