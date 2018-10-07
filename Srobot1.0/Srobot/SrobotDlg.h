
// SrobotDlg.h : ͷ�ļ�
//

#pragma once

#include"ManageSetDlg.h"//�Ӵ���ManageSetDlg��ͷ�ļ�

#include"XunFlySpecre.h"//¼����Ѷ��
#include"SerialPort.h"//����ͨ��
#include"NetCurl.h"//ͼ�������webapi����
#include"OrderSearch.h"
#include <sphelper.h>//΢��Microsoft Speech SDK ����ͷ�ļ�
//---------------------opencv-------------------
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//---------------------------------------------
#include<ctime>//ϵͳʱ��

#include "afxcmn.h"
#include "afxwin.h"

// CSrobotDlg �Ի���
class CSrobotDlg : public CDialogEx
{
// ����
public:
	CSrobotDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SROBOT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	SpecRe XunflySpe;//����ʶ����
	TulingRobot Tuling;//ͼ�������webapi��

	// ¼�������Ȼ���
	CSliderCtrl m_Sensitivity;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedEditsensi();
	// ����ʶ����
	CEdit m_SrResult;
	// ����ʶ�𿪹�
	CButton m_SrSwitch;
	afx_msg void OnBnClickedSrswitch();
	volatile bool XunflySpeRFlag;//����ʶ���̱߳�־
	volatile bool XunflySRSwitchFlg;//����ʶ��ֹͣ��־
	volatile bool SpeechSwFlg;//����������Ӧ��رհ�ť�ı�־
	volatile bool AISwFlg;//����Ӧ�𿪹ر�־
	static UINT XunflySpeR(void *param);//����ʶ���̺߳���
	char* SendToXfly();//¼����������Ѷ�ɻ��ʶ����
	std::string SendToTuling(char* mytext);//��ʶ����������ͼ������˲���ûظ�
	volatile bool StaSpeechPlay;//��������״̬���Ʊ�־
	void TextToSpeech(CString mytext);//ʹ��΢�� speech sdk ����ת����ÿ�ε��øú���Ҫ��StaSpeechPlay = true;
	afx_msg void OnBnClickedAiswitch();
	afx_msg void OnBnClickedExitall();
	volatile bool AiCloseFlg;//�ر�Ai��ť�ı�־
	static UINT AiClose(void* param);//�ر�Ai����Ӧ��ť�����濨�����߳�
	void DrawMat(cv::Mat& img, UINT nID);//����ͷ��ʾ������
	afx_msg void OnBnClickedCamerasw();//����ͷ����
	volatile bool ShowCaFlg;//����ͷ��ʾ�ı�־
	static UINT ShowCamera(void* param);//����ͷ��ʾ���߳�
	static UINT PostFacepp(void *param);//post����face++�����߳�
	volatile bool PostFaceFlg;//post��face++���߳��Ƿ������־
	afx_msg void OnBnClickedAdvacedset();
	CSerialPort PhizModule;//����ģ�鴮��
	volatile bool PhizFlg;//����ģ�鴮�ڴ򿪵ı�־
	volatile int PhizInf[2];//����ģ�鴮����Ϣ��¼
	CSerialPort MoveModule;//��ʽ�ƶ�ģ�鴮��
	volatile bool MoveFlg;//��ʽ�ƶ�ģ�鴮�ڴ򿪵ı�־
	volatile int MoveInf[2];//��ʽ�ƶ�ģ�鴮����Ϣ��¼
	CSerialPort ArmModuleF;//��е�ֱ�ģ�鴮��First
	CSerialPort ArmModuleS;//��е�ֱ�ģ�鴮��Second
	volatile int Gymnastics;//�ֱ۶�����ʱ���̼߳���
	volatile bool ArmFlg;//��е�ֱ�ģ��򿪵ı�־
	volatile int ArmInf[4];//��е�ֱ�ģ�鴮����Ϣ��¼
	afx_msg void OnBnClickedActionsw();//��������
	void OrderAction(const char* myorder);//����ת����
	volatile bool SpeechPlayFlg;//ͼ�����������߳̿��Ʊ�־
	CString SpeechWord;//�����������������
	static UINT SpeechPlay(void *param);//���������̺߳���
	afx_msg void OnBnClickedRelasetorque();
	afx_msg void OnBnClickedActionres();
	afx_msg void OnBnClickedGymsw();
	afx_msg void OnBnClickedPiaffesw();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStopall();
	afx_msg void OnBnClickedHugarm();
	afx_msg void OnBnClickedInvitearm();
};
