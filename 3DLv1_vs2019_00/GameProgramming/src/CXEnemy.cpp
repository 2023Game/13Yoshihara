#include "CXEnemy.h"

//コライダの初期化
CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColSphereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
	, mColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
{
	mTag = ETag::EENEMY;
}

//合成行列の設定
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[1]);
	//体
	mColSphereBody.Matrix(&mpCombinedMatrix[1]);
	//剣
	mColSphereSword0.Matrix(&mpCombinedMatrix[26]);
	mColSphereSword1.Matrix(&mpCombinedMatrix[26]);
	mColSphereSword2.Matrix(&mpCombinedMatrix[26]);
}

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	switch (m->Type())
	{
	case CCollider::EType::ESPHERE://自分が球コライダ
		if (o->Type() == CCollider::EType::ESPHERE &&//相手が球コライダ
			o->Tag() == CCollider::ETag::ESWORD &&//相手のタグが剣
			o->Parent()->Tag() == CCharacter3::ETag::EPLAYER &&//相手の親がプレイヤー
			m->Tag() == CCollider::ETag::EBODY)//自分のタグが体
		{
			//衝突判定
			if (m->Collision(m, o) == true)
			{
				//30フレームかけてダウンし、繰り返さない
				ChangeAnimation(11, false, 30);
			}
		}
	}
}
