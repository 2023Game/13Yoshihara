#pragma once
#include "CObjectBase.h"

// TODO�F�G���A�̉���
// ����ɐG���Ǝ����炻�̏ꏊ�Ń��X�|�[������G���A�̃N���X
class CRespawnArea : public CObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="respawnPos">���X�|�[���n�_</param>
	/// <param name="radius">�Փ˔���̔��a</param>
	CRespawnArea(CVector respawnPos, float radius);
	// �f�X�g���N�^
	~CRespawnArea();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
private:
	// �R���C�_�[�𐶐�
	void CreateCol(float radius);
	// �v���C���[���G���ƃ��X�|�[���n�_�ɐݒ肷��R���C�_�[
	CCollider* mpRespawnCol;
};