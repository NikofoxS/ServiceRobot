#pragma once

#include "time.h"
#include <cmath>
#include <fstream>
#include <float.h>

using namespace std;

#define pi 3.1415926
#define Input_Neurons      3	//�������Ԫ��������ֵ��������������ά��
#define Hidden_Neurons  1030301	//��������Ԫ��������ֵ�������������ĸ���
#define Output_Neurons     3	//�������Ԫ��������ֵ�������������ά��
typedef float Matrix[4][4];
typedef float Vector[4];

class GRNNNetwork
{
private:

	HWND hWnd;					//ר�ñ�ʾ���ڵľ��
	//float **w1;
	//float **w2;
	//float *U;
	//float V[3];					//�����ĳһ�ڵ����
	//float Input_Buf[3];			//��������������
	//float Output_Buf[3];		//�������������
	//float alpha[3];				//��Ҫ����Ĺؽڱ���
	//float distance;				//����
	////////////////////////////////////////////////////
	float **Lw1;
	float **Lw2;
	float *LU;
	float LV[3];					//�����ĳһ�ڵ����
	float LInput_Buf[3];			//��������������
	float LOutput_Buf[3];			//�������������
	float Lalpha[3];				//��Ҫ����Ĺؽڱ���
	////////////////////////////////////////////////////
	float **Rw1;
	float **Rw2;
	float *RU;
	float RV[3];					//�����ĳһ�ڵ����
	float RInput_Buf[3];			//��������������
	float ROutput_Buf[3];			//�������������
	float Ralpha[3];				//��Ҫ����Ĺؽڱ���
	bool GRNNIsOpened;				//�����ʼ����־λ
	
public:
	 GRNNNetwork();//�������캯��
	~GRNNNetwork();//������������
	
	void Initcenter();//�����ʼ��
	bool GRNNNetworkStart();//��������
	bool Calculate_Left(float a, float b, float c);
	bool Calculate_Right(float a, float b, float c);
	float Euclidean_distance(float *a, float *b);//������������Ȩֵ�����ŷ�Ͼ���
	//BP�����纯����Ŀ����У��Ŀ�궨λϵͳ�����
	//void mapminmaxIn(float* t);//����������һ��
	//void unmapminmaxOut(float* t);//�����������һ��
	//void correction(float* p);

protected:
	//CWinThread* pGRNNThread;
	//static UINT GRNNThreadFunc(LPVOID pParam);//�̺߳�������Ϊ��ĳ�ԱҪ����static
};

void MMMul(Matrix a, Matrix b, Matrix c, int m, int r, int s, int n);//�������
void MVMul(Matrix a, Vector b, Vector c, int m, int r, int n);//�����������
