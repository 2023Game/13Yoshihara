#ifndef CMATRIX_H
#define CMATRIX_H
/*
�}�g���N�X�N���X
4�s4��̍s��f�[�^�������܂�
*/
class CMatrix
{
public:
	//�t�s��擾
	CMatrix Transpose() const;
	//�s��̎擾
	float* M() const;
	//*���Z�q�̃I�[�o�[���[�h
	//CMatrix * CMatrix�̉��Z���ʂ�Ԃ�
	const CMatrix operator*(const CMatrix& m)const;
	//�s��l�̑��
	//M(�s��,��,�l)
	void M(int row, int col, float value);
	//�ړ��s��̍쐬
	//Translate(�ړ���X,�ړ���Y,�ړ���Z)
	CMatrix Translate(float mx, float my, float mz);
	//��]�s��(X��)�̍쐬
	//RotateX(�p�x)
	CMatrix RotateX(float degree);
	//��]�s��(Z��)�̍쐬
	//RotateZ(�p�x)
	CMatrix RotateZ(float degree);
	//��]�s��(Y��)�̍쐬
	//RotateY(�p�x)
	CMatrix RotateY(float degree);
	//�s��l�̎擾
	//M(�s,��)
	//mM[�s][��]���擾
	float M(int r, int c)const;
	//�g��k���s��̍쐬
	//Scale(�{��X,�{��Y,�{��Z)
	CMatrix Scale(float sx, float sy, float sz);
	//�\���m�F�p
	//4�~4�̍s�����ʏo��
	void Print();
	//�f�t�H���g�R���X�g���N�^
	CMatrix();
	//�P�ʍs��̍쐬
	CMatrix Identity();
private:
	//4�~4�̍s��f�[�^��ݒ�
	float mM[4][4];
};
#endif
