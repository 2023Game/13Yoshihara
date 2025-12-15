#pragma once
#include "CSwitchObject.h"

class CWeight;

// スイッチによって重りを生成する
class CWeightGenerator : public CSwitchObject
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	CWeightGenerator(const CVector& pos);
	~CWeightGenerator();

	void SetEnable(bool enable) override;

private:
	// データ保存に使用
	unsigned int mUniqueID;

	// 作用していないときの処理
	void UpdateOff() override;
	// 作用しているときの処理
	void UpdateOn() override;

	CWeight* mpWeight;
};