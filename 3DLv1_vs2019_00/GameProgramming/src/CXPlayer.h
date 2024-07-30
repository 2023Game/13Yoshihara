#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CActionCamera.h"

class CXPlayer : public CXCharacter
{
public:
	//コライダの初期化
	CXPlayer();
	//更新
	void Update();
	//合成行列の設定
	void Init(CModelX* model);
	//衝突判定
	void Collision(CCollider* m, CCollider* o);
private:
	CInput mInput;
	//コライダの宣言
	CCollider mColSphereHead;//頭
	CCollider mColSphereBody;//体
	CCollider mColSphereSword;//剣
	CColliderCapsule mColBody;//体
};
#endif