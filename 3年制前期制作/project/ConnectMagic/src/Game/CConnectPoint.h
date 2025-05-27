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

	// ペアの接続部を設定
	void SetPair(CConnectPoint* pair);

	// ペアの接続部を取得
	CConnectPoint* GetPair() const;

private:
	// 接続部がついているオブジェクト
	CConnectObject* mpConnectObj;

	// ペアの接続部
	CConnectPoint* mpPair;
};