#include "CBridge.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"
#include "CPlayer.h"

// �d��
#define WEIGHT 1.0f

// �ڑ��^�[�Q�b�g�̃I�t�Z�b�g���W
#define TARGET_OFFSET_POS CVector(0.0f,50.0f,0.0f)

// ��]���x
#define ROTATE_SPEED 1.0f
#define GRAVITY_ROTATE_SPEED 180.0f

// �X����ő�̊p�x
#define MAX_ANGLE 90.0f
// �X����ŏ��̊p�x
#define MIN_ANGLE 0.0f

// �|��n�߂�p�x
#define FALL_ANGLE 30.0f

// �R���X�g���N�^
CBridge::CBridge(float defaultAngle)
	: CConnectObject(WEIGHT, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mCurrentAngle(defaultAngle)
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
void CBridge::Connect(CVector wandPointPos, CVector targetPointPos)
{
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// �J�����̕���
	CVector cameraDir = -CCamera::CurrentCamera()->VectorZ();
	// �V�������W�����߂�
	CVector newPos = CPlayer::Instance()->Position() + cameraDir * pointMgr->GetConnectDistance();
	
	CVector pullDir = newPos - Position();

	// �X���p�x
	float angle = ROTATE_SPEED * Times::DeltaTime();

	// �����X����
	RotateBridge(angle, pullDir);
}

// �X�V����
void CBridge::Update()
{
#if _DEBUG
	CDebugPrint::Print("Bridge_Angle:%f\n", mCurrentAngle);
#endif
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

	float direction = -VectorZ().Dot(CVector::forward);
	// �����������o��
	direction = std::_Float_copysign(1.0f, direction);

	// �w�ʕ����֌X��(�����~������)
	if (dot > 0.0f)
	{
		// �ύX��̌X����
		// �X����ő�̊p�x��菬�����Ȃ�
		if (mCurrentAngle + angle < MAX_ANGLE)
		{
			// ���݂̊p�x�ɉ��Z
			mCurrentAngle += angle;
			Rotate(VectorX() * angle * direction);
		}
		// �X����ő�̊p�x���傫���Ȃ�
		else if (mCurrentAngle + angle > MAX_ANGLE)
		{
			// �ő�l�ɂȂ�悤�Ɋp�x��ݒ�
			angle = MAX_ANGLE - mCurrentAngle;
			mCurrentAngle = MAX_ANGLE;
			Rotate(VectorX() * angle * direction);
		}
	}
	// ���ʕ����֌X��(�����オ�����)
	else if (dot < 0.0f)
	{
		// �ύX��̌X����
		// �X����ŏ��̊p�x���傫���Ȃ�
		if (mCurrentAngle + angle > MIN_ANGLE)
		{
			// ���݂̊p�x�ɉ��Z
			mCurrentAngle += angle;
			Rotate(VectorX() * angle * direction);
		}
		// �X����ŏ��̊p�x��菬�����Ȃ�
		else if (mCurrentAngle + angle < MIN_ANGLE)
		{
			angle = mCurrentAngle - MIN_ANGLE;
			mCurrentAngle = MIN_ANGLE;
			Rotate(VectorX() * angle * direction);
		}
	}
}

// �w��p�x�ɐݒ肷��
void CBridge::SetAngle(float angle)
{
	// ��]���擾
	CVector rot = EulerAngles();
	// x���̊p�x��ݒ�
	rot.X(angle);
	// ��]��ݒ�
	Rotation(rot);
	// ��]�����p�x��ݒ�
	mCurrentAngle = angle;
}
