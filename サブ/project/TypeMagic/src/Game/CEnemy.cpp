#include "CEnemy.h"
#include "CharaStatusDefine.h"
#include "CColliderSphere.h"
#include "CPlayer.h"
#include "Maths.h"
#include "SpellDefine.h"
#include "CEnemyContext.h"
#include "CEnemyIdleState.h"
#include "CEnemyCastState.h"

// 体の半径
#define BODY_RADIUS 4.0f
// 探知の半径
#define SEARCH_RADIUS 100.0f

// 詠唱文字のオフセット座標
#define SPELL_TEXT_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.4f, WINDOW_HEIGHT * 0.4f, 0.0f)

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
	, mState(CEnemyIdleState::Instance())
	, mIsSpellComing(false)
	, mPriorityScore(0.0f)
	, mSpellMoveDir(CVector::zero)
{
	// メイン属性の文字列を設定
	SetMainElementalStr(elemental);
	// 詠唱する形の文字列を設定
	SetCastShapeStr(ESpellShapeType::eBall);

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
	SAFE_DELETE(mpSpellSearch);
}

// 更新
void CEnemy::Update()
{
	// 詠唱状態でなければ
	if (mState != CEnemyCastState::Instance())
	{
		// 最適な行動に変更
		ChangeBestState();
	}

	// プレイヤーの方向を向き続ける
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// 少しずつ減速
	mMoveSpeed -= mMoveSpeed * DECREASE_MOVE_SPEED;

	// nullでなければ更新
	if (mState != nullptr)
		mState->Update(this);

	// MPの再生
	RegeneMp();

	// 敵の基底クラスの更新
	CEnemyBase::Update();
	// 詠唱呪文指定クラスの更新
	CCastSpellStr::Update();

#if _DEBUG
	CDebugPrint::Print("EnemyHit:%d\n", mHitCount);
	CDebugPrint::Print("EnemyHp:%d\n", GetHp());
	CDebugPrint::Print("EnemyMp:%f\n", GetMp());
	// 状況を取得
	CEnemyContext::EnemyContext context = GetContext();
	// スコア評価クラス
	CEnemyContext* enemyContext = CEnemyContext::Instance();

	CDebugPrint::Print("EnemyMpRatio:%f\n", context.mpRatio);
	CDebugPrint::Print("EnemyScoreIdle:%f\n", enemyContext->ScoreIdle(context));
	CDebugPrint::Print("EnemyScoreCast:%f\n", enemyContext->ScoreCast(context));
	CDebugPrint::Print("EnemyScoreChase:%f\n", enemyContext->ScoreChase(context));
	CDebugPrint::Print("EnemyScoreDodge:%f\n", enemyContext->ScoreDodge(context));
	CDebugPrint::Print("EnemyScoreRun:%f\n", enemyContext->ScoreRun(context));
	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("SpellComing:%s\n", mIsSpellComing ? "true" : "false");
#endif

	// 飛んできているかをリセット
	SetSpellComing(false);
}

// メイン属性の文字列を設定
void CEnemy::SetMainElementalStr(ESpellElementalType elemental)
{
	mMainElementalStr = GetElementStr(elemental);
}

// メイン属性の文字列を取得
std::string CEnemy::GetMainElementalStr() const
{
	return mMainElementalStr;
}

// 詠唱する形の文字列を設定
void CEnemy::SetCastShapeStr(ESpellShapeType shape)
{
	mCastShapeStr = GetShapeStr(shape);
}

// 詠唱する形の文字列を取得
std::string CEnemy::GetCastShapeStr() const
{
	return mCastShapeStr;
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

// 経過時間に加算
void CEnemy::AddElapsedTime(float addTime)
{
	mElapsedTime += addTime;
}

// 経過時間を取得
float CEnemy::GetElapsedTime() const
{
	return mElapsedTime;
}

// 呪文が飛んできているかを設定
void CEnemy::SetSpellComing(bool enable, ESpellShapeType shape, float score, CVector moveDir)
{
	// 飛んできているなら
	if (enable)
	{
		// 一個目の探知なら
		if (!mIsSpellComing)
		{
			// 呪文の形を設定
			mComingSpellShape = shape;
			// 優先度スコアを設定
			mPriorityScore = score;
			// 呪文の移動方向を設定
			mSpellMoveDir = moveDir;
		}
		// 二個目以降なら
		else
		{
			// 新しい呪文の方が優先度が高いなら
			if (mPriorityScore < score)
			{
				// 呪文の形を設定
				mComingSpellShape = shape;
				// 優先度スコアを設定
				mPriorityScore = score;
				// 呪文の移動方向を設定
				mSpellMoveDir = moveDir;
			}
		}
	}

	mIsSpellComing = enable;
}

// 飛んできている呪文の移動方向を取得
CVector CEnemy::GetComingSpellMoveDir() const
{
	return mSpellMoveDir;
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

	// 呪文探知用
	mpSpellSearch = new CColliderSphere
	(
		this, ELayer::eSpellSearch,
		SEARCH_RADIUS
	);
	// 攻撃とだけ衝突
	mpSpellSearch->SetCollisionLayers({ ELayer::eAttackCol });
}

// 状況を取得
CEnemyContext::EnemyContext CEnemy::GetContext()
{	
	// プレイヤーを取得
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
	// 状況情報
	CEnemyContext::EnemyContext context;
	// HP割合を設定
	context.hpRatio = (float)GetHp() / (float)GetMaxHp();
	// MP割合を設定
	context.mpRatio = (float)GetMp() / (float)GetMaxMp();
	// プレイヤーのHP割合を設定
	context.hpRatioP = (float)player->GetHp() / (float)GetMaxHp();
	// プレイヤーのMP割合を設定
	context.mpRatioP = (float)player->GetMp() / (float)GetMaxMp();
	// プレイヤーまでの距離を設定
	context.distanceToPlayer = (player->Position() - Position()).Length();

	// プレイヤーが詠唱中か詠唱状態なら
	if (player->IsCasting() || player->IsCastState())
	{
		// 詠唱している
		context.isPlayerCasting = true;
	}
	// プレイヤーが詠唱中でない
	else
	{
		context.isPlayerCasting = false;
	}
	// 呪文が飛んできているか
	context.isSpellComing = mIsSpellComing;
	// 飛んできている呪文の形
	context.shape = mComingSpellShape;
	// 飛んできている呪文のスコア
	context.comingSpellScore = mPriorityScore;

	return context;
}

// 最適な行動に変更する
void CEnemy::ChangeBestState()
{
	// 最適な行動に変更
	ChangeState(CEnemyContext::Instance()->GetBestState(GetContext()));
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