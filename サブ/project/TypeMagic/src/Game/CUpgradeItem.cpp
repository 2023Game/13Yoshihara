#include "CUpgradeItem.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"
#include "CConnectPointManager.h"

// �R���C�_�[�̔��a
#define RADIUS 5.0f

// �傫��
#define SIZE 0.5f

// �㉺�ړ����؂�ւ�鎞��
#define CHANGE_MOVE_TIME 1.0f
// �㉺�ړ��̑��x
#define MOVE_SPEED 2.0f

// �R���X�g���N�^
CUpgradeItem::CUpgradeItem()
	: CItemBase(ETag::eItem, ETaskPriority::eBillboard)
	, mIsMoveUp(false)
	, mElapsedTime(0.0f)
{
	mpItemImg = new CBillBoardImage(
		"Field/UpgradeItem/UpgradeItem.png",
		ETag::eItem, ETaskPauseType::eGame
	);
	// �e�ɐݒ�
	mpItemImg->SetParent(this);
	// �T�C�Y���擾
	CVector2 size = mpItemImg->GetSize();
	size *= SIZE;
	// �T�C�Y��ݒ�
	mpItemImg->SetSize(size);

	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CUpgradeItem::~CUpgradeItem()
{
	SAFE_DELETE(mpCol);
}

// �Փˏ���
void CUpgradeItem::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCol)
	{
		// �v���C���[�Ȃ�
		if (other->Layer() == ELayer::ePlayer)
		{
			// �擾���̏���
			GetItem();
			// ���g���폜
			Delete();
			return;
		}
	}
}

// �擾���̏���
void CUpgradeItem::GetItem()
{
	// �ڑ����Ǘ��N���X���擾
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// �����A�C�e���̊l�����𑝉�
	pointMgr->AddUpgradeItemNum();
}

// �X�V
void CUpgradeItem::Update()
{
	// ���Ԍo��
	mElapsedTime += Times::DeltaTime();
	// �؂�ւ�鎞�Ԃ��߂�����
	if (mElapsedTime > CHANGE_MOVE_TIME)
	{
		mElapsedTime = 0.0f;
		// �㉺�ړ��̃t���O�𔽓]
		mIsMoveUp = !mIsMoveUp;
	}

	// �ړ����x
	float moveSpeed = 0.0f;

	// ��ړ��Ȃ�
	if (mIsMoveUp)
	{
		// �v���X�����ړ�
		moveSpeed += MOVE_SPEED * Times::DeltaTime();
	}
	// ���ړ��Ȃ�
	else
	{
		// �}�C�i�X�����ړ�
		moveSpeed -= MOVE_SPEED * Times::DeltaTime();
	}

	// �ړ�����
	Position(Position() + CVector(0.0f, moveSpeed, 0.0f));
}

// �S�č폜
void CUpgradeItem::Delete()
{
	// �A�C�e���摜���폜
	mpItemImg->Kill();
	mpItemImg = nullptr;
	// �폜
	Kill();
}

// �R���C�_�[�𐶐�
void CUpgradeItem::CreateCol()
{
	mpCol = new CColliderSphere(
		this, ELayer::eItem,
		RADIUS, true
	);
	// �v���C���[�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::ePlayer });
}
