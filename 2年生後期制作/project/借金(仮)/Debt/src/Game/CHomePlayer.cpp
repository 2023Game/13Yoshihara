#include "CHomePlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"

#define CAPSULE_RADIUS 2.5f	// カプセルコライダの半径
#define PLAYER_HEIGHT 16.0f	// プレイヤーの高さ

#define MOVE_SPEED 0.375f * 2.0f


// プレイヤーのアニメーションデータのテーブル
const CHomePlayer::AnimData CHomePlayer::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	{ "Character\\Player\\anim\\idle.x",		true,	153.0f	},	// 待機
	{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// 歩行
};

CHomePlayer::CHomePlayer()
	: CPlayerBase(CAPSULE_RADIUS, PLAYER_HEIGHT)
	, mState(EState::eIdle)
{
	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);


	//インタラクトオブジェクトとだけ衝突判定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::ePlayer,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eInteract });

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);
}

CHomePlayer::~CHomePlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}
	if (mpColliderCapsule != nullptr)
	{
		delete mpColliderCapsule;
		mpColliderCapsule = nullptr;
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

	// 待機中とジャンプ中は、移動処理を行う
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
			ChangeAnimation(EAnimType::eMove);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eIdle);
		}
	}
}

void CHomePlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

void CHomePlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}