#pragma once
#include "CBillBoardImage.h"

class CConnectObject;

// 接続部分
class CConnectPoint : public CBillBoardImage
{
public:
	// コンストラクタ
	CConnectPoint(CConnectObject* connectObj);
	// デストラクタ
	~CConnectPoint();

	// 更新
	void Update() override;

	// 接続部がついているオブジェクトを取得する
	CConnectObject* GetConnectObj();

private:
	// 接続部がついているオブジェクト
	CConnectObject* mpConnectObj;
};