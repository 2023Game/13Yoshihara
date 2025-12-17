#pragma once
#include "btBulletDynamicsCommon.h"

struct CollisionData
{
	btCollisionObject* selfBody;	// 衝突した自身の剛体
	btCollisionObject* otherBody;	// 衝突した相手の剛体
	btVector3 contactNormal;		// 剛体Bから剛体Aへ向かう法線ベクトル
};