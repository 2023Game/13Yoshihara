#pragma once
#include "CFire.h"
#include "CItemDrop.h"

class CImage3D;

// �c�^�̍����ƕ�
#define VINE_HEIGHT 50.0f
#define VINE_WIDTH	100.0f

// ���̎��
enum class EGrassType
{
	eGrass,		// ��
	eVine,		// �c�^
};

// �΂ƂȂ���ƔR���鑐�I�u�W�F�N�g
class CGrass : public CFire, public CItemDrop
{
public:
	// �R���X�g���N�^(�����l�͒ʏ�̑��j
	CGrass(EGrassType type = EGrassType::eGrass,
		CVector fireOffsetPos = CVector::zero,
		float fireScale = FIRE_SCALE);
	// �f�X�g���N�^
	~CGrass();

	// �X�V
	void Update() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �R�����Ƃ��̏���
	void Burning() override;

	// �S�č폜
	void Delete() override;

	// �摜�𐶐�
	void CreateImg();

	// ���̉摜
	CImage3D* mpGrassImage;

	// ���̎��
	EGrassType mGrassType;

	// �c�^�p�̃R���C�_�[
	CCollider* mpVineCol;
};