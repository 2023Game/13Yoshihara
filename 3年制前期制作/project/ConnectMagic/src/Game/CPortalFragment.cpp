#include "CPortalFragment.h"
#include "CImage3D.h"
#include "CColliderCapsule.h"

// 画像の座標
#define IMG_OFFSET_POS CVector(0.0f,10.0f,0.0f)

// コライダーの高さ
#define HEIGHT 10.0f
// コライダーの半径
#define RADIUS 5.0f

// 合成の移動速度
#define MERGE_SPEED 10.0f

// コンストラクタ
CPortalFragment::CPortalFragment(EFragmentType fragmentType)
	: CConnectObject(0.1f, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mIsMerge(false)
	, mMergePos(CVector::zero)
	, mpMergeArea(nullptr)
{
	std::string path = "";
	// 画像のパスを設定
	switch (fragmentType)
	{
	case EFragmentType::eFragment1:
		path = "Field/PortalFragment/PortalFragment1.png";
		break;
	case EFragmentType::eFragment2:
		path = "Field/PortalFragment/PortalFragment2.png";
		break;
	case EFragmentType::eFragment3:
		path = "Field/PortalFragment/PortalFragment3.png";
		break;
	case EFragmentType::eFragment4:
		path = "Field/PortalFragment/PortalFragment4.png";
		break;
	}
	// ポータルの欠片の画像を生成
	mpFragmentImg = new CImage3D(
		path,
		ETag::eConnectObject,
		ETaskPriority::eDefault, 0,
		ETaskPauseType::eGame
	);
	// 親に設定
	mpFragmentImg->SetParent(this);
	// 裏面描画する
	mpFragmentImg->SetBackFace(true);
	mpFragmentImg->Position(IMG_OFFSET_POS);

	// コライダーを生成
	CreateCol();

	// 接続ターゲットを生成
	CreateTarget(IMG_OFFSET_POS);
}

// デストラクタ
CPortalFragment::~CPortalFragment()
{
}

// 更新
void CPortalFragment::Update()
{
	// 基底クラスの更新処理
	CConnectObject::Update();

	// 合成場所に着いたなら
	if (mIsMerge)
	{
		// 合成地への方向
		CVector dir = mMergePos - Position();
		// 距離
		float distance = dir.Length();
		// 正規化
		dir.Normalize();
		// 移動距離
		float moveDistance = MERGE_SPEED * Times::DeltaTime();
		// 移動距離の方が長ければ
		if (distance < moveDistance)
		{
			// 座標を設定
			Position(mMergePos);
			// 無効
			SetEnable(false);
		}
		// 短ければ
		else
		{
			// 新しい座標を計算
			CVector newPos = Position() + dir * moveDistance;
			Position(newPos);
		}
	}
}

// 衝突処理
void CPortalFragment::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CConnectObject::Collision(self, other, hit);

	if (self == mpCol)
	{
		// 欠片を合成する場所の場合
		if (other->Layer() == ELayer::eFragmentMergeArea)
		{
			// 重力無効
			SetGravity(false);
			// コライダを無効
			SetEnableCol(false);
			// 合成場所の座標
			mMergePos = other->Owner()->Position();
			// 合成開始
			mIsMerge = true;
		}
	}
}

// コライダーを生成
void CPortalFragment::CreateCol()
{
	mpCol = new CColliderCapsule(
		this, ELayer::eObject,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, HEIGHT, 0.0f),
		RADIUS
	);
	// フィードとプレイヤーと
	// 敵と接続オブジェクトの探知用と欠片を合成する場所とだけ衝突判定
	mpCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eConnectSearch,
		ELayer::eFragmentMergeArea });
}
