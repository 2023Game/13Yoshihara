#pragma once
#include "CCharaBase.h"

/*
ゴミ収集車から生成される
回収員のクラス
*/
class CCollector : public CCharaBase
{
public :
	// コンストラクタ
	CCollector();
	// デストラクタ
	~CCollector();

	// 本体コライダ―を取得
	CCollider* GetBodyCol() const;

private:
};