#pragma once
//ֻ��Public.cpp��nlDlg.h������Public.h����Ҫ��һЩ���ڵ�����

class CPublic
{
public:
	CPublic();
	~CPublic();
	/*��Ա����*/
	HANDLE        m_hcom1,m_hcom2;//һ��ͨ�þ��
	OVERLAPPED    m_rdos1,m_rdos2;//��һ���ṹ�����
	OVERLAPPED    m_wtos1,m_wtos2;
	bool PortIsOpened;
	/*��Ա������������*/
	bool InitPort_1();//���ڳ�ʼ��
	bool InitPort_2();
	bool WriteToPort_1(unsigned char*, int length);//��������
	bool WriteToPort_2(unsigned char*, int length);
	bool ReceiveFromPort_1(unsigned char* buffer, int BytesToRead);//��������
	bool ReceiveFromPort_2(unsigned char* buffer, int BytesToRead);
	void Clearcom();
};
