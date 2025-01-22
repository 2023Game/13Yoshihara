#include "CResident.h"
#include "CDebugFieldOfView.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "Primitive.h"

// コライダのインクルード
#include "CColliderCapsule.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Resident\\anim\\"

// 住人のアニメーションデータのテーブル
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{"",					 true, 0.0f,1.0f},	// Tポーズ
	{ ANIM_PATH"Idle.x",	 true,80.0f,1.0f},	// 待機
	{ ANIM_PATH"Move.x",	 true,80.0f,1.0f},	// 移動
	{ ANIM_PATH"ThrowBag.x",false,80.0f,1.0f},	// ゴミ袋を投げる
};

#define BODY_RADIUS 2.5f	// 本体のコライダ―の半径
#define BODY_HEIGHT 25.0f	// 本体のコライダ―の高さ

#define ROTATE_SPEED 9.0f	// 回転速度

#define PATROL_NEAR_DIST 10.0f	// 巡回開始時に選択される巡回ポイントの最短距離
#define IDLE_TIME 3.0f			// 待機状態の時間

#define SCALE 5.0f	// スケール

// コンストラクタ
CResident::CResident(CModelX* model, CVector startPos, 
	std::vector<CNavNode*> patrolPoints)
	: CEnemyBase
	(
		0.0f,
		0.0f,
		{},
		0.0f
	)
	, CResidentStatus()
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsMovePause(false)
{
	// モデルを設定
	mpModel = model;
	// 生成時座標を設定
	Position(startPos);

	// 巡回ポイントを設定
	mpPatrolPoints = patrolPoints;

	// 大きさの調整
	Scale(SCALE, SCALE, SCALE);
	// アニメーションとモデルの初期化
	InitAnimationModel("Resident", &ANIM_DATA);

	// 本体のコライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eResident,
		CVector(0.0f, BODY_RADIUS / SCALE, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS / SCALE, 0.0f),
		BODY_RADIUS
	);
	// 地形、プレイヤー、敵、回収員
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy});
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eCollector });

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);
}

// デストラクタ
CResident::~CResident()
{
}

// 更新
void CResident::Update()
{
	// 現在の状態に合わせて更新処理を切り替え
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::ePatrol:			UpdatePatrol();			break;
	case EState::eThrowBag:			UpdateThrowBag();		break;
	}

	// ゴミを投げるまでをカウントダウンする
	CountThrowTime();

	// キャラクターの更新
	CEnemyBase::Update();

#if _DEBUG
	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("ResidentState:%s\n", GetStateStr(mState).c_str());
#endif
}

// 衝突処理
void CResident::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CEnemyBase::Collision(self, other, hit);
}

// 描画
void CResident::Render()
{
	CEnemyBase::Render();

	// 巡回状態であれば、
	if (mState == EState::ePatrol)
	{
		// 巡回ポイントを全て描画
		int size = mpPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireBox
			(
				mpPatrolPoints[i]->GetPos() + CVector(0.0f, 1.0f, 0.0f),
				CVector::one,
				c
			);
		}
	}
}

// 待機状態
void CResident::UpdateIdle()
{
	// ゴミ袋を投げるまでの時間が経過したら
	if (IsElapsedThrowTime())
	{
		// ゴミ袋を投げる状態へ
		ChangeState(EState::eThrowBag);
		return;
	}

	// 待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);


	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// 待機時間が経過したら、巡回状態へ移行
		ChangeState(EState::ePatrol);
		mElapsedTime = 0.0f;
		mStateStep = 0;
	}
}

// 巡回処理
void CResident::UpdatePatrol()
{
	// ゴミ袋を投げるまでの時間が経過したら
	if (IsElapsedThrowTime())
	{
		// 移動の中断
		mIsMovePause = true;
		// ゴミ袋を投げる状態へ
		ChangeState(EState::eThrowBag);
		return;
	}

	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：巡回開始時の巡回ポイントを求める
	case 0:
		// 移動が中断されていなければ巡回ポイントを変更
		if (!mIsMovePause)
		{
			mNextPatrolIndex = -1;
			ChangePatrolPoint(PATROL_NEAR_DIST);
		}
		else
		{
			mIsMovePause = false;
		}
		mStateStep++;
		break;

		// ステップ1：巡回ポイントまで移動
	case 1:
	{
		// 移動アニメーションを再生
		ChangeAnimation((int)EAnimType::eMove);

		if (mpMoveRoute.size() == 1)
		{
			mNextMoveIndex = 0;
		}
		// 最短経路の次のノードまで移動
		CNavNode* moveNode = mpMoveRoute[mNextMoveIndex];

		if (MoveTo(moveNode->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			// 移動が終われば、次のノードへ切り替え
			mNextMoveIndex++;
			// 最後のノード（目的地のノード）だった場合は、次のステップへ進める
			if (mNextMoveIndex >= mpMoveRoute.size())
			{
				mStateStep++;
			}
		}
		break;
	}
	// ステップ2：移動後は待機状態へ
	case 2:
		ChangeState(EState::eIdle);
		break;
	}
}

// ゴミ袋を投げる処理
void CResident::UpdateThrowBag()
{
	// TODO：移動停止、ゴミ袋を手に生成、ランダムな方向に投げる
	switch (mStateStep)
	{
		// ステップ0：ゴミ袋を投げるアニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eThrowBag);
		mStateStep++;
		break;

		// ステップ1：アニメーションが終了したら
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;

		// ステップ2：投げるまでの時間をリセットして巡回状態へ
	case 2:
		// 投げるまでの時間をリセット
		SetThrowTime();
		// 巡回状態へ
		ChangeState(EState::ePatrol);
		break;
	}
}

// 状態切り替え
void CResident::ChangeState(EState state)
{
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// 状態の文字列を取得
std::string CResident::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "待機";
	case EState::ePatrol:			return "巡回";
	case EState::eThrowBag:			return "ゴミ袋を投げる";
	}
	return "";
}

// 状態の色を取得
CColor CResident::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return CColor::white;
	case EState::ePatrol:			return CColor::green;
	case EState::eThrowBag:			return CColor::red;
	}
	return CColor::white;
}
#endif