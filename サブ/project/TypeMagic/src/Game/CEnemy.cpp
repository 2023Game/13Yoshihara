#include "CEnemy.h"
#include "CharaStatusDefine.h"
#include "CColliderSphere.h"
#include "CPlayer.h"
#include "Maths.h"
#include "SpellDefine.h"
#include "CEnemyContext.h"
#include "CEnemyIdleState.h"
#include "CEnemyCastState.h"
#include "CGaugeUI3D.h"
#include "CEnemyManager.h"
#include "CTextUI3D.h"

// 体の半径
#define BODY_RADIUS 4.0f
// 探知の半径
#define SEARCH_RADIUS 100.0f

// 詠唱文字のオフセット座標
#define SPELL_TEXT_UI_OFFSET_POS CVector(30.0f, 30.0f, 0.0f)

// 詠唱文字のサイズ
#define SPELL_TEXT_SIZE 32

// 残りが少ないと判定する基準
#define LOW_PER 0.2f

// 逃げる距離の倍率
#define RUN_DIST_RATIO 1.5f

// ゲージのパス
#define GAUGE_PATH "UI\\gauge.png"
// ゲージの座標
#define GAUGE_POS CVector(0.0f,50.0f,0.0f)
// ゲージの大きさ
#define GAUGE_SIZE 2.0f
// ゲージ同士の間隔
#define GAUGE_DIST 7.5f

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
	, mSpellDist(0.0f)
{
	// メイン属性の文字列を設定
	SetMainElementalStr(elemental);
	// 詠唱する形の文字列を設定
	SetCastShapeStr(ESpellShapeType::eBall);

	// 重力無効
	mIsGravity = false;
	// 移動方向を向かない
	mIsMoveDir = false;

	// HPゲージを設定
	mpHpGauge = new CGaugeUI3D(this, GAUGE_PATH, true, CGaugeUI3D::EGaugeType::eHpGauge);
	mpHpGauge->SetShow(false);
	mpHpGauge->Position(Position() + GAUGE_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

	// MPゲージを設定
	mpMpGauge = new CGaugeUI3D(this, GAUGE_PATH, true, CGaugeUI3D::EGaugeType::eMpGauge);
	mpMpGauge->SetShow(false);
	mpMpGauge->Position(Position() + GAUGE_POS + CVector(0.0f, -GAUGE_DIST, 0.0f));
	mpMpGauge->SetMaxPoint(GetMaxMp());
	mpMpGauge->SetCurrPoint(GetMp());

	// 呪文の文字列テキストの持ち主を設定
	mpSpellText->SetOwner(this);

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CEnemy::~CEnemy()
{
	SAFE_DELETE(mpSpellSearch);

	// MPゲージが存在したら、一緒に削除する
	if (mpMpGauge != nullptr)
	{
		mpMpGauge->SetOwner(nullptr);
		mpMpGauge->Kill();
	}
}

// 更新
void CEnemy::Update()
{
	// プレイヤーの方向を向き続ける
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// 徐々に減速
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

	// HPゲージの更新
	mpHpGauge->Position(Position() + GAUGE_POS);
	mpHpGauge->SetShow(true);
	mpHpGauge->SetCurrPoint(GetHp());
	// MPゲージの更新
	mpMpGauge->Position(Position() + GAUGE_POS + CVector(0.0f, -GAUGE_DIST, 0.0f));
	mpMpGauge->SetShow(true);
	mpMpGauge->SetCurrPoint(GetMp());	
	
	// 詠唱文字列のテキストの更新
	CTextUI3D* text3d = dynamic_cast<CTextUI3D*>(mpSpellText);
	if (text3d != nullptr)
	{
		text3d->SetWorldPos(Position() + VectorX() * SPELL_TEXT_UI_OFFSET_POS.X() +
			CVector(0.0f,
				SPELL_TEXT_UI_OFFSET_POS.Y(),
				SPELL_TEXT_UI_OFFSET_POS.Z()));
	}

#if _DEBUG
	//CDebugPrint::Print("EnemyHp:%d\n", GetHp());
	//CDebugPrint::Print("EnemyMp:%f\n", GetMp());
	//// 状況を取得
	//CEnemyContext::EnemyContext context = GetContext();
	//// スコア評価クラス
	//CEnemyContext* enemyContext = CEnemyContext::Instance();

	//CDebugPrint::Print("EnemyMpRatio:%f\n", context.mpRatio);
	//CDebugPrint::Print("EnemyScoreIdle:%f\n", enemyContext->ScoreIdle(context));
	//CDebugPrint::Print("EnemyScoreCast:%f\n", enemyContext->ScoreCast(context));
	//CDebugPrint::Print("EnemyScoreChase:%f\n", enemyContext->ScoreChase(context));
	//CDebugPrint::Print("EnemyScoreDodge:%f\n", enemyContext->ScoreDodge(context));
	//CDebugPrint::Print("EnemyScoreRun:%f\n", enemyContext->ScoreRun(context));
	//CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
	//CDebugPrint::Print("SpellComing:%s\n", mIsSpellComing ? "true" : "false");
#endif

	// 飛んできているかをリセット
	SetSpellComing(false);
}

// オブジェクト削除処理
void CEnemy::DeleteObject(CObjectBase* obj)
{
	CEnemyBase::DeleteObject(obj);

	// 削除されたのが呪文のテキストなら
	// ポインタを空にする
	if (obj == mpSpellText)
	{
		mpSpellText = nullptr;
	}
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
void CEnemy::SetSpellComing(bool enable, ESpellShapeType shape,
	float score, CVector moveDir, float dist)
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
			// 呪文との距離を設定
			mSpellDist = dist;
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
				// 呪文との距離を設定
				mSpellDist = dist;
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

// 死亡処理
void CEnemy::Death()
{
	// 削除
	CEnemyManager::Instance()->DeleteEnemy(this);
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
	// 呪文までの距離を設定
	context.distanceToSpell = mSpellDist;

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
	context.comingShape = mComingSpellShape;
	// 飛んできている呪文のスコア
	context.comingSpellScore = mPriorityScore;

	// シールドを貼っているか
	context.isShield = mIsShield;
	// 詠唱中の呪文
	context.castShape = mSpellShape;

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