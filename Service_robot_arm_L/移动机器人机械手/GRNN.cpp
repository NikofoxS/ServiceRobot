#include "stdafx.h"
#include "GRNN.h"

float spread =2;	//������ϵ��
float Lalp[3] = { 0 };
float Ralp[3] = { 0 };
GRNNNetwork::GRNNNetwork() : GRNNIsOpened(false){}//���캯�����������ʼ��

GRNNNetwork::~GRNNNetwork()
{
	if (GRNNIsOpened == true)
	{
		GRNNIsOpened = false;//������ʱ�������￨ʱ��
		for (int i = 0; i<Hidden_Neurons; ++i)
		{
			delete[] Lw1[i];
			delete[] Lw2[i];
			delete[] Rw1[i];
			delete[] Rw2[i];
		}
		delete[] Lw1;
		delete[] Lw2;
		delete[] LU;
		delete[] Rw1;
		delete[] Rw2;
		delete[] RU;
	}
	else return;
}

void GRNNNetwork::Initcenter()
{   
	//Ȩֵ
	Lw1 = new float*[Hidden_Neurons];	//�ڶ���ΪȨֵ���󿪱��ڴ�ռ䣬��mfc������������֮������
	for (int i = 0; i<Hidden_Neurons; ++i)
	{
		Lw1[i] = new float[3];
	}
	Lw2 = new float*[Hidden_Neurons];
	for (int i = 0; i<Hidden_Neurons; ++i)
	{
		Lw2[i] = new float[3];
	}
	LU = new float[Hidden_Neurons];
	Rw1 = new float*[Hidden_Neurons];	//�ڶ���ΪȨֵ���󿪱��ڴ�ռ䣬��mfc������������֮������
	for (int i = 0; i<Hidden_Neurons; ++i)
	{
		Rw1[i] = new float[3];
	}
	Rw2 = new float*[Hidden_Neurons];
	for (int i = 0; i<Hidden_Neurons; ++i)
	{
		Rw2[i] = new float[3];
	}
	RU = new float[Hidden_Neurons];

	//*****************��ȡP�ļ�����������*****************/
	ifstream input1("LP.dat", ifstream::binary);
	for (int i = 0; i != Hidden_Neurons; ++i)
	{
		input1.read((char*)LInput_Buf, 3 * sizeof(float));
		for (int j = 0; j != Input_Neurons; ++j)
		{
			Lw1[i][j] = LInput_Buf[j];
		}
	}
	input1.close();
	ifstream input2("RP.dat", ifstream::binary);
	for (int i = 0; i != Hidden_Neurons; ++i)
	{
		input2.read((char*)RInput_Buf, 3 * sizeof(float));
		for (int j = 0; j != Input_Neurons; ++j)
		{
			Rw1[i][j] = RInput_Buf[j];
		}
	}
	input2.close();
	//*****************��ȡT�ļ����Ƕ�����******************/
	ifstream input3("LT.dat", ifstream::binary);
	for (int i = 0; i != Hidden_Neurons; ++i)
	{
		input3.read((char*)LOutput_Buf, 3 * sizeof(float));
		for (int j = 0; j != Output_Neurons; ++j)
		{
			Lw2[i][j] = LOutput_Buf[j];
		}
	}
	input3.close();
	ifstream input4("RT.dat", ifstream::binary);
	for (int i = 0; i != Hidden_Neurons; ++i)
	{
		input4.read((char*)ROutput_Buf, 3 * sizeof(float));
		for (int j = 0; j != Output_Neurons; ++j)
		{
			Rw2[i][j] = ROutput_Buf[j];
		}
	}
	input4.close();
	MessageBox(hWnd, _T("�����ʼ����ϣ�"), _T("�������ʼ��"), MB_ICONASTERISK);
	GRNNIsOpened = true;//�����������ֻ���������������õ�����ʱ��
}


float GRNNNetwork::Euclidean_distance(float *a, float *b)
{
	float sum = 0.0;
	for (int i = 0; i<Input_Neurons; i++)
	{
		a[i] -= b[i];//����������Ŀ�������Ĳ�
	}
	for (int i = 0; i != 3; ++i)
	{
		sum += a[i] * a[i];//ƽ���ͣ������ϻ�Ӧ�ÿ�����
	}
	return sum;
}


