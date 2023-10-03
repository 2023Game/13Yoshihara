#include "CModel.h"
//�W�����o�͂̃C���N���[�h
#include <stdio.h>
#include "CVector.h"
//�`��
void CModel::Render()
{
	//�ϒ��z��̗v�f�������J��Ԃ�
	for (int i = 0; i < mTriangles.size(); i++)
	{
		//�ϒ��z��ɓY�����ŃA�N�Z�X����
		mTriangles[i].Render();
	}
}

//������s1�ƕ�����s2�̔�r
//s1��s2�����������0��
// �������Ȃ����0�ȊO��Ԃ�
int strcmp(const char* s1, const char* s2)
{
	int i = 0;
	//�����������Ԃ͌J��Ԃ�
	//�ǂ��炩�̕������I���ɂȂ�ƃ��[�v�̏I���
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
	}
	//�����Ȃ������0
	return s1[i] - s2[i];
}

//���f���t�@�C���̓���
//Load(���f���t�@�C����,�}�e���A���t�@�C����)
void CModel::Load(char* obj, char* mtl)
{
	//�t�@�C���|�C���^�ϐ��̍쐬
	FILE* fp;
	//�t�@�C������f�[�^�����
	//���̓G���A���쐬����
	char buf[ 256 ];
	//���_�f�[�^�̕ۑ�(CVector�^)
	std::vector<CVector>vertex;
	//�@���f�[�^�̕ۑ�
	std::vector<CVector>normal;

	//�t�@�C���̃I�[�v��
	//fopen(�t�@�C����,���[�h)
	//�I�[�v���ł��Ȃ��Ƃ���NULL��Ԃ�
	fp = fopen(mtl, "r");
	//�t�@�C���I�[�v���G���[�̔���
	//fp��NULL�̎��̓G���[
	if (fp == NULL)
	{
		//�R���\�[���ɃG���[�o�͂��Ė߂�
		printf("%s file open error\n", mtl);
		return;
	}
	//�t�@�C������1�s����
	//fgets(���̓G���A,�G���A�T�C�Y,�t�@�C���|�C���^)
	//�t�@�C���̍Ō�ɂȂ��NULL��Ԃ�
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		//���͂����l���R���\�[���ɏo�͂���
		printf("%s", buf);
	}
	//�t�@�C���̃N���[�Y
	fclose(fp);
	
	//�t�@�C���̃I�[�v��
	//fopen(�t�@�C����,���[�h)
	//�I�[�v���ł��Ȃ��Ƃ���NULL��Ԃ�
	fp = fopen(obj, "r"); 
	//�t�@�C���I�[�v���G���[�̔���
	//fp��NULL�̎��̓G���[
	if (fp == NULL)
	{
		//�R���\�[���ɃG���[�o�͂��Ė߂�
		printf("%s file open error\n", obj);
		return;
	}
	//�t�@�C������1�s����
	//fgets(���̓G���A,�G���A�T�C�Y,�t�@�C���|�C���^)
	//�t�@�C���̍Ō�ɂȂ��NULL��Ԃ�
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		//�f�[�^�𕪊�����
		char str[4][64] = { "","","","" };
		//�����񂩂�f�[�^��4�ϐ��֑������
		//sscanf(������,�ϊ��w��q,�ϐ�)
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//������̔�r
		//strcmp(������1,������2)
		//������1�ƕ�����2�������Ƃ�0�A�قȂ�Ƃ�0�ȊO��Ԃ�
		//�擪��v�̎��A���_��vertex�ɒǉ�����
		if (strcmp(str[0], "v") == 0)
		{
			//�ϒ��z��vertex�ɒǉ�
			//atof(������)�@�����񂩂�float�^�̒l��Ԃ�
			vertex.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		//�擪��vn�̎��A���_��nomal�ɒǉ�����
		else if (strcmp(str[0], "vn") == 0)
		{
			//�ϒ��z��normal�ɒǉ�
			normal.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));

		}
		//�擪��f�̎��A�O�p�`���쐬���Ēǉ�����
		else if (strcmp(str[0], "f") == 0)
		{
			//���_�Ɩ@���̔ԍ��쐬
			int v[3], n[3];
			//���_�Ɩ@���̔ԍ��擾
			sscanf(str[1], "%d//%d", &v[0], &n[0]);
			sscanf(str[2], "%d//%d", &v[1], &n[1]);
			sscanf(str[3], "%d//%d", &v[2], &n[2]);
			//�O�p�`�쐬
			CTriangle t;
			t.Vertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
			t.Normal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
			//�ϒ��z��mTriangles�ɎO�p�`��ǉ�
			mTriangles.push_back(t);
		}
	}
	//�t�@�C���̃N���[�Y
	fclose(fp);
}