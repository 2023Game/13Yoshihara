#include "CTeleport.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"

// コライダーの半径
#define RADIUS 15.0f

// コンストラクタ
CTeleport::CTeleport(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eTeleport, owner, target, TELEPORT_DELETE_TIME)
{
	// 最小スケールに設定
	Scale(TELEPORT_SCALE_MIN);
	mpModel = CResourceManager::Get<CModel>("NeutralTeleport");
	// コライダーを生成
	CreateCol();
	// 待機状態
	ChangeState(EState::eIdle);
	// コライダーを有効
	mpSpellCol->SetEnable(true);
}

// デストラクタ
CTeleport::~CTeleport()
{
}

// コライダーを生成
void CTeleport::CreateCol()
{
	mpSpellCol = new CColliderSphere(
		this, ELayer::eDefenseCol,
		RADIUS,
		true
	);
	// 攻撃判定とだけ衝突判定
	mpSpellCol->SetCollisionLayers({ ELayer::eAttackCol });
}

// 待機中の更新
void CTeleport::UpdateIdle()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// 大きくなっていく
	case 0:
		// 拡大
		Scale(Scale() + TELEPORT_SCALE_UP_NUM);

		// 最大スケール以上になったら
		if (Scale().X() >= TELEPORT_SCALE_MAX.X())
		{
			mElapsedTime = 0.0f;
			// 最大に設定
			Scale(TELEPORT_SCALE_MAX);
			// 持ち主の更新と描画を無効
			mpOwner->SetEnable(false);
			mpOwner->SetShow(false);
			// 次へ
			mStateStep++;
		}
		break;

		// 小さくなっていく
	case 1:
		// 縮小
		Scale(Scale() - TELEPORT_SCALE_UP_NUM);

		// 最小スケール以下になったら
		if (Scale().X() <= TELEPORT_SCALE_MIN.X())
		{
			mElapsedTime = 0.0f;
			// 最小に設定
			Scale(TELEPORT_SCALE_MIN);
			// 次へ
			mStateStep++;
		}
		break;

	case 2:
		// 待機時間が経過したら
		if (mElapsedTime > TELEPORT_IDLE_TIME)
		{
			// 発射状態へ
			ChangeState(EState::eShooting);
			// 親についていく
			SetParent(mpOwner);
			return;
		}
		break;
	}
}

// 発射中の更新
void CTeleport::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// 移動
	case 0:
	{
		// キャラクラスを取得
		CCharaBase* chara = dynamic_cast<CCharaBase*>(mpOwner);

		// 移動
		CVector moveSpeed = chara->GetMoveDir() * TELEPORT_MOVE_DIST;

		// プレイヤーの座標を変更
		mpOwner->Position(mpOwner->Position() + moveSpeed);
		mStateStep++;
		break;
	}

		// 大きくなっていく
	case 1:
		// 拡大
		Scale(Scale() + TELEPORT_SCALE_UP_NUM);

		// 最大スケール以上になったら
		if (Scale().X() >= TELEPORT_SCALE_MAX.X())
		{
			mElapsedTime = 0.0f;
			// 最大に設定
			Scale(TELEPORT_SCALE_MAX);
			// 持ち主の更新と描画を有効
			mpOwner->SetEnable(true);
			mpOwner->SetShow(true);
			// 次へ
			mStateStep++;
		}
		break;

		// 小さくなっていく
	case 2:
		// 縮小
		Scale(Scale() - TELEPORT_SCALE_UP_NUM);

		// 最小スケール以下になったら
		if (Scale().X() <= TELEPORT_SCALE_MIN.X())
		{
			// 削除
			Kill();
		}

		break;
	}
}
