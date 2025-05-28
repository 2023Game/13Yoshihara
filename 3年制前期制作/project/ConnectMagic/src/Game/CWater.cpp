#include "glew.h"
#include "CWater.h"
#include "CTexture.h"
#include "CMaterial.h"

// ���̈ʒu
#define LIGHT_POS CVector(0.0f,10.0f,0.0f)

// ��]
#define ROT CVector(180.0f,0.0f,0.0f)

// �X�P�[��
#define SCALE 10.0f

// �R���X�g���N�^
CWater::CWater()
	: mVAO(0)
	, mVBO(0)
	, mNormalMapTex(0)
	, mTime(0.0f)
	, mpMaterial(nullptr)
{
	// �����ݒ�
	Init("Shader\\water.vert", "Shader\\water.flag");

	// ��]��ݒ�
	Rotation(ROT);
	// �X�P�[����ݒ�
	Scale(Scale() * SCALE);
}

// �f�X�g���N�^
CWater::~CWater()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
	glDeleteTextures(1, &mNormalMapTex);
}

// �����ݒ�
bool CWater::Init(const char* vertexPath, const char* flagPath)
{
	// �V�F�[�_�[�ǂݍ���
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

// �X�V
void CWater::Update()
{
	// �o�ߎ��Ԃ�ݒ�
	mTime += Times::DeltaTime();
	CDebugPrint::Print("Time: % f\n", mTime);
}

// �`��
void CWater::Render()
{
	// �V�F�[�_�[�L��
	mWaterShader.Enable();

	const int normalMapTextureInit = 0;

	// ���݂̃J�������擾
	CCamera* camera = CCamera::CurrentCamera();

	// �m�[�}���}�b�v�e�N�X�`�����e�N�X�`�����j�b�g0�Ƀo�C���h
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mpMaterial->Texture()->Id());

	// �e��uniform�ɒl��ݒ�
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