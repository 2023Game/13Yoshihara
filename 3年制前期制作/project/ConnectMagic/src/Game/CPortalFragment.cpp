#include "CPortalFragment.h"
#include "CImage3D.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CFragmentMergeArea.h"
#include "CConnectPointManager.h"
#include "CConnectTarget.h"

// 画像の座標
const CVector IMG_OFFSET_POS =		CVector(0.0f, 10.0f, 0.0f);
// 画像のサイズ
constexpr float IMG_SIZE =			2.0f;

// コライダーの高さ
constexpr float HEIGHT =			10.0f;
// コライダーの半径
constexpr float RADIUS =			10.0f;

// 合成の移動速度
constexpr float MERGE_SPEED =		10.0f;

// 飛び上がりの移動速度
constexpr float JUMP_MOVE_SPEEDY =	100.0f;
constexpr float JUMP_MOVE_SPEED =	20.0f;

// 待機の上下移動速度
constexpr float IDLE_MOVE_SPEED =	2.0f;
// 待機の上下が切り替わる時間
constexpr float CHANGE_UD_TIME =	1.0f;

// コンストラクタ
CPortalFragment::CPortalFragment(EFragmentType fragmentType,
	CFragmentMergeArea* mergeArea)
	: CConnectObject(0.1f, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, CDropItem(this)
	, mFragmentType(fragmentType)
	, mpMergeArea(mergeArea)
{
	// 最初は無効
	SetEnable(false);
	SetShow(false);
	// 飛び上がり状態
	ChangeState(EState::eJump);

	std::string path = "";
	// 画像のパスを設定
	switch (fragmentType)
	{
	case EFragmentType::eFragment1:
		path = "Field/PortalFragment/PortalFragment1.png";
		break;
	case EFragmentType::eFragment2:
		path = "Field/PortalFragment/PortalFragment2.png";
		break;
	case EFragmentType::eFragment3:
		path = "Field/PortalFragment/PortalFragment3.png";
		break;
	case EFragmentType::eFragment4:
		path = "Field/PortalFragment/PortalFragment4.png";
		break;
	}
	// ポータルの欠片の画像を生成
	mpFragmentImg = new CImage3D(
		path,
		ETag::eConnectObject,
		ETaskPriority::eDefault, 0,
		ETaskPauseType::eGame,
		false,
		false
	);
	// 親に設定
	mpFragmentImg->SetParent(this);
	// 裏面描画する
	mpFragmentImg->SetBackFace(true);
	// 座標を設定
	mpFragmentImg->Position(IMG_OFFSET_POS);
	// サイズを設定
	mpFragmentImg->SetSize(mpFragmentImg->GetSize() * IMG_SIZE);

	// コライダーを生成
	CreateCol();

	// 接続ターゲットを生成
	CreateTarget(IMG_OFFSET_POS);
}

// デストラクタ
CPortalFragment::~CPortalFragment()
{
	SAFE_DELETE(mpFragmentImg);
}

// 更新
void CPortalFragment::Update()
{
	switch (mState)
	{
	case CPortalFragment::EState::eJump:
		UpdateJump();
		break;
	case CPortalFragment::EState::eIdle:
		UpdateIdle();
		break;
	case CPortalFragment::EState::eMerge:
		UpdateMerge();
		break;
	}
	// 基底クラスの更新処理
	CConnectObject::Update();

	mpFragmentImg->Update();
}

// 描画
void CPortalFragment::Render()
{
	// 画像の描画
	mpFragmentImg->Render();
}

// 衝突処理
void CPortalFragment::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CConnectObject::Collision(self, other, hit);

	if (self == mpCol)
	{
		// 相手が自身の合成場所の場合
		if (other->Owner() == mpMergeArea)
		{
			// 重力無効
			SetGravity(false);
			// コライダを無効
			SetEnableCol(false);
			for (CConnectTarget* target : mTargets)
			{
				// 接続されていれば解除
				CConnectPointManager::Instance()->DisableConnect(target);
				target->SetConnectObj(nullptr);
				target->Kill();
			}
			// 配列を空にする
			mTargets.clear();

			// 合成開始
			ChangeState(EState::eMerge);
		}
	}
}

// 状態切り替え
void CPortalFragment::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
	mMoveSpeedY = 0.0f;
}

// 飛び上がり
void CPortalFragment::UpdateJump()
{
	switch (mStateStep)
	{
		// 上方向の移動を設定
	case 0:
		mMoveSpeedY = JUMP_MOVE_SPEEDY * Times::DeltaTime();
		mStateStep++;
		break;

		// 前方向の移動を設定
	case 1:
		// 前方向の移動
		mMoveSpeed += VectorZ() * JUMP_MOVE_SPEED * Times::DeltaTime();

		if (mIsGrounded)
		{
			// 待機へ
			ChangeState(EState::eIdle);
			return;
		}
		break;
	}
}

// 待機
void CPortalFragment::UpdateIdle()
{
	// 時間経過
	mElapsedTime += Times::DeltaTime();
	// 画像の座標
	CVector pos = mpFragmentImg->Position();
	// 移動
	CVector move = CVector(0.0f, IDLE_MOVE_SPEED * Times::DeltaTime(), 0.0f);

	switch (mStateStep)
	{
		// 上移動
	case 0:
		// 移動を反映
		pos = pos + move;

		// 切り替わる時間を過ぎたら
		if (mElapsedTime > CHANGE_UD_TIME)
		{
			mElapsedTime = 0.0f;
			mStateStep++;
		}
		break;

		// 下移動
	case 1:
		// 移動を反映
		pos = pos - move;

		// 切り替わる時間を過ぎたら
		if (mElapsedTime > CHANGE_UD_TIME)
		{
			mElapsedTime = 0.0f;
			mStateStep--;
		}
		break;
	}

	// 座標を設定
	mpFragmentImg->Position(pos);
}

// 合成
void CPortalFragment::UpdateMerge()
{		
	// 目標の座標
	CVector targetPos = mpMergeArea->Position();
	// 合成地への方向
	CVector dir = targetPos - Position();
	// 距離
	float distance = dir.Length();
	// 正規化
	dir.Normalize();
	// 移動距離
	float moveDistance = MERGE_SPEED * Times::DeltaTime();
	// 移動距離の方が長ければ
	if (distance < moveDistance)
	{
		// 画像の高さをリセット
		CVector pos = mpFragmentImg->Position();
		pos.Y(targetPos.Y() + IMG_OFFSET_POS.Y());
		mpFragmentImg->Position(pos);
		// 欠片がある
		mpMergeArea->SetIsFragment((int)mFragmentType, true);
		// 座標を設定
		Position(targetPos);
		// 無効
		SetEnable(false);
	}
	// 短ければ
	else
	{
		// 新しい座標を計算
		CVector newPos = Position() + dir * moveDistance;
		Position(newPos);
	}
}

// コライダーを生成
void CPortalFragment::CreateCol()
{
	mpCol = new CColliderSphere(
		this, ELayer::eObject,
		RADIUS
	);
	// フィードとプレイヤーと
	// 敵と接続オブジェクトの探知用と欠片を合成する場所とだけ衝突判定
	mpCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eConnectSearch,
		ELayer::eFragmentMergeArea });
	// 座標を設定
	mpCol->Position(IMG_OFFSET_POS * 0.5f);
}

// すべて削除
void CPortalFragment::Delete()
{
	mpFragmentImg->Kill();
}
