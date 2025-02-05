#include "CTrashStatusBase.h"
#include "Maths.h"
#include "CTrashBag.h"

// �R���X�g���N�^
CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockbackDealt, int attackPower, int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed, attackPower)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(CVector::zero)
	, mTrashBagNum(0)
	, mGoldTrashBagNum(0)
{
}

// �f�X�g���N�^
CTrashStatusBase::~CTrashStatusBase()
{
}

// �S�ẴX�e�[�^�X��ݒ�
void CTrashStatusBase::SetAllStatus(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockbackDealt, int attackPower, int power)
{
	SetMaxHp(maxHp);
	SetBaseMoveSpeed(baseMoveSpeed);
	SetJumpSpeed(jumpSpeed);
	SetKnockbackDealt(knockbackDealt);
	SetAttackPower(attackPower);
	SetPower(power);
}

// �^����m�b�N�o�b�N���x���擾
float CTrashStatusBase::GetKnockbackDealt() const
{
	return mKnockbackDealt;
}

// �^����m�b�N�o�b�N���x��ݒ�
void CTrashStatusBase::SetKnockbackDealt(float knockbackDealt)
{
	mKnockbackDealt = knockbackDealt;
}

// �󂯂�m�b�N�o�b�N���擾
CVector CTrashStatusBase::GetKnockbackReceived() const
{
	return mKnockbackReceived;
}

// �󂯂�m�b�N�o�b�N��ݒ�
void CTrashStatusBase::SetKnockbackReceived(CVector knockbackReceived)
{
	mKnockbackReceived = knockbackReceived;
}

// �S�~�𗎂Ƃ��͂��擾
int CTrashStatusBase::GetPower() const
{
	return mPower;
}

// �S�~�𗎂Ƃ��͂�ݒ�
void CTrashStatusBase::SetPower(int power)
{
	mPower = power;
}

// �S�~�܂̐������Z����
void CTrashStatusBase::SetTrashBag(int num)
{
	mTrashBagNum += num;
	// 0��菬�����Ȃ�����0�ɂ���
	if (mTrashBagNum < 0) mTrashBagNum = 0;
}

// �S�~�܂̐����擾����
int CTrashStatusBase::GetTrashBag() const
{
	return mTrashBagNum;
}

// �S�[���h�S�~�܂̐������Z����
void CTrashStatusBase::SetGoldTrashBag(int num)
{
	mGoldTrashBagNum += num;
	// 0��菬�����Ȃ�����0�ɂ���
	if (mGoldTrashBagNum < 0) mGoldTrashBagNum = 0;
}

// �S�[���h�S�~�܂̐����擾����
int CTrashStatusBase::GetGoldTrashBag() const
{
	return mGoldTrashBagNum;
}

// �S�~�܂𗎂Ƃ�����
void CTrashStatusBase::DropTrashBag(int power,
	CVector pos, CVector front, CVector side,
	CVector offsetPos)
{
	// �S�~�܂𗎂Ƃ��͂�0�ȉ��ɂȂ�܂ŌJ��Ԃ�
	// �S�~�܂𗎂Ƃ����Ƃ��o������dropPower�����炷
	for (int dropPower = power; dropPower > 0; dropPower)
	{
		// �����̃S�~�܂̏�������0�Ȃ珈�����Ȃ�
		if (GetTrashBag() + GetGoldTrashBag() <= 0) return;
		// ���Ƃ���������
		int random = Math::Rand(0, 1);
		// �Е���0�Ȃ�A���Ƃ��̂͂�������m��
		if (GetTrashBag() <= 0) random = 1;
		if (GetGoldTrashBag() <= 0) random = 0;

		switch (random)
		{
		case 0:
			// �S�~�܂���ł��������Ă����痎�Ƃ�
			if (GetTrashBag() > 0)
			{
				// X�̃I�t�Z�b�g���W�͔͈͓��Ń����_���Ɍ��߂�
				float randomX = Math::Rand(-offsetPos.X(), offsetPos.X());
				Math::Clamp(randomX, -offsetPos.X(), offsetPos.X());
				// �����Ă���S�~�܂̐���1���炷
				SetTrashBag(-1);
				// �S�~�܂𐶐�
				CTrashBag* trashBag = new CTrashBag(false);
				// �i�I�t�Z�b�g���W�~���Ƃ��́j�����炵���ʒu�ɐ���
				CVector offset = CVector(0.0f, offsetPos.Y(), 0.0f);
				offset += side * randomX;
				offset += front * offsetPos.Z();
				trashBag->Position(pos + offset * dropPower);
				// ���ł������x��ݒ�
				trashBag->SetThrowSpeed(front * GetKnockbackDealt(), GetKnockbackDealt());
				// ���Ƃ��͂�1���炷
				dropPower--;
			}
			break;
		case 1:
			// �S�[���h�S�~�܂���ł��������Ă����痎�Ƃ�
			if (GetGoldTrashBag() > 0)
			{
				// X�̃I�t�Z�b�g���W�͔͈͓��Ń����_���Ɍ��߂�
				float randomX = Math::Rand(-offsetPos.X(), offsetPos.X());
				// �����Ă���S�[���h�S�~�܂̐���1���炷
				SetGoldTrashBag(-1);
				// �S�~�܂𐶐�
				CTrashBag* trashBag = new CTrashBag(true);
				// �i�I�t�Z�b�g���W�~���Ƃ��́j�����炵���ʒu�ɐ���
				CVector offset = CVector(0.0f, offsetPos.Y(), 0.0f);
				offset += side * randomX;
				offset += front * offsetPos.Z();
				trashBag->Position(pos + offset * dropPower);
				// ���ł������x��ݒ�
				trashBag->SetThrowSpeed(front * GetKnockbackDealt(), GetKnockbackDealt());
				// ���Ƃ��͂�1���炷
				dropPower--;
			}
			break;
		}
	}
}
