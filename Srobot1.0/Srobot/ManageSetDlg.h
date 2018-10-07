#pragma once

#include"SrobotDlg.h"//������SrobotDlg��ͷ�ļ�
// CManageSetDlg dialog
//---------------------opencv-------------------
#ifndef OPENCVHEAD
#define OPENCVHEAD
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#endif
//---------------------------------------------
#include "afxwin.h"


class CManageSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CManageSetDlg)

public:
	CManageSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageSetDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANAGESET_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int BaudRate[12] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
	afx_msg void OnBnClickedExitmana();
	afx_msg void OnBnClickedLenssw();
	void DrawPic(cv::Mat& img, UINT nID);//��ͷ��ʾ������
	volatile bool ShowLeFlg;//��ͷ��ʾ���߳̿��صı�־
	static UINT ShowLens(void* param);//��ͷ��ʾ���߳�
	afx_msg void OnBnClickedChoosepic();
	afx_msg void OnBnClickedTakepic();
	afx_msg void OnBnClickedUploadpic();
	static UINT UploadFace(void* param);//����¼�����߳�
	void PhizModuleIniti();//����ģ���ʼ��
	// ����Ĳ�����
	CComboBox m_BaudPhiz;
	// �����COM�ں�
	CComboBox m_ComPhiz;
	afx_msg void OnBnClickedDeteccom();
	afx_msg void OnBnClickedSwphiz();
	afx_msg void OnBnClickedSwmove();
	// �ƶ�ģ�鴮�ں�
	CComboBox m_ComMove;
	// �ƶ�ģ�鲨����
	CComboBox m_BaudMove;
	void MoveModuleIniti();//�ƶ�ģ���ʼ��
	void ArmModuleIniti();//��е�ֱ�ģ���ʼ��
	afx_msg void OnBnClickedSwarm();
	// ��е��ģ�鲨����F
	CComboBox m_BaudArmF;
	// ��е��ģ�鲨����S
	CComboBox m_BaudArmS;
	// ��е��ģ�鴮�ں�F
	CComboBox m_ComArmF;
	// ��е��ģ�鴮�ں�S
	CComboBox m_ComArmS;
	afx_msg void OnBnClickedActtorque();
};
//���ʸ�������	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();