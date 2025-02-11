#include "CDeliveryFieldManager.h"
#include "CDeliveryField.h"
#include "CDeliveryHome.h"
#include "CDeliveryPlayer.h"
#include "CSky.h"
#include "Maths.h"

// �z�B�̃t�B�[���h�Ǘ��N���X�̃C���X�^���X
CDeliveryFieldManager* CDeliveryFieldManager::spInstance = nullptr;

// ��������t�B�[���h�̐�
#define FIELD_NUM 5

// �t�B�[���h���m�̋���
#define FIELD_DIST 400.0f * SCALE_RATIO

// �v���C���[�Ƃ̋����̍ō��l�A
// �����艓���t�B�[���h�̓v���C���[�̑O����
// �Ƃ͖����ɂ���
#define PLAYER_DIST (FIELD_DIST * 1.5f)

// �Ƃ̃I�t�Z�b�g���W
#define HOME_OFFSET_POSX   250.0f
#define HOME_OFFSET_POSY    10.0f
#define HOME_OFFSET_POSZ -450.0f
// �Ƃ̕���
#define HOME_ROT_L CVector(0.0f,-90.0f,0.0f)
#define HOME_ROT_R CVector(0.0f, 90.0f,0.0f)

// �Ō�ɉƂ̐���������������W��
// ���̒l�𒴂����玟�̉Ƃ̐������������
#define HOME_JUDGE_POSZ -100.0f

// �Ƃ̐�����
#define HOME_POP_RATE 25.0f

// �C���X�^���X���擾
CDeliveryFieldManager* CDeliveryFieldManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CDeliveryFieldManager::CDeliveryFieldManager()
	: mLastPopZ(HOME_OFFSET_POSZ)
{
	spInstance = this;

	// �t�B�[���h�𐶐�
	CreateField();
}

// �f�X�g���N�^
CDeliveryFieldManager::~CDeliveryFieldManager()
{
	SAFE_DELETE(spInstance);
}

// �X�V
void CDeliveryFieldManager::Update()
{
	// �Ō�ɉƂ̐������������Z���W����O�ֈړ����Ă���
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	mLastPopZ = mLastPopZ + moveSpeed;
	// �Ō�ɐ�������Z���W������𒴂�����
	if (mLastPopZ > HOME_JUDGE_POSZ)
	{
		// �Ƃ𐶐����邩���肷��
		CreateHome();
	}

	// �v���C���[�̍��W���擾
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();

	// �S�Ẵt�B�[���h�ƃv���C���[�̋������v�Z����
	for (CDeliveryField* field : mFields)
	{
		CVector fieldPos = field->Position();
		// �v���C���[��艜�ɂ���Ȃ玟��
		if (fieldPos.Z() < playerPos.Z()) continue;

		// �v���C���[�Ƃ̋���
		float dist = fieldPos.Z() - playerPos.Z();
		// �v���C���[�Ƃ̋������ō��l�ȏ�Ȃ�
		if (dist > PLAYER_DIST)
		{
			// ��ԉ����̃t�B�[���h��Z���W����
			// �t�B�[���h���m�̋���������ɉ�����Z���W
			float newPosZ = mpFarField->Position().Z() - FIELD_DIST;
			// ���W��ݒ�
			field->Position(0.0f, 0.0f, newPosZ);
			// ��ԉ����t�B�[���h��ۑ�
			mpFarField = field;
		}
	}
	// �S�ẲƂƃv���C���[�̋������v�Z����
	for (CDeliveryHome* home: mHomes)
	{
		CVector homePos = home->Position();
		// �v���C���[��艜�ɂ���Ȃ玟��
		if (homePos.Z() < playerPos.Z()) continue;

		// �v���C���[�Ƃ̋���
		float dist = homePos.Z() - playerPos.Z();
		// �v���C���[�Ƃ̋������ō��l�ȏ�Ȃ�
		if (dist > PLAYER_DIST)
		{
			// ����
			home->SetEnable(false);
			home->SetShow(false);
		}
	}
}

