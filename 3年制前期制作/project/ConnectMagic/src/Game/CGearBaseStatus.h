#pragma once

#define TEST_NAME "TEST"

// 装備のステータス基底クラス
class CGearBaseStatus
{
private:
	enum class ERare
	{
		eCommon,	// 白
		eUnCommon,	// 緑
		eRare,		// 青
		eEpic,		// 紫
		eLegendary	// 橙
	};
	// レア度s
	ERare mRare;

	// 装備名
	std::string mName;

	// ステータス
	std::map<std::string, int> mStatus;

public:
	// コンストラクタ
	CGearBaseStatus(ERare rare = ERare::eCommon, std::string name = TEST_NAME);
	// デストラクタ
	~CGearBaseStatus();

	// レア度を取得する
	ERare GetRare();
	// 装備名を取得する
	std::string GetName();
	// ステータスを取得する
	std::map<std::string, int> GetStatus();

	// TODO：例
	// ステータスを表示
	void PrintStatus()
	{
		// ステータスを取得
		std::map<std::string, int> statusList = GetStatus();
		// 全てのステータスを表示
		for (auto status : statusList)
		{
			// ステータスの数値を取得
			int statusNum = status.second;

			// 数値が0なら次へ
			if (statusNum == 0)
			{
				continue;
			}

			// 表示
			printf("%s:%d\n", status.first, statusNum);
		}
	}
};
