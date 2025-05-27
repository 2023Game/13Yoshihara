#include "CBridge.h"
#include "CColliderMesh.h"

// �d��
#define WEIGHT 0.9f

// �ڑ��^�[�Q�b�g�̃I�t�Z�b�g���W
#define TARGET_OFFSET_POS CVector(0.0f,50.0f,0.0f)

// ��]���x
#define ROTATE_SPEED 5.0f
#define GRAVITY_ROTATE_SPEED 180.0f

// �X����ő�̊p�x
#define MAX_ANGLE 90.0f
// �X����ŏ��̊p�x
#define MIN_ANGLE 0.0f

// �|��n�߂�p�x
#define FALL_ANGLE 30.0f

// �R���X�g���N�^
CBridge::CBridge()
	: CConnectObject(WEIGHT, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mCurrentAngle(0.0f)
{
	// �d�͖���
	mIsGravity = false;

	mpModel = CResourceManager::Get<CModel>("Bridge");

	// �R���C�_�[�𐶐�
	CreateCol();

	// �ڑ��^�[�Q�b�g�̐���
	CreateTarget(TARGET_OFFSET_POS);
}

// �f�X�g���N�^
CBridge::~CBridge()
{
}

// �������鏈��
void CBridge::Pull(CVector pullDir, float opponentWeight)
{
	// �X���p�x
	float angle = ROTATE_SPEED * Times::DeltaTime();

	// �����X����
	RotateBridge(angle, pullDir);
}

// �X�V����
void CBridge::Update()
{
	CDebugPrint::Print("Bridge_Angle:%f\n", mCurrentAngle);
	// �X���p�x
	float angle = GRAVITY_ROTATE_SPEED * Times::DeltaTime();
	// �����d�͂ŌX����
	RotateBridge(angle, CVector::down);
}

// �R���C�_�[�𐶐�
void CBridge::CreateCol()
{
	mpCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Bridge_Col"),
		true
	);
	// �v���C���[�ƓG�ƃI�u�W�F�N�g
	// �Ɛڑ��I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔��肷��
	mpCol->SetCollisionLayers({ 
		ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eObject,ELayer::eConnectSearch });
}

// �p�x�������X����
void CBridge::RotateBridge(float angle, CVector dir)
{
	// �w�ʕ����Ƃ̊֌W
	float dot = -VectorZ().Dot(dir);
	// �͂̂������ŌX���𒲐�
	angle *= dot;

	// �w�ʕ����֌X��(�����~������)
	if (dot > 0.0f)
	{
		// �ύX��̌X����
		// �X����ő�̊p�x��菬�����Ȃ�
		if (mCurrentAngle + angle < MAX_ANGLE)
		{
			// ���݂̊p�x�ɉ��Z
			mCurrentAngle += angle;
			Rotate(-VectorX() * angle);
		}
		// �X����ő�̊p�x���傫���Ȃ�
		else if (mCurrentAngle + angle > MAX_ANGLE)
		{
			// �ő�l�ɂȂ�悤�Ɋp�x��ݒ�
			angle = MAX_ANGLE - mCurrentAngle;
			mCurrentAngle = MAX_ANGLE;
			Rotate(-VectorX() * angle);
		}
	}
	// ���ʕ����֌X��(�����オ�����)
	else if (dot < 0.0f)
	{
		// �ύX��̌X����
		// �X����ŏ��̊p�x���傫���Ȃ�
		if (mCurrentAngle - angle > MIN_ANGLE)
		{
			// ���݂̊p�x�Ɍ��Z
			mCurrentAngle -= angle;
			Rotate(VectorX() * angle);
		}
		// �X����ŏ��̊p�x��菬�����Ȃ�
		else if (mCurrentAngle - angle < MIN_ANGLE)
		{
			angle = mCurrentAngle - MIN_ANGLE;
			mCurrentAngle = MIN_ANGLE;
			Rotate(VectorX() * angle);
		}
	}
}
