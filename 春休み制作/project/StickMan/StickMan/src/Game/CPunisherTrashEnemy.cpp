#include "CPunisherTrashEnemy.h"
#include "CGaugeUI3D.h"
#include "CDebugFieldOfView.h"
#include "CColliderCapsule.h"
#include "Maths.h"

// 大きさ
#define SCALE CVector(0.5f,0.5f,0.5f)

#define BODY_RADIUS 12.5f	// 本体のコライダ―の半径
#define BODY_HEIGHT 25.0f	// 本体のコライダ―の高さ
#define BODY_WIDTH 50.0f	// 本体のコライダ―の幅

// 攻撃コライダ―
#define ATTACK_COL_RADIUS	10.0f	// 半径
#define ATTACK_COL_HEIGHT	25.0f	// 高さ
#define ATTACK_COL_WIDTH	70.0f*2.5f	// 幅
// オフセット座標
#define ATTACK_COL_OFFSET_POS CVector(0.0f,-10.0f,80.0f)

// クリティカル攻撃コライダー
#define CRITICAL_COL_RADIUS		15.0f		// 半径
#define CRITICAL_COL_HEIGHT		25.0f		// 高さ
#define CRITICAL_COL_WIDTH		140.0f*2.0f		// 幅
// オフセット座標
#define CRITICAL_COL_OFFSET_POS CVector(0.0f,-10.0f,140.0f)

#define ATTACK_RANGE 90.0f	// 攻撃する距離

// Hpゲージのオフセット座標
#define GAUGE_OFFSET_POS CVector(0.0f,37.5f,0.0f)

// コンストラクタ
CPunisherTrashEnemy::CPunisherTrashEnemy()
	:CTrashEnemy(true, true)
{
	// 大きさの設定
	Scale(SCALE);
	// ゴミ袋の数の初期値を設定
	SetTrashBag(GetDefaultBagNum());
	SetGoldTrashBag(GetDefaultGoldBagNum());

	// コライダーを作成
	CreateCol();
	
	// 最初は無効
	mpHpGauge->SetEnable(false);
	mpHpGauge->SetShow(false);
#if _DEBUG
	mpDebugFov->SetEnable(false);
	mpDebugFov->SetShow(false);
#endif
}

// デストラクタ
CPunisherTrashEnemy::~CPunisherTrashEnemy()
{
}

// コライダーを作成する
void CPunisherTrashEnemy::CreateCol()
{
	// 大きさの取得
	float scale = SCALE.X();
	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(BODY_WIDTH - BODY_RADIUS / scale, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS / scale, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	// 地形、プレイヤー、敵、回収員、攻撃、車両、キャラの探知用、ゴミ袋
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle, ETag::eTrashBag });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,ELayer::eCharaSearch,
		ELayer::ePlayer, ELayer::eEnemy,ELayer::eCollector, ELayer::eAttackCol, ELayer::eVehicle, ELayer::eTrashBag });

	// 攻撃コライダー
	mpAttackCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, ATTACK_COL_HEIGHT, ATTACK_COL_WIDTH - ATTACK_COL_RADIUS * 10),
		CVector(0.0f, ATTACK_COL_HEIGHT, -ATTACK_COL_WIDTH + ATTACK_COL_RADIUS * 10),
		ATTACK_COL_RADIUS
	);
	// クリティカル攻撃コライダー
	mpCriticalCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, CRITICAL_COL_HEIGHT, CRITICAL_COL_WIDTH - CRITICAL_COL_RADIUS * 10),
		CVector(0.0f, CRITICAL_COL_HEIGHT, -CRITICAL_COL_WIDTH + CRITICAL_COL_RADIUS * 10),
		CRITICAL_COL_RADIUS
	);

	// プレイヤーと衝突判定するように設定
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->SetCollisionLayers({ ELayer::ePlayer });
	mpCriticalCol->SetCollisionTags({ ETag::ePlayer });
	mpCriticalCol->SetCollisionLayers({ ELayer::ePlayer });

	// 自分の前に位置調整
	mpAttackCol->Position(ATTACK_COL_OFFSET_POS);
	mpCriticalCol->Position(CRITICAL_COL_OFFSET_POS);

	// 攻撃コライダーは最初はオフにしておく
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);
}

// 巡回状態から他の状態へ移行する条件をチェック
// お仕置き用はすぐ追いかける
bool CPunisherTrashEnemy::ChangePatrolToOther()
{
	ChangeState(EState::eChase);
	return true;
}

// 追跡状態から他の状態へ移行する条件をチェック
// お仕置き用は追いかけ続ける
bool CPunisherTrashEnemy::ChangeChaseToOther()
{
	// プレイヤーに攻撃できるならば、攻撃状態へ移行
	if (CanAttackPlayer(ATTACK_RANGE))
	{
		// 1から100までの100個の数から乱数を取得
		int random = Math::Rand(1, 100);
		// クリティカル確率以下の値ならクリティカル攻撃
		if (random <= GetCriticalChance())
		{
			ChangeState(EState::eCriticalStart);
		}
		// それ以外の時は通常攻撃
		else
		{
			ChangeState(EState::eAttackStart);
		}
		return true;
	}
	return false;
}

// Hpゲージの更新
void CPunisherTrashEnemy::UpdateHpGauge()
{
	mpHpGauge->Position(Position() + GAUGE_OFFSET_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());
}
