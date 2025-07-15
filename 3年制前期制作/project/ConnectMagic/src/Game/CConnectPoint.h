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
	CConnectObject* GetConnectObj() const;
	// 接続部がついているオブジェクトを設定
	void SetConnectObj(CConnectObject* obj);

private:
	// 接続部がついているオブジェクト
	CConnectObject* mpConnectObj;
};