// �w��̃t�B�[���h���擾����
CDeliveryField* CDeliveryFieldManager::GetField(int num, bool& end)
{
	if (num == mFields.size() - 1) end = true;

	return mFields[num];
}

// �t�B�[���h�𐶐�
void CDeliveryFieldManager::CreateField()
{
	for (int i = 0; i < FIELD_NUM; i++)
	{
		CDeliveryField* field = new CDeliveryField();
		field->Position(0.0f, 0.0f, -FIELD_DIST * (i - 1));
		mFields.push_back(field);
		// ��ԍŌ�̃t�B�[���h��Z���W��ۑ�
		if (i == FIELD_NUM - 1) mpFarField = field;
	}
}

// �Ƃ𐶐�
void CDeliveryFieldManager::CreateHome()
{
	// �������邩
	bool isPopL = false;	// ��
	bool isPopR = false;	// �E

	// 1����100�܂ł�100�̐������烉���_���Ŏ擾
	float pop = Math::Rand(1, 100);
	// �������ȉ��Ȃ獶�ɐ���
	if (pop <= HOME_POP_RATE) isPopL = true;
	// ������x�����_���擾
	pop = Math::Rand(1, 100);
	// �������ȉ��Ȃ�E�ɐ���
	if (pop <= HOME_POP_RATE) isPopR = true;

	// �����ł�����
	bool isPopSuccessL = false;	// ��
	bool isPopSuccessR = false;	// �E
	// �������Ȃ��Ȃ琶���ł������Ƃɂ���
	if (!isPopL) isPopSuccessL = true;
	if (!isPopR) isPopSuccessR = true;

	for (CDeliveryHome* home : mHomes)
	{
		// ���������o�����烋�[�v�I��
		if (isPopSuccessL && isPopSuccessR) break;
		// ���ɗL���Ȃ玟��
		if (home->IsEnable()) continue;

		// �����܂��������Ă��Ȃ��Ȃ�
		if (!isPopSuccessL)
		{
			// ���̍��W��ݒ�
			home->Position(
				-HOME_OFFSET_POSX,
				 HOME_OFFSET_POSY,
				 HOME_OFFSET_POSZ
			);
			// ������ݒ�
			home->Rotation(HOME_ROT_L);
			isPopSuccessL = true;
		}
		// �E���܂��������Ă��Ȃ��Ȃ�
		else if (!isPopSuccessR)
		{
			// �E�̍��W��ݒ�
			home->Position(
				HOME_OFFSET_POSX,
				HOME_OFFSET_POSY,
				HOME_OFFSET_POSZ
			);
			// ������ݒ�
			home->Rotation(HOME_ROT_R);
			isPopSuccessR = true;
		}
		// �L��
		home->SetEnable(true);
		home->SetShow(true);
	}

	// �����܂��������Ă��Ȃ��Ȃ�
	if (!isPopSuccessL)
	{
		// �V�����Ƃ𐶐�
		CDeliveryHome* home = new CDeliveryHome();
		// ���̍��W��ݒ�
		home->Position(
			-HOME_OFFSET_POSX,
			 HOME_OFFSET_POSY,
			 HOME_OFFSET_POSZ
		);
		// ������ݒ�
		home->Rotation(HOME_ROT_L);
		// ���X�g�ɒǉ�
		mHomes.push_back(home);
	}
	// �E���܂��������Ă��Ȃ��Ȃ�
	if (!isPopSuccessR)
	{
		// �V�����Ƃ𐶐�
		CDeliveryHome* home = new CDeliveryHome();
		// �E�̍��W��ݒ�
		home->Position(
			HOME_OFFSET_POSX,
			HOME_OFFSET_POSY,
			HOME_OFFSET_POSZ
		);
		// ������ݒ�
		home->Rotation(HOME_ROT_R);
		// ���X�g�ɒǉ�
		mHomes.push_back(home);
	}

	// �Ō�ɔ��肵��Z���W���Ƃ̐����ꏊ�ɐݒ�
	mLastPopZ = HOME_OFFSET_POSZ;
}
