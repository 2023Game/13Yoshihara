#include "glew.h"
#include "CWater.h"
#include "CMaterial.h"
#include "CConnectPointManager.h"
#include "CPhysicsManager.h""
#include "PhysicsMaterial.h"

// 頂点
const CVector VERT_POS_1 = CVector(-1.0f, 0.0f, -1.0f);
const CVector VERT_POS_2 = CVector(-1.0f, 0.0f,  1.0f);
const CVector VERT_POS_3 = CVector( 1.0f, 0.0f,  1.0f);
const CVector VERT_POS_4 = CVector( 1.0f, 0.0f, -1.0f);

// 物理設定
constexpr float MASS = 0.0f;
const CVector HALF_EXTENTS = CVector(1.0f, 1.0f, 1.0f);

// コンストラクタ
CWater::CWater(CVector pos, CVector scale)
	: CObjectBase(ETag::eWater,ETaskPriority::eBackground,0,ETaskPauseType::eGame)
	, mVAO(0)
	, mVBO(0)
	, mNormalMapTex(0)
	, mTime(0.0f)
	, mpMaterial(nullptr)
{
	// 初期設定
	Init("Shader\\water.vert", "Shader\\water.flag");

	Position(pos);
	// スケールを設定
	Scale(scale);

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CWater::~CWater()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
	glDeleteTextures(1, &mNormalMapTex);
}

// 初期設定
bool CWater::Init(const char* vertexPath, const char* flagPath)
{
	// シェーダー読み込み
	if (!mWaterShader.Load(vertexPath, flagPath))
	{
		printf("Failed to load water shaders\n");
		return false;
	}

	// 平面を生成
	float vertices[] = {
		// positions										// uvs       // normals
		VERT_POS_1.X(), VERT_POS_1.Y(), VERT_POS_1.Z(),		0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		VERT_POS_2.X(), VERT_POS_2.Y(), VERT_POS_2.Z(),		1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		VERT_POS_3.X(), VERT_POS_3.Y(), VERT_POS_3.Z(),		1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		VERT_POS_1.X(), VERT_POS_1.Y(), VERT_POS_1.Z(),		0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		VERT_POS_3.X(), VERT_POS_3.Y(), VERT_POS_3.Z(),		0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		VERT_POS_4.X(), VERT_POS_4.Y(), VERT_POS_4.Z(),		1.0f, 1.0f,  0.0f, 1.0f, 0.0f
	};

	// VAOを一つ生成
	glGenVertexArrays(1, &mVAO);
	// VBOを一つ生成
	glGenBuffers(1, &mVBO);

	// VAOをバインド（設定開始）
	glBindVertexArray(mVAO);
	// VBOをバインド（設定開始）
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	// データを転送
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	glVertexAttribPointer(index, size, type, normalized, stride, offset)
	index：シェーダーで使う属性の場所
	size：要素数
	stride：次の頂点までのバイト数
	offset：この属性の先頭位置
	*/

	// 位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// UV座標
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// 法線
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// バインドの解除（設定完了）
	glBindVertexArray(0);

	// マテリアルを生成
	mpMaterial = new CMaterial();
	// 水のテクスチャを読み込む
	mpMaterial->LoadTexture("Water", "Field\\Water\\Water.png", false);
	// ノーマルマップのIDを設定
	mNormalMapTex = mpMaterial->Texture()->Id();

	return true;
}

// 更新
void CWater::Update()
{
	// 経過時間を設定
	mTime += Times::DeltaTime();
}

// 描画
void CWater::Render()
{
	// シェーダー有効
	mWaterShader.Enable();

	const int normalMapTextureInit = 0;

	// 現在のカメラを取得
	CCamera* camera = CCamera::CurrentCamera();

	// ノーマルマップテクスチャをテクスチャユニット0にバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mpMaterial->Texture()->Id());

	// 各種uniformに値を設定
	mWaterShader.SetUniforms(
		mTime,
		normalMapTextureInit,
		camera->Position(),
		Matrix(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix()
	);

	// VAOをバインド
	glBindVertexArray(mVAO);
	// 三角形を描画
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// バインド解除
	glBindVertexArray(0);

	// シェーダー無効化
	glUseProgram(0);

	mWaterShader.Disable();
}

// コライダーを生成
void CWater::CreateCol()
{
	// 物理設定
	PhysicsMaterial material;
	material.mass = MASS;

	// サイズの計算
	CVector size = CVector(
		HALF_EXTENTS.X() * Scale().X(),
		HALF_EXTENTS.Y() * Scale().Y(),
		HALF_EXTENTS.Z() * Scale().Z());
	CPhysicsManager* physicsMgr = CPhysicsManager::Instance();
	// 本体コライダー
	physicsMgr->CreateBoxSensor(
		this,
		size,
		ELayer::eCrushed,
		{ ELayer::ePlayer,ELayer::eConnectObj }
	);
}
