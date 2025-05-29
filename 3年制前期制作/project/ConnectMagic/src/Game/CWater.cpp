#include "glew.h"
#include "CWater.h"
#include "CMaterial.h"
#include "CColliderRectangle.h"

// ���_
#define VERT_POS_1 -1.0f, 0.0f, -1.0f
#define VERT_POS_2 -1.0f, 0.0f,  1.0f
#define VERT_POS_3  1.0f, 0.0f,  1.0f
#define VERT_POS_4  1.0f, 0.0f, -1.0f

// �R���X�g���N�^
CWater::CWater(CVector scale)
	: mVAO(0)
	, mVBO(0)
	, mNormalMapTex(0)
	, mTime(0.0f)
	, mpMaterial(nullptr)
{
	// �����ݒ�
	Init("Shader\\water.vert", "Shader\\water.flag");

	// �R���C�_�[�𐶐�
	CreateCol();

	// �X�P�[����ݒ�
	Scale(scale);
}

// �f�X�g���N�^
CWater::~CWater()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
	glDeleteTextures(1, &mNormalMapTex);

	SAFE_DELETE(mpCol);
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

	// ���ʂ𐶐�
	float vertices[] = {
		// positions    // uvs       // normals
		VERT_POS_1,		0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		VERT_POS_2,		1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		VERT_POS_3,		1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		VERT_POS_1,		0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		VERT_POS_3,		0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		VERT_POS_4,		1.0f, 1.0f,  0.0f, 1.0f, 0.0f
	};

	// VAO�������
	glGenVertexArrays(1, &mVAO);
	// VBO�������
	glGenBuffers(1, &mVBO);

	// VAO���o�C���h�i�ݒ�J�n�j
	glBindVertexArray(mVAO);
	// VBO���o�C���h�i�ݒ�J�n�j
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	// �f�[�^��]��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	glVertexAttribPointer(index, size, type, normalized, stride, offset)
	index�F�V�F�[�_�[�Ŏg�������̏ꏊ
	size�F�v�f��
	stride�F���̒��_�܂ł̃o�C�g��
	offset�F���̑����̐擪�ʒu
	*/

	// �ʒu
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// UV���W
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// �@��
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// �o�C���h�̉����i�ݒ芮���j
	glBindVertexArray(0);

	// �}�e���A���𐶐�
	mpMaterial = new CMaterial();
	// ���̃e�N�X�`����ǂݍ���
	mpMaterial->LoadTexture("Water", "Field\\Water\\Water.png", false);
	// �m�[�}���}�b�v��ID��ݒ�
	mNormalMapTex = mpMaterial->Texture()->Id();

	return true;
}

// �X�V
void CWater::Update()
{
	// �o�ߎ��Ԃ�ݒ�
	mTime += Times::DeltaTime();
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
		camera->Position(),
		Matrix(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix()
	);

	// VAO���o�C���h
	glBindVertexArray(mVAO);
	// �O�p�`��`��
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// �o�C���h����
	glBindVertexArray(0);

	// �V�F�[�_�[������
	glUseProgram(0);

	mWaterShader.Disable();
}

// �R���C�_�[�𐶐�
void CWater::CreateCol()
{
	mpCol = new CColliderRectangle(
		this, ELayer::eWater,
		CVector(VERT_POS_1),
		CVector(VERT_POS_2),
		CVector(VERT_POS_3),
		CVector(VERT_POS_4),
		true
	);
	// �v���C���[�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::ePlayer });
}
