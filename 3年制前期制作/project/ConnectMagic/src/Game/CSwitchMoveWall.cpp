#include "CSwitchMoveWall.h"
#include "CColliderMesh.h"
#include "Maths.h"
#include "CConnectPointManager.h"

// �R���X�g���N�^
CSwitchMoveWall::CSwitchMoveWall(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime, bool isCrushed)
	: mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mpCrushedCol(nullptr)
{
	mpModel = model;

	// �R���C�_�[�𐶐�
	CreateCol();
	// �v���C���[���󂷂Ȃ�
	if (isCrushed)
	{
		// �v���C���[�����܂ꂽ���p�̃R���C�_�[
		mpCrushedCol = new CColliderMesh(this, ELayer::eCrushed, col, true);
		// �v���C���[�Ƃ����Փ�
		mpCrushedCol->SetCollisionLayers({ ELayer::ePlayer });
	}

	// �������W��ݒ�
	Position(mDefaultPos);
	Scale(scale);
}

// �f�X�g���N�^
CSwitchMoveWall::~CSwitchMoveWall()
{
	SAFE_DELETE(mpCrushedCol);
}

// �R���C�_�[�𐶐�
void CSwitchMoveWall::CreateCol()
{
	mpCol = new CColliderMesh(this, ELayer::eWall, mpModel, true);

	// �ڑ����̊Ǘ��N���X
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// �R���C�_�[��ǉ�
	pointMgr->AddCollider(mpCol);
	// ���݂̃J����
	CCamera* camera = CCamera::CurrentCamera();
	// �R���C�_�[��ǉ�
	camera->AddCollider(mpCol);
}

// ��p���Ă��Ȃ����̏���
void CSwitchMoveWall::UpdateOff()
{
	// �o�ߎ��Ԃ�0�b�Ȃ珈�����Ȃ�
	if (mElapsedTime == 0.0f) return;

	// �ړ�����
	Move();

	// �o�ߎ��Ԃ�����
	mElapsedTime -= Times::DeltaTime();
	// �o�ߎ��Ԃ�0�ȉ��Ȃ�
	if (mElapsedTime <= 0.0f)
	{
		// �o�ߎ��Ԃ�0
		mElapsedTime = 0.0f;
		// ���W���X�V
		Move();
	}
}

// ��p���Ă��鎞�̏���
void CSwitchMoveWall::UpdateOn()
{
	// �ړ����Ԃ��o�߂��Ă����珈�����Ȃ�
	if (mElapsedTime == mMoveTime) return;

	// �ړ�����
	Move();

	// ���Ԍo��
	mElapsedTime += Times::DeltaTime();
	// �ړ����Ԃ��o�߂��Ă�����
	if (mElapsedTime >= mMoveTime)
	{
		// �o�ߎ��Ԃ̒l���ړ����Ԃɂ���
		mElapsedTime = mMoveTime;
		// ���W���X�V
		Move();
	}
}

// �ړ��̏���
void CSwitchMoveWall::Move()
{
	// ���Ԃ��i�񂾊���
	float per = mElapsedTime / mMoveTime;
	// ���W�����߂�
	Position(mDefaultPos + mMoveVec * per);
}
