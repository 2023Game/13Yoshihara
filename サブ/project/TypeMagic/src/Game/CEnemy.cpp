#include "CEnemy.h"
#include "CharaStatusDefine.h"
#include "CColliderSphere.h"
#include "CPlayer.h"
#include "Maths.h"
#include "SpellDefine.h"
#include "CEnemyContext.h"
#include "CEnemyIdleState.h"

// 体の半径
#define BODY_RADIUS 4.0f

// 詠唱文字のオフセット座標
#define SPELL_TEXT_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.2f, 0.0f)

// 詠唱文字のサイズ
#define SPELL_TEXT_SIZE 32

// 残りが少ないと判定する基準
#define LOW_PER 0.2f

// 逃げる距離の倍率
#define RUN_DIST_RATIO 1.5f

// コンストラクタ
CEnemy::CEnemy(ESpellElementalType elemental)
	: CEnemyBase(0.0f, 0.0f, {}, 0.0f)
	, CEnemyStatus()
	, CCastSpellStr(this, ECastType::eBasic, {}, SPELL_TEXT_UI_OFFSET_POS, SPELL_TEXT_SIZE)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMainElemental(elemental)
	, mCastShape(ESpellShapeType::eBall)
	, mState(CEnemyIdleState::Instance())
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
	// 最適な行動に変更
	ChangeBestState();

	// プレイヤーの方向を向き続ける
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// 少しずつ減速
	mMoveSpeed -= mMoveSpeed * DECREASE_MOVE_SPEED;

	// nullでなければ更新
	if (mState != nullptr)
		mState->Update(this);

	// 敵の基底クラスの更新
	CEnemyBase::Update();
	// 詠唱呪文指定クラスの更新
	CCastSpellStr::Update();

#if _DEBUG
	CDebugPrint::Print("EnemyHit:%d\n", mHitCount);
	CDebugPrint::Print("EnemyHp:%d\n", GetHp());
	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
#endif
}

// メイン属性を設定
void CEnemy::SetMainElemental(ESpellElementalType elemental)
{
	mMainElemental = elemental;
}

// メイン属性を取得
ESpellElementalType CEnemy::GetMainElemental() const
{
	return mMainElemental;
}

// 詠唱する形を取得
ESpellShapeType CEnemy::GetCastShape() const
{
	return mCastShape;
}

// 状態内のステップを設定
void CEnemy::SetStateStep(int step)
{
	mStateStep = step;
}

// 状態内のステップを取得
int CEnemy::GetStateStep() const
{
	return mStateStep;
}

// 経過時間を設定
void CEnemy::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// 経過時間を取得
float CEnemy::GetElapsedTime() const
{
	return mElapsedTime;
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

// 最適な行動に変更する
void CEnemy::ChangeBestState()
{
	// プレイヤーを取得
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
	// 状況情報
	CEnemyContext::EnemyContext context;
	// HP割合を設定
	context.hpRatio = GetHp() / GetMaxHp();
	// MP割合を設定
	context.mpRatio = GetMp() / GetMaxMp();
	// プレイヤーまでの距離を設定
	context.distanceToPlayer = (player->Position() - Position()).Length();
	// プレイヤーが詠唱しているか
	context.isPlayerCasting = player->IsCasting();
	// 近くに壁があるかを設定
	context.isNearWall = false;

	// 最適な行動に変更
	ChangeState(CEnemyContext::Instance()->GetBestState(context));
}

// 状態切り替え
void CEnemy::ChangeState(CEnemyStateBase* state)
{
	// nullか同じなら処理しない
	if (state == nullptr ||
		mState == state) return;
	// 現在の状態は終了
	mState->End(this);
	// 状態を変更
	mState = state;
	// 新しい状態を開始
	mState->Start(this);
}

#if _DEBUG
// 状態の文字列を取得
std::string CEnemy::GetStateStr(CEnemyStateBase* state) const
{
	return state->GetName();
}
#endif