#include "glew.h"
#include "CWater.h"
#include "CTexture.h"
#include "CMaterial.h"

// 光の位置
#define LIGHT_POS CVector(0.0f,10.0f,0.0f)

// 回転
#define ROT CVector(180.0f,0.0f,0.0f)

// スケール
#define SCALE 10.0f

// コンストラクタ
CWater::CWater()
	: mVAO(0)
	, mVBO(0)
	, mNormalMapTex(0)
	, mTime(0.0f)
	, mpMaterial(nullptr)
{
	// 初期設定
	Init("Shader\\water.vert", "Shader\\water.flag");

	// 回転を設定
	Rotation(ROT);
	// スケールを設定
	Scale(Scale() * SCALE);
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

	float vertices[] = {
		// positions        // uvs       // normals
		-1.0f, 0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,

		-1.0f, 0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f,  1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	mpMaterial = new CMaterial();
	mpMaterial->LoadTexture("Water", "Field\\Water\\Water.png", false);
	mNormalMapTex = mpMaterial->Texture()->Id();

	return true;
}

// 更新
void CWater::Update()
{
	// 経過時間を設定
	mTime += Times::DeltaTime();
	CDebugPrint::Print("Time: % f\n", mTime);
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
		LIGHT_POS,
		camera->Position(),
		Matrix(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix()
	);

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glUseProgram(0);

	mWaterShader.Disable();
}