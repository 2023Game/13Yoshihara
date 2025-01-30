#include "CResident.h"
#include "CDebugFieldOfView.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "Primitive.h"
#include "Maths.h"
#include "CTrashBag.h"

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
#define BODY_HEIGHT 2.5f	// 本体のコライダ―の高さ

#define ROTATE_SPEED 9.0f	// 回転速度

#define IDLE_TIME 1.0f			// 待機状態の時間

#define SCALE 5.0f	// スケール

// 投げるときに見る座標
#define LOOK_POS0  CVector(0.0f,0.0f,-100.0f)
#define LOOK_POS1  CVector(0.0f,0.0f, -80.0f)
#define LOOK_POS2  CVector(0.0f,0.0f, -60.0f)
#define LOOK_POS3  CVector(0.0f,0.0f, -40.0f)
#define LOOK_POS4  CVector(0.0f,0.0f, -20.0f)
#define LOOK_POS5  CVector(0.0f,0.0f,   0.0f)
#define LOOK_POS6  CVector(0.0f,0.0f,  20.0f)
#define LOOK_POS7  CVector(0.0f,0.0f,  40.0f)
#define LOOK_POS8  CVector(0.0f,0.0f,  60.0f)
#define LOOK_POS9  CVector(0.0f,0.0f,  80.0f)
#define LOOK_POS10 CVector(0.0f,0.0f, 100.0f)

// ゴミを投げるときのオフセット座標
#define TRASH_BAG_OFFSET_POSY CVector(0.0f,2.0f,0.0f)
#define TRASH_BAG_OFFSET_POSZ 5.0f

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
	, mLookPos(CVector::zero)
	, mTrashBagNum(-1)
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

	// ゴミ袋の生成
	CreateTrashBag();

	// 本体のコライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eResident,
		CVector(0.0f, BODY_RADIUS / SCALE, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS / SCALE, 0.0f),
		BODY_RADIUS
	);
	// 地形、プレイヤー、敵、回収員、ゴミ袋
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy,ETag::eTrashBag});
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eCollector,ELayer::eTrashBag });

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);
}

