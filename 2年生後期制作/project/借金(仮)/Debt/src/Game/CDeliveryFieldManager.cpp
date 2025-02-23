#include "CDeliveryFieldManager.h"
#include "CDeliveryField.h"
#include "CDeliveryFieldItem.h"
#include "CDeliveryHome.h"
#include "CDeliveryObstruction.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemyManager.h"
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
#define HOME_POP_RATE 50.0f

// ��Q���̃I�t�Z�b�g���W
#define OBSTRUCTION_OFFSET_POSZ -450.0f
// �Ō�ɏ�Q���̐���������������W��
// ���̒l�𒴂����玟�̏�Q���̐������������
#define OBSTRUCTION_JUDGE_POSZ -200.0f
// ��Q���̐�����
#define OBSTRUCTION_POP_RATE 100.0f

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
	, mLastObstructionRoad(ERoadType::None)
	, mLastItemRoad(ERoadType::None)
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
	// �Ō�ɏ�Q���̐������������Z���W����O�ֈړ����Ă���
	mLastObstructionPopZ = mLastObstructionPopZ + moveSpeed;
	// �Ō�Ƀt�B�[���h�A�C�e���̐������������Z���W����O�ֈړ����Ă���
	mLastFieldItemPopZ = mLastFieldItemPopZ + moveSpeed;
	// �Ō�ɐ�������Z���W������𒴂�����
	if (mLastHomePopZ > HOME_JUDGE_POSZ)
	{
		// �Ƃ𐶐����邩���肷��
		CreateHome();
	}

	// �Ō�ɐ�������Z���W������𒴂�����
	if (mLastObstructionPopZ > OBSTRUCTION_JUDGE_POSZ)
	{
		// ��Q���𐶐����邩���肷��
		CreateObstruction();
	}
	// �Ō�ɐ�������Z���W������𒴂�����
	else if (mLastFieldItemPopZ > ITEM_JUDGE_POSZ)
	{
		// �t�B�[���h�A�C�e���𐶐����邩���肷��
		CreateFieldItem();
		// �G���L���Ȃ�
		if (CDeliveryEnemyManager::Instance()->GetEnamyEnable())
		{
			// ���������
			CreateFieldItem();
		}
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

	// �S�Ẵt�B�[���h�A�C�e���ƃv���C���[�̋������v�Z����
	for (CDeliveryFieldItem* fieldItem : mFieldItems)
	{
		CVector fieldItemPos = fieldItem->Position();
		// �v���C���[��艜�ɂ���Ȃ玟��
		if (fieldItemPos.Z() < playerPos.Z()) continue;

		// �v���C���[�Ƃ̋���
		float dist = fieldItemPos.Z() - playerPos.Z();
		// �v���C���[�Ƃ̋������ō��l�ȏ�Ȃ�
		if (dist > PLAYER_DIST)
		{
			// ����
			fieldItem->SetEnable(false);
			fieldItem->SetShow(false);
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
	bool isPop = false;

	// 1����100�܂ł�100�̐������烉���_���Ŏ擾
	float pop = Math::Rand(1, 100);
	// �������ȉ��Ȃ琶��
	if (pop <= OBSTRUCTION_POP_RATE) isPop = true;

	// �����ł�����
	bool isPopSuccess = false;

	for (CDeliveryObstruction* obstruction : mObstructions)
	{
		// �����o�����烋�[�v�I��
		if (isPopSuccess) break;
		// ���ɗL���Ȃ玟��
		if (obstruction->IsEnable()) continue;

		// ��Q���̍��W�������_���Őݒ�
		ObstructionRandomPos(obstruction);
		// ��������
		isPopSuccess = true;
		// �L��
		obstruction->SetEnable(true);
		obstruction->SetShow(true);
	}

	// �܂��������Ă��Ȃ��Ȃ�
	while (!isPopSuccess &&
		isPop)
	{
		// �V������Q���𐶐�
		CDeliveryObstruction* obstruction = new CDeliveryObstruction();
		// ��Q���̍��W�������_���Őݒ�
		ObstructionRandomPos(obstruction);
		// ���X�g�ɒǉ�
		mObstructions.push_back(obstruction);
		// ��������
		isPopSuccess = true;
	}

	// �Ō�ɔ��肵��Z���W����Q���̐����ꏊ�ɐݒ�
	mLastObstructionPopZ = OBSTRUCTION_OFFSET_POSZ;
}

// ��Q���̍��W�������_���Őݒ�
void CDeliveryFieldManager::ObstructionRandomPos(CDeliveryObstruction* obstruction)
{
	// �S�Ă̓�
	std::vector<ERoadType> allRoads =
	{ ERoadType::eLeft1,ERoadType::eLeft2,ERoadType::eRight1,ERoadType::eRight2 };
	// �����ł��铹
	std::vector<ERoadType> canPopRoads;

	// �S�Ă̓����J��Ԃ�
	for (ERoadType road : allRoads)
	{
		// �Ō�ɐ����������łȂ����
		if (road != mLastObstructionRoad)
		{
			// �����ł��铹�ɒǉ�
			canPopRoads.push_back(road);
		}
	}

	// 1���瓹�̐��̗���
	int random = Math::Rand(1, canPopRoads.size());
	// �������铹������
	ERoadType popRoad = canPopRoads[random - 1];
	// �������铹��X���W
	float popPosX = 0.0f;

	// �������铹��X���W������
	switch (popRoad)
	{
	case ERoadType::eLeft1:

		popPosX = ROAD_LEFT1_POSX;

		break;
	case ERoadType::eLeft2:

		popPosX = ROAD_LEFT2_POSX;

		break;
	case ERoadType::eRight1:

		popPosX = ROAD_RIGHT1_POSX;

		break;
	case ERoadType::eRight2:

		popPosX = ROAD_RIGHT2_POSX;

		break;
	}		
	// ���W��ݒ�
	obstruction->Position(
		popPosX,
		0.0f,
		OBSTRUCTION_OFFSET_POSZ
	);
	// ����ݒ�
	obstruction->SetRoadType(popRoad);
	// �Ō�ɏ�Q���̐�������������ݒ�
	mLastObstructionRoad = popRoad;
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

	// �����ł�����
	bool isPopSuccess = false;

	for (CDeliveryFieldItem* fieldItem : mFieldItems)
	{
		// �����o�����烋�[�v�I��
		if (isPopSuccess) break;
		// ���ɗL���Ȃ玟��
		if (fieldItem->IsEnable()) continue;

		// �A�C�e���̍��W�������_���Őݒ�
		ItemRandomPos(fieldItem);
		// ��������
		isPopSuccess = true;
		// �L��
		fieldItem->SetEnable(true);
		fieldItem->SetShow(true);
	}

	// �܂��������Ă��Ȃ��Ȃ�
	while (!isPopSuccess&&
		isPop)
	{
		// �V�����A�C�e���𐶐�
		CDeliveryFieldItem* fieldItem = new CDeliveryFieldItem();

		// ���W�������_���Őݒ�
		ItemRandomPos(fieldItem);

		// ���X�g�ɒǉ�
		mFieldItems.push_back(fieldItem);
		// ��������
		isPopSuccess = true;
	}

	// �Ō�ɔ��肵��Z���W���A�C�e���̐����ꏊ�ɐݒ�
	mLastFieldItemPopZ = ITEM_OFFSET_POSZ;
}

// �A�C�e���̍��W�������_���Őݒ�
void CDeliveryFieldManager::ItemRandomPos(CDeliveryFieldItem* fieldItem)
{
	// �S�Ă̓�
	std::vector<ERoadType> allRoads =
	{ ERoadType::eLeft1,ERoadType::eLeft2,ERoadType::eRight1,ERoadType::eRight2 };
	// �����ł��铹
	std::vector<ERoadType> canPopRoads;

	// �S�Ă̓����J��Ԃ�
	for (ERoadType road : allRoads)
	{
		// �Ō�ɐ����������łȂ����
		if (road != mLastItemRoad)
		{
			// �����ł��铹�ɒǉ�
			canPopRoads.push_back(road);
		}
	}

	// 1���瓹�̐��̗���
	int random = Math::Rand(1, canPopRoads.size());
	// �������铹������
	ERoadType popRoad = canPopRoads[random - 1];
	// �������铹��X���W
	float popPosX = 0.0f;

	// �������铹��X���W������
	switch (popRoad)
	{
	case ERoadType::eLeft1:

		popPosX = ROAD_LEFT1_POSX;

		break;
	case ERoadType::eLeft2:

		popPosX = ROAD_LEFT2_POSX;

		break;
	case ERoadType::eRight1:

		popPosX = ROAD_RIGHT1_POSX;

		break;
	case ERoadType::eRight2:

		popPosX = ROAD_RIGHT2_POSX;

		break;
	}
	// ���W��ݒ�
	fieldItem->Position(
		popPosX,
		0.0f,
		OBSTRUCTION_OFFSET_POSZ
	);
	// ����ݒ�
	fieldItem->SetRoadType(popRoad);
	// �Ō�ɏ�Q���̐�������������ݒ�
	mLastItemRoad = popRoad;
}
