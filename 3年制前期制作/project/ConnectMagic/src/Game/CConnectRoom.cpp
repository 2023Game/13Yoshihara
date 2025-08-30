#include "CConnectRoom.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"
#include "CColliderSphere.h"
#include "CShield.h"
#include "CRoomManager.h"

// ���̃I�t�Z�b�g���W
#define BOX_OFFSET_POS CVector(25.0f,0.0f,-40.0f)
// �X�C�b�`�̃I�t�Z�b�g���W
#define SWITCH_OFFSET_POS_NEXT	CVector(-25.0f,0.0f,-40.0f)
// �����̒���
#define ROOM_LENGTH 80.0f

// �����ǂ̃I�t�Z�b�g���W
#define MOVE_WALL_OFFSET_POS_NEXT	CVector(0.0f,0.0f,-70.0f)
#define MOVE_WALL_OFFSET_POS_PRE	CVector(0.0f,0.0f,0.0f)
// �����ǂ̃X�P�[��
#define MOVE_WALL_SCALE CVector(2.0f,4.0f,1.0f)
// �����ǂ̈ړ�
#define MOVE_WALL_MOVE	CVector(0.0f,40.0f,0.0f)
// �����ǂ̈ړ�����
#define MOVE_WALL_MOVE_TIME 0.5f

// ����t���O�p�̃R���C�_�[�̔��a
#define COL_RADIUS 15.0f
// �R���C�_�[�̈ʒu
#define COL_POS CVector(0.0f,0.0f,-45.0f)

// �V�[���h�̃I�t�Z�b�g���W
#define SHIELD_OFFSET_POS CVector(0.0f,0.0f,-75.0f)

// �R���X�g���N�^
CConnectRoom::CConnectRoom(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
	, mpNextRoom(nullptr)
	, mpPreRoom(nullptr)
{
	mpModel = CResourceManager::Get<CModel>("ConnectRoom");
	// ���W��ݒ�
	Position(pos);

	// �R���C�_�[����
	CreateCol();

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CConnectRoom::~CConnectRoom()
{
	SAFE_DELETE(mpCloseCol);
}

// ���̕�����ݒ�
void CConnectRoom::SetNextRoom(CRoomBase* room)
{
	mpNextRoom = room;
}

// �O�̕�����ݒ�
void CConnectRoom::SetPreRoom(CRoomBase* room)
{
	mpPreRoom = room;
}

// �Փˏ���
void CConnectRoom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CFieldBase::Collision(self, other, hit);

	// ����t���O�p�̃R���C�_�[
	if (self == mpCloseCol)
	{
		// �v���C���[�Ȃ�
		if (other->Layer() == ELayer::ePlayer)
		{
			// �O�̕����Ɍq����ǂ����
			mpPreWall->SetOnOff(false);
		}
	}
}

// �����̗L��������ݒ�
void CConnectRoom::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);
	mpNextSwitch->SetEnableSwitch(enable);
}

// �X�V
void CConnectRoom::Update()
{
	// ������������
	// �ǂ��J���Ă���Ȃ�
	if (!mpNextRoom->IsEnable() &&
		mpNextWall->IsOpen())
	{
		// �����Ǘ��N���X
		CRoomManager* roomMgr = CRoomManager::Instance();
		// ������L��
		roomMgr->RoomOn();
	}

	// �O���L������
	// �ǂ��J���Ă��Ȃ��Ȃ�
	if (mpPreRoom->IsEnable() &&
		!mpPreWall->IsOpen())
	{
		// �����Ǘ��N���X
		CRoomManager* roomMgr = CRoomManager::Instance();
		// �����𖳌�
		roomMgr->RoomOff();
	}
}

// �R���C�_�[�̐���
void CConnectRoom::CreateCol()
{
	// �R���C�_�[�𐶐�
	CFieldBase::CreateCol("ConnectRoom_Ground_Col", "ConnectRoom_Wall_Col", "");

	// ����t���O�p�̃R���C�_�[
	mpCloseCol = new CColliderSphere(
		this, ELayer::eSwitch,
		COL_RADIUS, true
	);
	mpCloseCol->Position(COL_POS);
	mpCloseCol->SetCollisionLayers({ ELayer::ePlayer });
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CConnectRoom::CreateFieldObjects()
{
	// �X�C�b�`�𐶐�
	mpNextSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS_NEXT, true);
	// �����ǂ𐶐�
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CModel* colModel = CResourceManager::Get<CModel>("MoveObject_Col");
	mpNextWall = new CSwitchMoveWall(model, colModel,
		Position() + MOVE_WALL_OFFSET_POS_NEXT, 
		MOVE_WALL_SCALE, 
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME, true);
	mpPreWall = new CSwitchMoveWall(model, colModel,
		Position() + MOVE_WALL_OFFSET_POS_PRE,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME, true);
	// �ŏ�����J���Ă���
	mpPreWall->SetIsOpen(true);
	// ��p����X�C�b�`�ɐݒ�
	mpNextWall->SetSwitchs({ mpNextSwitch });
	// ��p����I�u�W�F�N�g�ɐݒ�
	mpNextSwitch->SetActionObj(mpNextWall);
	// ���X�g�ɒǉ�
	mObjs.push_back(mpNextWall);
	mObjs.push_back(mpPreWall);

	// �V�[���h�𐶐�
	mpShield = new CShield();
	mpShield->Position(Position() + SHIELD_OFFSET_POS);
	// ���X�g�ɒǉ�
	mObjs.push_back(mpShield);
}