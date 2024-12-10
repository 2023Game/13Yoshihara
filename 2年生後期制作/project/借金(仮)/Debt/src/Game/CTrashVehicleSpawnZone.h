#pragma once
#include "CCharaBase.h"
#include "CColliderMesh.h"

// �ԗ��̐����ꏊ�ɎԂ����邩���f���邽�߂̃N���X
class CTrashVehicleSpawnZone : public CCharaBase
{

private:
	CColliderMesh* mpSpawnZoneColliderMesh;

	// �����\���̃f�[�^
	struct IsCanPops
	{
		bool IsLeft1CanPop;		// ������1�Ԗڂ̓��ɐ����\��
		bool IsLeft2CanPop;		// ������2�Ԗڂ̓��ɐ����\��
		bool IsRight1CanPop;	// �E����1�Ԗڂ̓��ɐ����\��
		bool IsRight2CanPop;	// �E����2�Ԗڂ̓��ɐ����\��
	};
	IsCanPops mIsCanPops;	// �����\���̃f�[�^

	const float mLeft1PosX;	// ������1�Ԃ�X���W
	const float mLeft2PosX;	// ������2�Ԃ�X���W
	const float mRight1PosX;// �E����1�Ԃ�X���W
	const float mRight2PosX;// �E����2�Ԃ�X���W

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="left1">������1�Ԃ�X���W</param>
	/// <param name="left2">������2�Ԃ�X���W</param>
	/// <param name="right1">�E����1�Ԃ�X���W</param>
	/// <param name="right2">�E����2�Ԃ�X���W</param>
	CTrashVehicleSpawnZone(float left1, float left2, float right1, float right2);
	~CTrashVehicleSpawnZone();

	void Update();
	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �����\���̃f�[�^���擾
	IsCanPops GetCanPops() const;

	// �����\����ݒ�
	void SetLeft1CanPop(bool canPop);	// ������1��
	void SetLeft2CanPop(bool canPop);	// ������2��
	void SetRight1CanPop(bool canPop);	// �E����1��
	void SetRight2CanPop(bool canPop);	// �E����2��
	
};