
// �ƶ������˻�е��Dlg.h : ͷ�ļ�
//
#pragma once

#include "GRNN.h"
#include "float.h"
#include "Public.h"
#include "afxwin.h"

// C�ƶ������˻�е��Dlg �Ի���
class CnlDlg : public CDialogEx
{
// ����
public:
	CnlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSyscommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_vec;
	CButton m_1;
	CButton m_2;
	CButton m_3;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedSetcom();
	afx_msg void OnBnClickedTorque(UINT nID);
	afx_msg void OnBnClickedMonitor();
	int m_speed;
	static UINT Thread1(void *param);
	bool ThreadFlag;
	afx_msg void OnBnClickedSinglemove();
	CButton m_4;
	CButton m_5;
	CButton m_6;
	CButton m_7;
	CButton m_8;
	CButton m_9;
	afx_msg void OnBnClickedGym();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool TimerFlag;
	afx_msg void OnBnClickedButton4();
};
