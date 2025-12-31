#pragma once
#include "btBulletDynamicsCommon.h"

struct CollisionData
{
	btCollisionObject* selfBody;	// 衝突した自身の剛体
	btCollisionObject* otherBody;	// 衝突した相手の剛体
	CObjectBase* otherObj;			// 相手オブジェクト
	CVector hitPoint;				// 衝突位置
	CVector contactNormal;		// 相手から自分へ向かう法線ベクトル
};