
#ifndef CVECTOR_H
#define CVECTOR_H
#include "CMatrix.h"

/*
�x�N�g���N���X
�x�N�g���f�[�^�������܂�
*/
class CVector 
{
public:
	//���K��
	//�傫��1�̃x�N�g����Ԃ�
	CVector Normalize() const;
	//�O��
	//Cross(�x�N�g��)
	CVector Cross(const CVector& v)const;
	//*���Z�q�̃I�[�o�[���[�h
	//CVector * float�̉��Z���ʂ�Ԃ�
	CVector operator*(const float& f)const;
	//����
	//Dot(�x�N�g��)
	float Dot(const CVector& v)const;
	//�x�N�g���̒�����Ԃ�
	float Length() const;
	//CVector*CMatrix�̌��ʂ�CVector�ŕԂ�
	CVector operator*(const CMatrix& m);
	//-���Z�q�̃I�[�o�[���[�h
	//CVector - CVector�̉��Z���ʂ�Ԃ�
	CVector operator-(const CVector& v)const;
	//+���Z�q�̃I�[�o�[���[�h
	//CVector + CVector�̉��Z���ʂ�Ԃ�
	CVector operator+(const CVector& v)const;
	//+=���Z�q�̃I�[�o�[���[�h
	//CVector1+=CVector2�̉��Z���s��
	void operator +=(const CVector& v);
	//�f�t�H���g�R���X�g���N�^
	CVector();
	//�R���X�g���N�^
	//CVector(X���W,Y���W,Z���W)
	CVector(float x, float y, float z);
	//�e���ł̒l�̐ݒ�
	//Set(X���W, Y���W, Z���W)
	void Set(float x, float y, float z);
	//X�̒l�𓾂�
	float X() const;
	//Y�̒l�𓾂�
	float Y() const;
	//Z�̒l�𓾂�
	float Z() const;
	
	void X(float f);
	void Y(float f);
	void Z(float f);

	//�_�Ɛ��̍ŒZ���������߂�
	float CalcPointLineDist(const CVector& p, const CVector& s, const CVector& e, CVector* mp, float* t);
	//2���Ԃ̍ŒZ������Ԃ�
	float CalcLineLineDist(
		const CVector& s1,//�n�_1
		const CVector& e1,//�I�_1
		const CVector& s2,//�n�_2
		const CVector& e2,//�I�_2
		CVector* mp1,//��_1
		CVector* mp2,//��_2
		float* t1,//�䗦1
		float* t2//�䗦2
	);
	//0~1�̊ԂɃN�����v(�l�������I�ɂ���͈͓��ɂ��邱��)
	void clamp0to1(float& v);
	//2�����Ԃ̍ŒZ����
	float CalcSegmentSegmentDist(
		const CVector& s1, const CVector& e1,//����1
		const CVector& s2, const CVector& e2,//����2
		CVector* mp1,//�ŒZ���̒[�_1�i�n�_��I�_�ɂȂ邱�Ƃ�����j
		CVector* mp2//�ŒZ���̒[�_2�i�x�X��I�_�ɂȂ邱�Ƃ�����j
	);
private:
	//3D�e���ł̒l��ݒ�
	float mX, mY, mZ;
};
#endif