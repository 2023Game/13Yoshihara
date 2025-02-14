#include "CDeliveryFieldManager.h"
#include "CDeliveryField.h"
#include "CDeliveryFieldItem.h"
#include "CDeliveryHome.h"
#include "CDeliveryObstruction.h"
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
#define HOME_OFFSET_POSZ  -450.0f
// �Ƃ̕���
#define HOME_ROT_L CVector(0.0f,-90.0f,0.0f)
#define HOME_ROT_R CVector(0.0f, 90.0f,0.0f)
// �Ō�ɉƂ̐���������������W��
// ���̒l�𒴂����玟�̉Ƃ̐������������
#define HOME_JUDGE_POSZ -100.0f
// �Ƃ̐�����
#define HOME_POP_RATE 25.0f

// ��Q���̃I�t�Z�b�g���W
#define OBSTRUCTION_OFFSET_POSZ -450.0f
// �Ō�ɏ�Q���̐���������������W��
// ���̒l�𒴂����玟�̏�Q���̐������������
#define OBSTRUCTION_JUDGE_POSZ -100.0f
// ��Q���̐�����
#define OBSTRUCTION_POP_RATE 50.0f

// �A�C�e���̃I�t�Z�b�g���W
#define ITEM_OFFSET_POSZ -550.0f
// �Ō�ɃA�C�e���̐���������������W��
// ���̒l�𒴂����玟�̃A�C�e���̐������������
#define ITEM_JUDGE_POSZ -200.0f
// �A�C�e���̐�����
#define ITEM_POP_RATE 50.0f

