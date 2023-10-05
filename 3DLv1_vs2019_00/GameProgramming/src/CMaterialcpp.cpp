#include "CMaterial.h"
//memset,strncpy�̃C���N���[�h
#include <string.h>
#include "glut.h"

/*
strncpy (char* str1,const char* str2,int len)
�R�s�[��str1�ɃR�s�[��str2�̕�����len�������܂ŃR�s�[����
*/
char* strncpy(char* str1, const char* str2, int len)
{
	int i = 0;
	//i��len��菬�����A���A�R�s�[�����I���łȂ��ԌJ��Ԃ�
	while (i < len && *str2 != '\0')
	{
		*(str1 + i) = *str2;//�R�s�[��ɃR�s�[������
		str2++;//�R�s�[��������
		i++;
	}
	str1[i] = '\0';//�R�s�[��̕�����ɏI���
	return str1;//�R�s�[��̐擪�A�h���X��ԋp
}

//�f�t�H���g�R���X�g���N�^
CMaterial::CMaterial()
{
	//���O��0�Ŗ���
	memset(mName, 0, sizeof(mName));
	//0�Ŗ��߂�
	memset(mDiffuse, 0, sizeof(mDiffuse));
}

//�}�e���A����L���ɂ���
void CMaterial::Enabled()
{
	//�g�U���̐ݒ�
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
}

//�}�e���A���̖��O�̎擾
char* CMaterial::Name()
{
	return mName;
}

//�}�e���A���̖��O��ݒ肷��
//Name(�}�e���A���̖��O)
void CMaterial::Name(char* name)
{
	strncpy(mName, name, MATERIAL_NAME_LEN);
}

//mDiffuse�z��̎擾
float* CMaterial::Diffuse()
{
	return mDiffuse;
}