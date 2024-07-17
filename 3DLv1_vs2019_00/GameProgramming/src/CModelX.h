#ifndef CMODELX_H //�C���N���[�h�K�[�h
#define CMODELX_H

#include <vector>    //vector�N���X�̃C���N���[�h�i���I�z��j
#include "CMatrix.h" //�}�g���N�X�N���X�̃C���N���[�h
#include "CVector.h" //Vector�N���X�̃C���N���[�h
class CModelX;      //CModelX�N���X�̐錾
class CModelXFrame; //CModelXFrame�N���X�̐錾
class CMesh;        //CMesh�N���X�̐錾
class CMaterial;    //CMaterial�N���X�̐錾
class CSkinWeights; //CSkinWeights�N���X�̐錾
class CAnimationSet;//CAnimationSet�N���X�̐錾
class CAnimation;   //CAnimation�N���X�̐錾
class CAnimationKey;//CAnimationKey�N���X�̐錾

#define MODEL_FILE "res\\���O�i.x" //���̓t�@�C����
#define KNIGHT_MODEL "res\\knight\\knight_low.X"

//�̈������}�N����
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a; a = nullptr;}
//�z��̃T�C�Y�擾���}�N����
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))
/*
CModelX
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX {
	friend CModelXFrame;
	friend CAnimationSet;
	friend CAnimation;
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
	//�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
	std::vector<CAnimationSet*>& AnimationSet();
	//mFrame�̎Q�Ƃ�Ԃ�
	std::vector<CModelXFrame*>& Frames();
	CModelXFrame* FindFrame(char* name);
	//�t���[���̕ϊ��s����X�V����
	void AnimateFrame();
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	void SetSkinWeightFrameIndex();
	//���_�ɃA�j���[�V������K�p
	void AnimateVertex();
	//�w�肵�������s����g���Čv�Z
	void AnimateVertex(CMatrix*);
	//�}�e���A���̌���
	CMaterial* FindMaterial(char* name);
	//�}�e���A���z��̎擾
	std::vector<CMaterial*>& Material();
	/// <summary>
	/// �A�j���[�V�����𔲂��o��
	/// </summary>
	/// <param name="idx">�����������A�j���[�V�����Z�b�g�̔ԍ�</param>
	/// <param name="start">�����������A�j���[�V�����̊J�n����</param>
	/// <param name="end">�����������A�j���[�V�����̏I������</param>
	/// <param name="name">�ǉ�����A�j���[�V�����Z�b�g�̖��O</param>
	void SeparateAnimationSet(int idx, int start, int end, char* name);
	//�ǂݍ��ݍς݂�����
	bool IsLoaded();
	//�A�j���[�V�����Z�b�g�̒ǉ�
	void AddAnimationSet(const char* file);
private:
	std::vector<CModelXFrame*> mFrame;//�t���[���̔z��
	std::vector<CAnimationSet*> mAnimationSet;//�A�j���[�V�����Z�b�g�̔z��
	std::vector<CMaterial*> mMaterial;//�}�e���A���z��
	//c����؂蕶���Ȃ�true��Ԃ�
	bool IsDelimiter(char c);
	char* mpPointer;   //�ǂݍ��݈ʒu
	char mToken[1024]; //���o�����P��̗̈�
	bool mLoaded;	//�ǂݍ��ݍς݂��ǂ���
};

//Frame�N���X
class CModelXFrame {
	friend CModelX;
	friend CAnimationSet;
	friend CAnimation;
	friend CAnimationKey;
public:
	CModelXFrame();
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame();
	//�`��
	void Render();
	//mIndex��Ԃ�
	int Index();
	//�����s��̍쐬
	void AnimateCombined(CMatrix* parent);
	//mCombinedMatrix��Ԃ�
	const CMatrix& CombinedMatrix();
private:
	std::vector<CModelXFrame*> mChild;//�q�t���[���̔z��
	CMatrix mTransformMatrix;//�ϊ��s��
	char* mpName;//�t���[�����O
	int mIndex; //�t���[���ԍ�
	CMesh* mpMesh; //Mesh�f�[�^
	CMatrix mCombinedMatrix;//�����s��
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
	//�X�L���E�F�C�g�Ƀt���[���ԍ���ݒ肷��
	void SetSkinWeightFrameIndex(CModelX* model);
	//���_�ɃA�j���[�V�����K�p
	void AnimateVertex(CModelX* model);
	//�w�肵�������s����g���Čv�Z
	void AnimateVertex(CMatrix*);
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
	std::vector<CSkinWeights*> mSkinWeights;//�X�L���E�F�C�g
	CVector* mpAnimateVertex;//�A�j���[�V�����p���_
	CVector* mpAnimateNormal;//�A�j���[�V�����p�@��
	//�e�N�X�`�����W�f�[�^
	float* mpTextureCoords;
};

//�X�L���E�F�C�g�N���X
class CSkinWeights {
	friend CModelX;
	friend CMesh;
public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
private:
	char* mpFrameName;//�t���[����
	int mFrameIndex; //�t���[���ԍ�
	int mIndexNum;//���_�ԍ���
	int* mpIndex; //���_�ԍ��z��
	float* mpWeight;//���_�E�F�C�g�z��
	CMatrix mOffset;//�I�t�Z�b�g�}�g���b�N�X
};

//�A�j���[�V�����Z�b�g
class CAnimationSet {
	friend CModelX;
public:
	CAnimationSet();
	CAnimationSet(CModelX* model);
	~CAnimationSet();
	void Time(float time);//���Ԃ̐ݒ�
	void Weight(float weight);//�d�݂̐ݒ�
	void AnimateMatrix(CModelX* model);
	std::vector<CAnimation*>& Animation();
	//mTime��Ԃ�
	float Time();
	//mMaxTime��Ԃ�
	float MaxTime();
private:
	//�A�j���[�V�����Z�b�g��
	char* mpName;
	//�A�j���[�V����
	std::vector<CAnimation*>mAnimation;
	float mTime; //���ݎ���
	float mWeight; //�d��
	float mMaxTime; //�ő厞��
};

//�A�j���[�V�����N���X
class CAnimation {
	friend CModelX;
	friend CAnimationSet;
public:
	CAnimation();
	CAnimation(CModelX* model);
	~CAnimation();
private:
	char* mpFrameName;//�t���[����
	int mFrameIndex;//�t���[���ԍ�
	int mKeyNum; //�L�[���i���Ԑ��j
	CAnimationKey* mpKey;//�L�[�̔z��
};

//�A�j���[�V�����L�[�N���X
class CAnimationKey {
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
private:
	//����
	float mTime;
	//�s��
	CMatrix mMatrix;
};
#endif

