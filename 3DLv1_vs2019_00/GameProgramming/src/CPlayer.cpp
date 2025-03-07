//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CTaskManager.h"
#include "CApplication.h"

#define ROTATION_XV CVector(1.0f,0.0f,0.0f)//回転速度
#define ROTATION_YV CVector(0.0f,1.0f,0.0f)//回転速度
#define VELOCITY CVector(0.0f,0.0f,0.1f)//移動速度

CPlayer* CPlayer::spInstance = nullptr;

CPlayer* CPlayer::Instance()
{
	return spInstance;
}


//CPlayer(位置,回転,スケール)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale);//行列の更新
}

//更新処理
void CPlayer::Update()
{
	//スペースキー入力で弾発射
	if (mInput.Key(VK_SPACE))
	{
		CBullet* bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet->Rotation(mRotation);
		bullet->Update();
		//CApplication::TaskManager()->Add(bullet);
	}         
	//Sキー入力で下向き
	if (mInput.Key('S'))
	{
		//X軸の回転値を減算
		mRotation = mRotation - ROTATION_XV;
	}
	//Wキー入力で上向き
	if (mInput.Key('W'))
	{
		//X軸の回転値を加算
		mRotation = mRotation + ROTATION_XV;
	}
	//Aキー入力で回転
	if (mInput.Key('A'))
	{
		//Y軸の回転値を増加
		mRotation = mRotation + ROTATION_YV;
	}

	//Dキー入力で回転
	if (mInput.Key('D'))
	{
		//Y軸の回転値を減少
		mRotation = mRotation - ROTATION_YV;
	}

	//上キー入力で前進
	if (mInput.Key(VK_UP))
	{
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}

	//変換行列の更新
	CTransform::Update();

	//UI設定
	CApplication::Ui()->PosY(mPosition.Y());
	CApplication::Ui()->RotX(mRotation.X());
	CApplication::Ui()->RotY(mRotation.Y());
}

CPlayer::CPlayer()
{
	//インスタンスの設定
	spInstance = this;
}

void CPlayer::Collision(CCollider* m, CCollider* o)
{
	//自身のコライダタイプの判定
	switch (m->Type())
	{
	case CCollider::EType::ELINE://線分コライダ
			//相手のコライダが三角コライダの時
		if (o->Type() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;//調整用ベクトル
		    //三角形と線分の衝突判定
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//位置の更新(mPosition + adjust)
				mPosition = mPosition + adjust;
				//行列の更新
				CTransform::Update();
			}
		}

			break;
	}
}

//衝突処理
void CPlayer::Collision()
{
	//コライダの優先度変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}