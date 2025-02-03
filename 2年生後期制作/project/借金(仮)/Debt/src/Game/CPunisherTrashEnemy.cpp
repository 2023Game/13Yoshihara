#include "CPunisherTrashEnemy.h"
#include "CGaugeUI3D.h"
#include "CDebugFieldOfView.h"

// �傫��
#define SCALE 0.1f

#define BODY_RADIUS 2.5f	// �{�̂̃R���C�_�\�̔��a
#define BODY_HEIGHT 25.0f	// �{�̂̃R���C�_�\�̍���
#define BODY_WIDTH 50.0f	// �{�̂̃R���C�_�\�̕�

// �R���X�g���N�^
CPunisherTrashEnemy::CPunisherTrashEnemy()
	:CTrashEnemy(true, BODY_RADIUS, BODY_HEIGHT, BODY_WIDTH)
{
	// �ŏ��͖���
	mpHpGauge->SetEnable(false);
	mpHpGauge->SetShow(false);
#if _DEBUG
	mpDebugFov->SetEnable(false);
	mpDebugFov->SetShow(false);
#endif
}

// �f�X�g���N�^
CPunisherTrashEnemy::~CPunisherTrashEnemy()
{
}
