#include"XunFlySpecre.h"
#include<iostream>
SpecRe test;
void RecSpeR();
char* result;
int main()
{
	std::string feilname = "vioce.wav";
	test.WriteFeil(feilname);//�趨.wav�ļ���������·��
	test.VDA_Initialize();//¼����ʼ��
	test.XflySR_Initialize();//Ѷ��ʶ���ʼ�� 
	test.sensitivity = 21;//���ö˵��������ȣ�Ĭ��Ϊ21
	test.vda_count = 0;//��������Ͼ������ʶ��Ĺ���������

	while (1)
	{
		std::cout << "�뽲����\n";
		RecSpeR();
		std::cout << result << std::endl;
		strcpy(result, "");
	}

	test.AutoRecord_Close();//�ر�¼��
	test.XflySR_Close();//�ر�Ѷ��ʶ��

	std::cin.get();

}
void RecSpeR()
{ 
	test.Detection();//��ʼ¼�����Զ����ݶ˵���ץȡ
	test.WriteToWav();//����Ϊ.wav�ļ�
	test.AutoRecord_Reset();//������λ
	test.RecoSpere();//�������ϴ���Ѷ��
	result = test.GetResult();//��ȡʶ����

}