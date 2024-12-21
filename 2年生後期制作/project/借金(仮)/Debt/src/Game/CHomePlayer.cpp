#include "CHomePlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"

#define CAPSULE_RADIUS 2.5f	// カプセルコライダの半径
#define PLAYER_HEIGHT 16.0f	// プレイヤーの高さ

#define MOVE_SPEED 0.375f * 2.0f

#define ANIM_PATH "Character\\Player\\anim\\"

// プレイヤーのアニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f},	// 待機
	{ ANIM_PATH"walk.x",		true,	66.0f,	1.0f},	// 歩行
};

CHomePlayer::CHomePlayer()
	: CPlayerBase(CAPSULE_RADIUS, PLAYER_HEIGHT)
	, mState(EState::eIdle)
{
	// アニメーションとモデルを初期化
	InitAnimationModel("Player", &ANIM_DATA);

	//フィールド、壁、オブジェクトとだけ衝突判定をする
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, CAPSULE_RADIUS, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT - CAPSULE_RADIUS, 0.0f),
		CAPSULE_RADIUS
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eObject });

	//インタラクトオブジェクトとだけ衝突判定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::ePlayer,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eInteract });

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);
}

CHomePlayer::~CHomePlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}
}

void CHomePlayer::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	}

	// 待機中は、移動処理を行う
	if (mState == EState::eIdle)
	{
		UpdateMove();
	}

	CPlayerBase::Update();

	CDebugPrint::Print("State:%d\n", mState);
}

void CHomePlayer::UpdateIdle()
{
}

// 移動の更新処理
void CHomePlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;

		// 待機状態であれば、移動アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation((int)EAnimType::eMove);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation((int)EAnimType::eIdle);
		}
	}
}

void CHomePlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}