#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"

class CXEnemy : public CXCharacter
{
public:
	//コライダの初期化
	CXEnemy();
	void Update();
	//合成行列の設定
	void Init(CModelX* model);
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
private:
	//コライダの宣言
	CCollider mColSphereHead;//頭
	CCollider mColSphereBody;//体
	CCollider mColSphereSword0;//剣
	CCollider mColSphereSword1;//剣
	CCollider mColSphereSword2;//剣
	CColliderCapsule mColBody;//体
};
#endif