#include "CXEnemy.h"

//コライダの初期化
CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	mTag = ETag::EENEMY;
}

//合成行列の設定
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//体
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
	//剣
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
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
