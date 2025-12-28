#pragma once
#include "LinearMath/btIDebugDraw.h"

class CPhysicsDebugDraw : public btIDebugDraw
{
public:
	// コンストラクタ
	CPhysicsDebugDraw();

	// ワイヤーフレーム描画
	virtual void drawLine(
		const btVector3& from,
		const btVector3& to,
		const btVector3& color) override;

	// 衝突点を描画
	virtual void drawContactPoint(
		const btVector3& PointOnB,
		const btVector3& normalOnB,
		btScalar distance,
		int lifeTime,
		const btVector3& color) override;

	// エラーレポート
	virtual void reportErrorWarning(const char* warningString) override;

	// テキストの描画
	virtual void draw3dText(const btVector3& location, const char* textString) override;

	// デバッグモードの設定と取得
	virtual void setDebugMode(int debugMode) override;
	virtual int getDebugMode() const override;

private:
	int mDebugMode;
};