bool GRNNNetwork::Calculate_Left(float a, float b, float c)
{

	LInput_Buf[0] = a;
	LInput_Buf[1] = b;
	LInput_Buf[2] = c;

	//�������������
	float x[3];
	for (int i = 0; i != Hidden_Neurons; ++i)
	{
		for (int j = 0; j != 3; ++j)
		{
			x[j] = LInput_Buf[j];
		}
		LU[i] = (float)exp((0.0 - 0.8326*0.8326)*(Euclidean_distance(x, Lw1[i])) / (spread*1));
		//0<U[i]<=1����������������������ݲ��ܴ���U[i]��������0�����������������������ľ�������Χ�ڣ���ô��ʱ��U[i]=1�������Ϊ0
	}
	//������������
	float sum_U = 0.0;
	float m[3] = { 0, 0, 0};
	for (int i = 0; i<Hidden_Neurons; i++)
	{
		sum_U += LU[i];
	}
	for (int j = 0; j<Output_Neurons; j++)
	{
		for (int i = 0; i<Hidden_Neurons; i++)
		{
			m[j] += LU[i] * Lw2[i][j];//�����㵽������ʵ���������
		}
		LV[j] = m[j] / sum_U;//�����Ӧѵ���������sita
	}
	//ͨ������NaN���������������0/0��Inf/Inf��Inf-Inf��Inf*0��������������޽�Ӧ����0/0��Ե��
	Lalpha[0] = (float)(LV[0] * 180 / pi);
	Lalpha[1] = (float)(LV[1] * 180 / pi);
	Lalpha[2] = (float)(LV[2] * 180 / pi);
	Lalp[0] = Lalpha[0];
	Lalp[1] = Lalpha[1];
	Lalp[2] = Lalpha[2];

	return true;
}


bool GRNNNetwork::Calculate_Right(float a, float b, float c)
{

	RInput_Buf[0] = a;
	RInput_Buf[1] = b;
	RInput_Buf[2] = c;

	//�������������
	float x[3];
	for (int i = 0; i != Hidden_Neurons; ++i)
	{
		for (int j = 0; j != 3; ++j)
		{
			x[j] = RInput_Buf[j];
		}
		RU[i] = (float)exp((0.0 - 0.8326*0.8326)*(Euclidean_distance(x, Rw1[i])) / (spread * 1));
		//0<U[i]<=1����������������������ݲ��ܴ���U[i]��������0�����������������������ľ�������Χ�ڣ���ô��ʱ��U[i]=1�������Ϊ0
	}
	//������������
	float sum_U = 0.0;
	float m[3] = { 0, 0, 0 };
	for (int i = 0; i<Hidden_Neurons; i++)
	{
		sum_U += RU[i];
	}
	for (int j = 0; j<Output_Neurons; j++)
	{
		for (int i = 0; i<Hidden_Neurons; i++)
		{
			m[j] += RU[i] * Rw2[i][j];//�����㵽������ʵ���������
		}
		RV[j] = m[j] / sum_U;//�����Ӧѵ���������sita
	}
	//ͨ������NaN���������������0/0��Inf/Inf��Inf-Inf��Inf*0��������������޽�Ӧ����0/0��Ե��
	Ralpha[0] = (float)(RV[0] * 180 / pi);
	Ralpha[1] = (float)(RV[1] * 180 / pi);
	Ralpha[2] = (float)(RV[2] * 180 / pi);
	Ralp[0] = Ralpha[0];
	Ralp[1] = Ralpha[1];
	Ralp[2] = Ralpha[2];

	return true;
}


///////////////////////////////////////////////////����������������˵ĺ�����ʱû��
void MMMul(Matrix a, Matrix b, Matrix c, int m, int r, int s, int n)	//c=a*b
{
	int     i, j, k;
	float   sum;
	Matrix  d;
	if ((m>0) && (n>0) && (r == s))
	{
		for (i = 0; i<m; i++)
			for (j = 0; j<n; j++)
			{
				sum = 0.0;
				for (k = 0; k<s; k++)
					sum += a[i][k] * b[k][j];
				d[i][j] = sum;
			}
		for (i = 0; i<m; i++)
			for (j = 0; j<n; j++)
				c[i][j] = d[i][j];
	}
	else
	{
		AfxMessageBox(_T("multiplication is error!"));
	}
}
void MVMul(Matrix a, Vector b, Vector c, int m, int r, int n)	//c=b*a
{
	int      i, j;
	float   sum;
	if ((m>0) && (n>0) && (r == n))
	{
		for (i = 0; i<m; i++)
		{
			sum = 0.0;
			for (j = 0; j<r; j++)
				sum += a[i][j] * b[j];
			c[i] = sum;
		}
	}
	else
	{
		AfxMessageBox(_T("The multiplication of the Matrix and Vector is error!"));
	}
}
///////////////////////////////////////////////////����������������˵ĺ�����ʱû��