#include "CPunisherTruck.h"
#include "CGaugeUI3D.h"
#include "CTrashPlayer.h"
#include "CPunisherCollector.h"

// お仕置き用のHpゲージの画像のパス
#define PUNISHER_HP_GAUGE_PATH "UI\\punisher_garbageTruck_hp_gauge.png"

// プレイヤーとの座標比較する時の閾値
#define THRESHOLD 10.0f

// 回収員生成のオフセット座標
#define COLLECTOR_OFFSET_POS VectorX() * 10.0f

// コンストラクタ
CPunisherTruck::CPunisherTruck(CModel* model, const CVector& pos, const CVector& rotation,
	ERoadType road, std::vector<CNavNode*> patrolPoints)
	: CGarbageTruck(model, pos, rotation, road, patrolPoints, true)
{
	// お仕置き用のHpゲージを設定
	mpHpGauge = new CGaugeUI3D(this, PUNISHER_HP_GAUGE_PATH);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());
	// 最初は無効
	mpHpGauge->SetEnable(false);
	mpHpGauge->SetShow(false);

	int num = GetCollectorsNum();
	// 回収員を全て生成し、無効にしておく
	for (int i = 0; i < num; i++)
	{
		mpCollectors.push_back(new CPunisherCollector(this,
			{ mpNode0,mpNode1,mpNode2,mpNode3 }));
		// 無効にする
		mpCollectors[i]->SetOnOff(false);
	}
}

// デストラクタ
CPunisherTruck::~CPunisherTruck()
{
}

// 変数をリセット
void CPunisherTruck::Reset()
{
	CGarbageTruck::Reset();
	// Hpをリセット
	SetHp();
}

// 移動処理
void CPunisherTruck::UpdateMove()
{
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());

	// 自身のZ座標がプレイヤーのZ座標から閾値の範囲内の場合かつ
	// 回収が開始できる座標かつ撤退中ではない場合
	if (Position().Z() <= player->Position().Z() + THRESHOLD &&
		Position().Z() >= player->Position().Z() - THRESHOLD &&
		CanCollectPosZ() && !mIsReturn)
	{
		// 回収状態へ
		ChangeState(EState::eCollect);
		return;
	}
	// 通常のゴミ収集車の移動処理をする
	CGarbageTruck::UpdateMove();
}

// 回収処理
void CPunisherTruck::UpdateCollect()
{
	switch (mStateStep)
	{
		// ステップ0：設定を変更する
	case 0:
	{
		// 動いていない
		mIsMove = false;
		// 移動速度をゼロにする
		mMoveSpeed = CVector::zero;
		// 撤退までの時間を初期値に戻す
		SetReturnTime();
		// 回収員の人数を初期値に戻す
		SetCollectorsNum();

		int size = mpCollectors.size();
		// 回収員を全て有効にする
		for (int i = 0; i < size; i++)
		{
			// 有効にする
			mpCollectors[i]->SetOnOff(true);
			// 自分の座標＋オフセット座標を設定
			mpCollectors[i]->Position(Position() + COLLECTOR_OFFSET_POS * (i + 1));
		}

		mStateStep++;
		break;
	}

	// ステップ1：壊れるまで回収状態であり続ける
	case 1:
	{
		int size = mpCollectors.size();
		// 無効になっても有効にし直す
		for (int i = 0; i < size; i++)
		{
			if (!mpCollectors[i]->IsEnable())
			{
				// 有効にする
				mpCollectors[i]->SetOnOff(true);
				// 自分の座標＋オフセット座標を設定
				mpCollectors[i]->Position(Position() + COLLECTOR_OFFSET_POS * (i + 1));
			}
		}
		break;
	}
	}
}
