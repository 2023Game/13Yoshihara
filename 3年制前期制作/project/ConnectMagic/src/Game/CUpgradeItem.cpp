#include "CUpgradeItem.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"
#include "CConnectPointManager.h"

// コライダーの半径
constexpr float RADIUS =			5.0f;

// 大きさ
constexpr float SIZE =				0.5f;

// 上下移動が切り替わる時間
constexpr float CHANGE_MOVE_TIME =	1.0f;
// 上下移動の速度
constexpr float MOVE_SPEED =		2.0f;

// コンストラクタ
CUpgradeItem::CUpgradeItem()
	: CItemBase(ETag::eItem, ETaskPriority::eBillboard)
	, mIsMoveUp(false)
	, mElapsedTime(0.0f)
{
	mpItemImg = new CBillBoardImage(
		"Field/UpgradeItem/UpgradeItem.png",
		ETag::eItem, ETaskPauseType::eGame
	);
	// 親に設定
	mpItemImg->SetParent(this);
	// サイズを取得
	CVector2 size = mpItemImg->GetSize();
	size *= SIZE;
	// サイズを設定
	mpItemImg->SetSize(size);

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CUpgradeItem::~CUpgradeItem()
{
	SAFE_DELETE(mpCol);
}

// 衝突処理
void CUpgradeItem::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCol)
	{
		// プレイヤーなら
		if (other->Layer() == ELayer::ePlayer)
		{
			// 取得時の処理
			GetItem();
			// 自身を削除
			Delete();
			return;
		}
	}
}

// 取得時の処理
void CUpgradeItem::GetItem()
{
}

// 更新
void CUpgradeItem::Update()
{
	// 時間経過
	mElapsedTime += Times::DeltaTime();
	// 切り替わる時間を過ぎたら
	if (mElapsedTime > CHANGE_MOVE_TIME)
	{
		mElapsedTime = 0.0f;
		// 上下移動のフラグを反転
		mIsMoveUp = !mIsMoveUp;
	}

	// 移動速度
	float moveSpeed = 0.0f;

	// 上移動なら
	if (mIsMoveUp)
	{
		// プラス方向移動
		moveSpeed += MOVE_SPEED * Times::DeltaTime();
	}
	// 下移動なら
	else
	{
		// マイナス方向移動
		moveSpeed -= MOVE_SPEED * Times::DeltaTime();
	}

	// 移動する
	Position(Position() + CVector(0.0f, moveSpeed, 0.0f));
}

// 全て削除
void CUpgradeItem::Delete()
{
	// アイテム画像を削除
	mpItemImg->Kill();
	mpItemImg = nullptr;
	// 削除
	Kill();
}

// コライダーを生成
void CUpgradeItem::CreateCol()
{
	mpCol = new CColliderSphere(
		this, ELayer::eItem,
		RADIUS, true
	);
	// プレイヤーとだけ衝突判定
	mpCol->SetCollisionLayers({ ELayer::ePlayer });
}
