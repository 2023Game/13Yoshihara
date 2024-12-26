#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

// �ԗ��̐����ꏊ�ɎԂ����邩���f���邽�߂̃N���X
class CTrashVehicleSpawnZone : public CObjectBase
{

private:
	CColliderMesh* mpSpawnZoneColliderMesh;

	// �����\���̃f�[�^
	// true�Ȃ�΁A�����\
	struct IsCanPops
	{
		bool IsLeft1CanPop;		// ������1�Ԗڂ̓��ɐ����\��
		bool IsLeft2CanPop;		// ������2�Ԗڂ̓��ɐ����\��
		bool IsRight1CanPop;	// �E����1�Ԗڂ̓��ɐ����\��
		bool IsRight2CanPop;	// �E����2�Ԗڂ̓��ɐ����\��
	};
	IsCanPops mIsCanPops;	// �����\���̃f�[�^
public:
	// �R���X�g���N�^
	CTrashVehicleSpawnZone();
	// �f�X�g���N�^
	~CTrashVehicleSpawnZone();

	void Update();
	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	/// <summary>
	/// �����\���̃f�[�^���擾
	/// </summary>
	/// <returns>�f�[�^��true�Ȃ�΁A�����\</returns>
	IsCanPops GetCanPops() const;

	// �����\����ݒ�
	void SetLeft1CanPop(bool canPop);	// ������1��
	void SetLeft2CanPop(bool canPop);	// ������2��
	void SetRight1CanPop(bool canPop);	// �E����1��
	void SetRight2CanPop(bool canPop);	// �E����2��
	
};