#pragma once
#include "CObjectBase.h"
#include "CConnectObject.h"

// 接続できるターゲット部分
class CConnectTarget : public CObjectBase
{
public:
	// コンストラクタ
	CConnectTarget(CConnectObject* connectObj);
	// デストラクタ
	~CConnectTarget();

	// 描画
	void Render() override;

	// 接続部がついているオブジェクトを取得
	CConnectObject* GetConnectObj() const;
	// 接続部がついているオブジェクトを設定
	void SetConnectObj(CConnectObject* obj);

private:
	// 接続部がついているオブジェクト
	CConnectObject* mpConnectObj;
};