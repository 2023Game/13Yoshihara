#ifndef CMODEL_H
#define CMODEL_H
#include <vector>
#include "CTriangle.h"
/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel
{
public:
	//���f���t�@�C���̓���
	//Load(���f���t�@�C����,�}�e���A���t�@�C����)
	void Load(char* obj, char* mtl);
	//�`��
	void Render();

private:
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle>mTriangles;
};

#endif
