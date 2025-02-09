#include "CDeliveryFieldManager.h"
#include "CDeliveryField.h"
#include "CDeliveryPlayer.h"
#include "CSky.h"

// �z�B�̃t�B�[���h�Ǘ��N���X�̃C���X�^���X
CDeliveryFieldManager* CDeliveryFieldManager::spInstance = nullptr;

// ��������t�B�[���h�̐�
#define FIELD_NUM 5

// �t�B�[���h���m�̋���
#define FIELD_DIST 400.0f * SCALE_RATIO

// �v���C���[�Ƃ̋����̍ō��l�A�����艓���t�B�[���h�̓v���C���[�̑O����
#define PLAYER_DIST (FIELD_DIST * 1.5f)

// �C���X�^���X���擾
CDeliveryFieldManager* CDeliveryFieldManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CDeliveryFieldManager::CDeliveryFieldManager()
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
			// �Ƃ̈ʒu�������_���Ɍ��߂�
			field->RandomHome();
			// ��ԉ����t�B�[���h��ۑ�
			mpFarField = field;
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
