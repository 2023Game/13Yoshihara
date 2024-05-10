#ifndef CMODELX_H //�C���N���[�h�K�[�h
#define CMODELX_H

#include <vector>    //vector�N���X�̃C���N���[�h�i���I�z��j
#include "CMatrix.h" //�}�g���N�X�N���X�̃C���N���[�h
#include "CVector.h" //Vector�N���X�̃C���N���[�h
class CModelX;      //CModelX�N���X�̐錾
class CModelXFrame; //CModelXFrame�N���X�̐錾
class CMesh;        //CMesh�N���X�̐錾
class CMaterial;    //CMaterial�N���X�̐錾

#define MODEL_FILE "res\\sample.blend.x"

//�̈������}�N����
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a; a = nullptr;}
/*
CModelX
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX {
	friend CModelXFrame;
public:
	CModelX();
	~CModelX();
	void Render();
	//�m�[�h�̓ǂݔ�΂�
	void SkipNode();
	//�P��̎��o��
	char* GetToken();
	//�t�@�C���ǂݍ���
	void Load(char* file);
	//mToken�̃|�C���^��Ԃ�
	char* Token();
	//�g�[�N���������Ȃ�����true
	bool EOT();
private:
	std::vector<CModelXFrame*> mFrame;//�t���[���̔z��
	//c����؂蕶���Ȃ�true��Ԃ�
	bool IsDelimiter(char c);
	char* mpPointer;   //�ǂݍ��݈ʒu
	char mToken[1024]; //���o�����P��̗̈�
};

//Frame�N���X
class CModelXFrame {
	friend CModelX;
public:
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame();
	//�`��
	void Render();
private:
	std::vector<CModelXFrame*> mChild;//�q�t���[���̔z��
	CMatrix mTransformMatrix;//�ϊ��s��
	char* mpName;//�t���[�����O
	int mIndex; //�t���[���ԍ�
	CMesh* mpMesh; //Mesh�f�[�^
};

//CMesh�N���X�̒�`
class CMesh {
public:
	//�R���X�g���N�^
	CMesh();
	//�f�X�g���N�^
	~CMesh();
	//�`��
	void Render();
	//�ǂݍ��ݏ���
	void Init(CModelX* model);
private:
	int mVertexNum;   //���_��
	CVector* mpVertex;//���_�f�[�^
	int mFaceNum; //�ʐ�
	int* mpVertexIndex; //�ʂ��\�����钸�_�C���f�b�N�X
	int mNormalNum; //�@����
	CVector* mpNormal; //�@���x�N�g��
	int mMaterialNum;//�}�e���A����
	int mMaterialIndexNum;//�}�e���A���ԍ����i�ʐ��j
	int* mpMaterialIndex;//�}�e���A���ԍ�
	std::vector<CMaterial*> mMaterial;//�}�e���A���f�[�^
};

#endif