// デストラクタ
CResident::~CResident()
{
	// リストをクリア
	mpTrashBags.clear();
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

// 次に巡回するポイントを変更する
void CResident::ChangePatrolPoint()
{
	// 巡回ポイントが設定されていない場合は、処理しない
	int size = mpPatrolPoints.size();
	if (size == 0) return;

	// 巡回開始時であれば、一番最初の巡回ポイントを選択
	if (mNextPatrolIndex == -1)
	{
		// 一番最初の巡回ポイントに設定する
		mNextPatrolIndex = 0;
	}
	// 移動が中断されていたら、
	// 巡回ポイントの番号を変更しない
	else if (mIsMovePause)
	{
		mIsMovePause = false;
		// 最後のノード（目的地のノード）だった場合、次の巡回ポイントを指定
		if (mNextMoveIndex >= mpMoveRoute.size())
		{
			mNextPatrolIndex++;
			if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
		}
	}
	// 巡回中だった場合、次の巡回ポイントを指定
	else
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
	}

	// 次に巡回するポイントが決まった場合
	if (mNextPatrolIndex >= 0)
	{
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			// 経路探索用のノードの座標を更新
			mpNavNode->SetPos(Position());

			// 次に巡回するポイントの経路を設定
			mpMoveRoute.clear();
			mpMoveRoute.push_back(mpNavNode);
			mpMoveRoute.push_back(mpPatrolPoints[mNextPatrolIndex]);
			// 次の目的地インデックスを設定
			mNextMoveIndex = 1;
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
		//巡回ポイントを求める
		ChangePatrolPoint();
		mStateStep++;
		break;

		// ステップ1：巡回ポイントまで移動
	case 1:
	{
		// 移動アニメーションを再生
		ChangeAnimation((int)EAnimType::eMove);

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
	// 向く座標へのベクトル
	CVector lookVec = mLookPos - Position();
	lookVec.Y(0.0f);
	// 向く方向
	CVector lookDir = lookVec.Normalized();
	// 徐々に向く方向へ向ける
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// 現在の正面方向
		lookDir,	// 向く方向
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// TODO：移動停止、ゴミ袋を手に生成、ランダムな方向に投げる
	switch (mStateStep)
	{
		// ステップ0：ゴミ袋を投げるアニメーション再生
	case 0:
	{
		// 向く座標をランダムで決定
		SetLookPosRandom();
		ChangeAnimation((int)EAnimType::eThrowBag);
		// ゴミ袋を有効
		if (!SpawnTrashBag())
		{
			// 投げるまでの時間をリセット
			SetThrowTime();
			// 生成できなければ巡回状態へ
			ChangeState(EState::ePatrol);
			return;
		}
		mpTrashBags[mTrashBagNum]->Position(Position() + TRASH_BAG_OFFSET_POSY +
			VectorZ() * TRASH_BAG_OFFSET_POSZ);
		mpTrashBags[mTrashBagNum]->SetGravity(false);
		mStateStep++;
		break;
	}
		// ステップ1：アタッチ解除して向いている方向に投げる
	case 1:
		mpTrashBags[mTrashBagNum]->Position(Position() + TRASH_BAG_OFFSET_POSY +
			VectorZ() * TRASH_BAG_OFFSET_POSZ);
		// アニメーションが50%進行したら
		if (GetAnimationFrameRatio() >= 0.2f)
		{
			// 重力をオン
			mpTrashBags[mTrashBagNum]->SetGravity(true);
			// ゴミを投げる速度を設定
			SetThrowSpeed();
			// 自分の正面方向に投げる速度を設定
			CVector throwSpeed = VectorZ() * GetThrowSpeed();
			// 飛んでいく速度を設定
			mpTrashBags[mTrashBagNum]->SetThrowSpeed(throwSpeed, GetThrowSpeed());
			mStateStep++;
		}
		break;
		// ステップ2：アニメーションが終了したら
	case 2:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;

		// ステップ3：投げるまでの時間をリセットして巡回状態へ
	case 3:
		// 投げるまでの時間をリセット
		SetThrowTime();
		// 生成中のゴミ袋の番号をリセット
		mTrashBagNum = -1;
		// 移動を中断しているなら
		if (mIsMovePause)
		{
			// 巡回状態へ
			ChangeState(EState::ePatrol);
		}
		// していないなら
		else
		{
			// 待機状態へ
			ChangeState(EState::eIdle);
		}
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

// 向く座標をランダムに決定する
void CResident::SetLookPosRandom()
{
	// 乱数を取得
	int random = Math::Rand(0, 10);

	// 向く座標を設定
	switch (random)
	{
	case 0:
		mLookPos = LOOK_POS0;
		break;
	case 1:
		mLookPos = LOOK_POS1;
		break;
	case 2:
		mLookPos = LOOK_POS2;
		break;
	case 3:
		mLookPos = LOOK_POS3;
		break;
	case 4:
		mLookPos = LOOK_POS4;
		break;
	case 5:
		mLookPos = LOOK_POS5;
		break;
	case 6:
		mLookPos = LOOK_POS6;
		break;
	case 7:
		mLookPos = LOOK_POS7;
		break;
	case 8:
		mLookPos = LOOK_POS8;
		break;
	case 9:
		mLookPos = LOOK_POS9;
		break;
	case 10:
		mLookPos = LOOK_POS10;
		break;
	}
}

// ゴミ袋の生成
void CResident::CreateTrashBag()
{
	// 投げれる最大数分作成
	for (int i = 0; i < GetMaxThrowNum(); i++)
	{
		mpTrashBags.push_back(new CTrashBag(false));
		// 無効にしておく
		mpTrashBags.back()->SetOnOff(false);
	}
}

// 生成できるゴミ袋を有効にする
bool CResident::SpawnTrashBag()
{
	// ゴミ袋の番号記憶用
	int i = -1;
	// 全てのゴミ袋を確認
	for (CTrashBag* trashBag : mpTrashBags)
	{
		i++;
		// 有効なら次へ
		if (trashBag->IsEnable()) continue;
		// ゴールド化ならゴールドに
		if (GoldBag())
		{
			trashBag->SetGold(true);
		}
		// そうでないなら通常
		else
		{
			trashBag->SetGold(false);
		}
		// 有効にする
		trashBag->SetOnOff(true);
		mTrashBagNum = i;
		return true;
	}
	return false;
}

// ゴミ袋がゴールドか否かを計算
bool CResident::GoldBag()
{
	// 1から100までの100個の数から乱数を取得
	int random = Math::Rand(1, 100);

	// ゴールド確率以下の値ならゴールド
	if (random <= GetGoldChance())
	{
		return true;
	}
	// そうでないなら通常
	else
	{
		return false;
	}
}
