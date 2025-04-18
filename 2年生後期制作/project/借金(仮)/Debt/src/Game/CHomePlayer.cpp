#include "CHomePlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CInteractObject.h"

#define BODY_RADIUS 2.5f	// 本体コライダの半径
#define BODY_HEIGHT 16.0f	// 本体コライダの高さ

#define SEARCH_RADIUS 10.0f	// 調べるオブジェクトの探知範囲の半径

#define BASE_MOVE_SPEED 0.375f * 2.0f		// 移動速度

#define ANIM_PATH "Character\\Player\\anim\\"

// プレイヤーのアニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	298.0f,	0.5f},	// 待機
	{ ANIM_PATH"walk.x",		true,	63.0f,	1.0f},	// 歩行
};

// コンストラクタ
CHomePlayer::CHomePlayer()
	: CPlayerBase()
	, mState(EState::eIdle)
{
	// アニメーションとモデルを初期化
	InitAnimationModel("Player", &ANIM_DATA);

	// 本体のコライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	// フィールド、壁、オブジェクトとだけ衝突
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall, ELayer::eObject });

	// 調べるオブジェクトを探知するコライダ―
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		SEARCH_RADIUS
	);
	mpSearchCol->Position(0.0f, SEARCH_RADIUS * 0.5f , 0.0f);
	// 調べるオブジェクトとのみ衝突
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eInteractObj });

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);
}

// デストラクタ
CHomePlayer::~CHomePlayer()
{

}

// 更新
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

#if _DEBUG
	CDebugPrint::Print("State:%d\n", mState);
#endif
}

// 待機状態
void CHomePlayer::UpdateIdle()
{
	// キー入力
	ActionInput();
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
		mMoveSpeed += move * BASE_MOVE_SPEED;

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

// アクションのキー入力
void CHomePlayer::ActionInput()
{
	// 接地していれば
	if (mIsGrounded)
	{
		// 近くの調べるオブジェクトを取得
		CInteractObject* obj = GetNearInteractObject();
		if (obj != nullptr)
		{
			// Fキーを押したら、近くの調べるオブジェクトを調べる
			if (CInput::PushKey('F'))
			{
				obj->Interact();
			}
		}
	}
}

// 衝突処理
void CHomePlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}