// �C���X�^���X���擾
CDeliveryFieldManager* CDeliveryFieldManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CDeliveryFieldManager::CDeliveryFieldManager()
	: mLastHomePopZ(HOME_OFFSET_POSZ)
	, mLastObstructionPopZ(OBSTRUCTION_OFFSET_POSZ)
	, mLastFieldItemPopZ(ITEM_OFFSET_POSZ)
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
	mLastHomePopZ = mLastHomePopZ + moveSpeed;
	// �Ō�ɐ�������Z���W������𒴂�����
	if (mLastHomePopZ > HOME_JUDGE_POSZ)
	{
		// �Ƃ𐶐����邩���肷��
		CreateHome();
	}
	// �Ō�ɏ�Q���̐������������Z���W����O�ֈړ����Ă���
	mLastObstructionPopZ = mLastObstructionPopZ + moveSpeed;
	// �Ō�ɐ�������Z���W������𒴂�����
	if (mLastObstructionPopZ > OBSTRUCTION_JUDGE_POSZ)
	{
		// ��Q���𐶐����邩���肷��
		CreateObstruction();
	}
	// �Ō�ɏ�Q���̐������������Z���W����O�ֈړ����Ă���
	mLastFieldItemPopZ = mLastFieldItemPopZ + moveSpeed;
	// �Ō�ɐ�������Z���W������𒴂�����
	if (mLastFieldItemPopZ > ITEM_JUDGE_POSZ)
	{
		// �A�C�e���𐶐����邩���肷��
		CreateFieldItem();
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
	// �S�Ă̏�Q���ƃv���C���[�̋������v�Z����
	for (CDeliveryObstruction* obstruction : mObstructions)
	{
		CVector obstructionPos = obstruction->Position();
		// �v���C���[��艜�ɂ���Ȃ玟��
		if (obstructionPos.Z() < playerPos.Z()) continue;

		// �v���C���[�Ƃ̋���
		float dist = obstructionPos.Z() - playerPos.Z();
		// �v���C���[�Ƃ̋������ō��l�ȏ�Ȃ�
		if (dist > PLAYER_DIST)
		{
			// ����
			obstruction->SetEnable(false);
			obstruction->SetShow(false);
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
	mLastHomePopZ = HOME_OFFSET_POSZ;
}

// ��Q���𐶐����邩���肷��
void CDeliveryFieldManager::CreateObstruction()
{
	// �������邩
	bool isPopL1 = false;	// ��1
	bool isPopL2 = false;	// ��2
	bool isPopR1 = false;	// �E1
	bool isPopR2 = false;	// �E2

	// 1����100�܂ł�100�̐������烉���_���Ŏ擾
	float pop = Math::Rand(1, 100);
	// �������ȉ��Ȃ獶1�ɐ���
	if (pop <= OBSTRUCTION_POP_RATE) isPopL1 = true;
	// ������x�����_���擾
	pop = Math::Rand(1, 100);
	// �������ȉ��Ȃ獶1�ɐ���
	if (pop <= OBSTRUCTION_POP_RATE) isPopL2= true;
	// ������x�����_���擾
	pop = Math::Rand(1, 100);
	// �������ȉ��Ȃ�E1�ɐ���
	if (pop <= OBSTRUCTION_POP_RATE) isPopR1 = true;
	// ������x�����_���擾
	pop = Math::Rand(1, 100);
	// �������ȉ��Ȃ�E1�ɐ���
	if (pop <= OBSTRUCTION_POP_RATE) isPopR2 = true;

	// �����ł�����
	bool isPopSuccessL1 = false;	// ��1
	bool isPopSuccessL2 = false;	// ��2
	bool isPopSuccessR1 = false;	// �E1
	bool isPopSuccessR2 = false;	// �E2
	// �������Ȃ��Ȃ琶���ł������Ƃɂ���
	if (!isPopL1) isPopSuccessL1 = true;
	if (!isPopL2) isPopSuccessL2 = true;
	if (!isPopR1) isPopSuccessR1 = true;
	if (!isPopR2) isPopSuccessR2 = true;

	// �ʂ�Ȃ���Ԃ����Ȃ�����
	// �S�Đ�������Ȃ��������Ȃ�
	if (!isPopSuccessL1 &&
		!isPopSuccessL2 &&
		!isPopSuccessR1 &&
		!isPopSuccessR2)
	{
		int random = Math::Rand(1, 4);
		switch (random)
		{
		case 1:
			// ��1�𐶐����Ȃ�
			isPopSuccessL1 = true;
			break;
		case 2:
			// ��2�𐶐����Ȃ�
			isPopSuccessL2 = true;
			break;
		case 3:
			// �E1�𐶐����Ȃ�
			isPopSuccessR1 = true;
			break;
		case 4:
			// �E2�𐶐����Ȃ�
			isPopSuccessR2 = true;
			break;
		}
	}

	for (CDeliveryObstruction* obstruction : mObstructions)
	{
		// �S�Đ����o�����烋�[�v�I��
		if (isPopSuccessL1 &&
			isPopSuccessL2 &&
			isPopSuccessR1 &&
			isPopSuccessR2) break;
		// ���ɗL���Ȃ玟��
		if (obstruction->IsEnable()) continue;

		// ��Q���̍��W�������_���Őݒ�
		ObstructionRandomPos(obstruction,
			isPopSuccessL1,isPopSuccessL2,
			isPopSuccessR1, isPopSuccessR2);
		// �L��
		obstruction->SetEnable(true);
		obstruction->SetShow(true);
	}

	while (!isPopSuccessL1 ||
		!isPopSuccessL2 ||
		!isPopSuccessR1 ||
		!isPopSuccessR2)
	{
		// �V������Q���𐶐�
		CDeliveryObstruction* obstruction = new CDeliveryObstruction();
		// ��Q���̍��W�������_���Őݒ�
		ObstructionRandomPos(obstruction,
			isPopSuccessL1, isPopSuccessL2,
			isPopSuccessR1, isPopSuccessR2);
		// ���X�g�ɒǉ�
		mObstructions.push_back(obstruction);
	}

	// �Ō�ɔ��肵��Z���W���Ƃ̐����ꏊ�ɐݒ�
	mLastObstructionPopZ = OBSTRUCTION_OFFSET_POSZ;
}

// ��Q���̍��W�������_���Őݒ�
void CDeliveryFieldManager::ObstructionRandomPos(
	CDeliveryObstruction* obstruction,
	bool& isPopSuccessL1, bool& isPopSuccessL2,
	bool& isPopSuccessR1, bool& isPopSuccessR2)
{
	// ��1���܂��������Ă��Ȃ��Ȃ�
	if (!isPopSuccessL1)
	{
		// ���̍��W��ݒ�
		obstruction->Position(
			ROAD_LEFT1_POSX,
			0.0f,
			OBSTRUCTION_OFFSET_POSZ
		);
		// ����ݒ�
		obstruction->SetRoadType(ERoadType::eLeft1);
		isPopSuccessL1 = true;
	}
	// ��2���܂��������Ă��Ȃ��Ȃ�
	else if (!isPopSuccessL2)
	{
		// ���̍��W��ݒ�
		obstruction->Position(
			ROAD_LEFT2_POSX,
			0.0f,
			OBSTRUCTION_OFFSET_POSZ
		);
		// ����ݒ�
		obstruction->SetRoadType(ERoadType::eLeft2);
		isPopSuccessL2 = true;
	}
	// �E1���܂��������Ă��Ȃ��Ȃ�
	else if (!isPopSuccessR1)
	{
		// �E�̍��W��ݒ�
		obstruction->Position(
			ROAD_RIGHT1_POSX,
			0.0f,
			OBSTRUCTION_OFFSET_POSZ
		);
		// ����ݒ�
		obstruction->SetRoadType(ERoadType::eRight1);
		isPopSuccessR1 = true;
	}
	// �E2���܂��������Ă��Ȃ��Ȃ�
	else if (!isPopSuccessR2)
	{
		// �E�̍��W��ݒ�
		obstruction->Position(
			ROAD_RIGHT2_POSX,
			0.0f,
			OBSTRUCTION_OFFSET_POSZ
		);
		// ����ݒ�
		obstruction->SetRoadType(ERoadType::eRight2);
		isPopSuccessR2 = true;
	}
}

// �A�C�e���𐶐����邩���肷��
void CDeliveryFieldManager::CreateFieldItem()
{
	// �������邩
	bool isPop = false;

	// 1����100�܂ł�100�̐������烉���_���Ŏ擾
	float pop = Math::Rand(1, 100);
	// �������ȉ��Ȃ琶��
	if (pop <= ITEM_POP_RATE) isPop = true;
	// �������Ȃ��Ȃ珈�����Ȃ�
	if (!isPop) return;

	// �����ł�����
	bool isPopSuccess = false;

	for (CDeliveryFieldItem* fieldItem : mFieldItems)
	{
		// �����o�����烋�[�v�I��
		if (isPopSuccess) break;
		// ���ɗL���Ȃ玟��
		if (fieldItem->IsEnable()) continue;

		// �܂��������Ă��Ȃ��Ȃ�
		if (!isPopSuccess)
		{
			// ���W�������_���Őݒ�
			ItemRandomPos(fieldItem);
		}
		// ��������
		isPopSuccess = true;
		// �L��
		fieldItem->SetEnable(true);
		fieldItem->SetShow(true);
	}

	// �܂��������Ă��Ȃ��Ȃ�
	if (!isPopSuccess)
	{
		// �V������Q���𐶐�
		CDeliveryFieldItem* fieldItem = new CDeliveryFieldItem();
		
		// ���W�������_���Őݒ�
		ItemRandomPos(fieldItem);
		
		// ���X�g�ɒǉ�
		mFieldItems.push_back(fieldItem);
	}

	// �Ō�ɔ��肵��Z���W���Ƃ̐����ꏊ�ɐݒ�
	mLastFieldItemPopZ = ITEM_OFFSET_POSZ;
}

// �A�C�e���̍��W�������_���Őݒ�
void CDeliveryFieldManager::ItemRandomPos(CDeliveryFieldItem* fieldItem)
{
	int random = Math::Rand(1, 4);
	switch (random)
	{
	case 1:
		// ��1�̍��W��ݒ�
		fieldItem->Position(
			ROAD_LEFT1_POSX,
			0.0f,
			ITEM_OFFSET_POSZ
		);
		// ����ݒ�
		fieldItem->SetRoadType(ERoadType::eLeft1);
		break;
	case 2:
		// ��2�̍��W��ݒ�
		fieldItem->Position(
			ROAD_LEFT2_POSX,
			0.0f,
			ITEM_OFFSET_POSZ
		);
		// ����ݒ�
		fieldItem->SetRoadType(ERoadType::eLeft2);
		break;
	case 3:
		// �E1�̍��W��ݒ�
		fieldItem->Position(
			ROAD_RIGHT1_POSX,
			0.0f,
			ITEM_OFFSET_POSZ
		);
		// ����ݒ�
		fieldItem->SetRoadType(ERoadType::eRight1);
		break;
	case 4:
		// �E2�̍��W��ݒ�
		fieldItem->Position(
			ROAD_RIGHT2_POSX,
			0.0f,
			ITEM_OFFSET_POSZ
		);
		// ����ݒ�
		fieldItem->SetRoadType(ERoadType::eRight2);
		break;
	}
}
