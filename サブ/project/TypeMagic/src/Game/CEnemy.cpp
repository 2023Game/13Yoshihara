#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CBall.h"
#include "CPlayer.h"

// 体の半径
#define BODY_RADIUS 4.0f

// 詠唱文字のオフセット座標
#define SPELL_TEXT_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.2f, 0.0f)

// 詠唱文字のサイズ
#define SPELL_TEXT_SIZE 32

// コンストラクタ
CEnemy::CEnemy()
	: CEnemyBase(0.0f, 0.0f, {}, 0.0f)
	, CCastSpellStr(this, ECastType::eBasic, {}, SPELL_TEXT_UI_OFFSET_POS, SPELL_TEXT_SIZE)
	, mElapsedTime(0.0f)
{
	// 重力無効
	mIsGravity = false;
	// 移動方向を向かない
	mIsMoveDir = false;

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CEnemy::~CEnemy()
{
}

// 更新
void CEnemy::Update()
{
	// プレイヤーの方向を向き続ける
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// 敵の基底クラスの更新
	CEnemyBase::Update();
	// 詠唱呪文指定クラスの更新
	CCastSpellStr::Update();

	mElapsedTime += Times::DeltaTime();
	if (mElapsedTime > 2.0f)
	{
		CastStart(ESpellElementalType::eThunder, ESpellShapeType::eBreath);
		mElapsedTime = 0.0f;
	}

#if _DEBUG
	CDebugPrint::Print("EnemyHit:%d\n", mHitCount);
#endif
}

// メイン属性を設定
void CEnemy::SetMainElemental(ESpellElementalType elemental)
{
	mMainElement = elemental;
}

// コライダーを生成
void CEnemy::CreateCol()
{
	// 本体コライダ
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eEnemy,
		BODY_RADIUS
	);
	// フィールド,壁、オブジェクトとプレイヤーと攻撃とだけ衝突
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::ePlayer,
		ELayer::eAttackCol});
}

// 待機状態
void CEnemy::UpdateIdle()
{
}

// 移動処理
void CEnemy::UpdateMove()
{
}

// 詠唱中
void CEnemy::UpdateCast()
{
}

// 追いかける
void CEnemy::UpdateChase()
{
}

// 回避中
void CEnemy::UpdateDodge()
{
}

// 逃げる
void CEnemy::UpdateRun()
{
}

// 状態切り替え
void CEnemy::ChangeState(EState state)
{
}
