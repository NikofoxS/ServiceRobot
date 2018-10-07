
// SrobotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Srobot.h"
#include "SrobotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSrobotDlg �Ի���



CSrobotDlg::CSrobotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SROBOT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSrobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Sensitivity, m_Sensitivity);
	DDX_Control(pDX, IDC_SrResult, m_SrResult);
	DDX_Control(pDX, IDC_SrSwitch, m_SrSwitch);
}

BEGIN_MESSAGE_MAP(CSrobotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_EditSensi, &CSrobotDlg::OnBnClickedEditsensi)
	ON_BN_CLICKED(IDC_SrSwitch, &CSrobotDlg::OnBnClickedSrswitch)
	ON_BN_CLICKED(IDC_AISwitch, &CSrobotDlg::OnBnClickedAiswitch)
	ON_BN_CLICKED(IDC_ExitAll, &CSrobotDlg::OnBnClickedExitall)
	ON_BN_CLICKED(IDC_CameraSw, &CSrobotDlg::OnBnClickedCamerasw)
	ON_BN_CLICKED(IDC_AdvancedSet, &CSrobotDlg::OnBnClickedAdvacedset)
	ON_BN_CLICKED(IDC_ActionSw, &CSrobotDlg::OnBnClickedActionsw)
	ON_BN_CLICKED(IDC_RelaseTorque, &CSrobotDlg::OnBnClickedRelasetorque)
	ON_BN_CLICKED(IDC_ActionRes, &CSrobotDlg::OnBnClickedActionres)
	ON_BN_CLICKED(IDC_GymSw, &CSrobotDlg::OnBnClickedGymsw)
	ON_BN_CLICKED(IDC_PiaffeSw, &CSrobotDlg::OnBnClickedPiaffesw)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_StopAll, &CSrobotDlg::OnBnClickedStopall)
	ON_BN_CLICKED(IDC_HugArm, &CSrobotDlg::OnBnClickedHugarm)
	ON_BN_CLICKED(IDC_InviteArm, &CSrobotDlg::OnBnClickedInvitearm)
END_MESSAGE_MAP()


// CSrobotDlg ��Ϣ�������

BOOL CSrobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	XunflySRSwitchFlg = false;//����ʶ�𿪹�ֹͣ��־��ʼ��
	m_Sensitivity.SetRange(1, 100, FALSE);//���û���ķ�Χ
	m_Sensitivity.SetPos(21);//���û���ĳ�ʼֵ
	GetDlgItem(IDC_SensiSta)->SetWindowTextW(_T("21"));//�������ʼֵ��ʾ��Ϊ21
	GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T(""));//״̬����ʼֵ��ʾ��Ϊ��
	//XunflySpe.sensitivity = m_Sensitivity.GetPos();//����¼�����������
	m_SrSwitch.SetWindowTextW(_T("������ʶ��"));
	GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("������Ӧ��"));
	XunflySpeRFlag = false;//����ʶ�����̱߳�־�ر�
	GetDlgItem(IDC_AISwitch)->EnableWindow(false);
	GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("������ͷ"));
	GetDlgItem(IDC_EditSensi)->SetWindowTextW(_T("����¼��������"));
	GetDlgItem(IDC_ExitAll)->SetWindowTextW(_T("�˳�����"));
	GetDlgItem(IDC_AdvancedSet)->SetWindowTextW(_T("�߼�����"));
	GetDlgItem(IDC_ActionSw)->SetWindowTextW(_T("����ʾ����"));
	GetDlgItem(IDC_GymSw)->SetWindowTextW(_T("��ʼ�㲥���"));
	GetDlgItem(IDC_PiaffeSw)->SetWindowTextW(_T("��ʼԭ��̤��"));
	GetDlgItem(IDC_ActionRes)->SetWindowTextW(_T("������λ"));
	GetDlgItem(IDC_RelaseTorque)->SetWindowTextW(_T("����Ť��"));
	GetDlgItem(IDC_StopAll)->SetWindowTextW(_T("ֹͣȫ������"));
	GetDlgItem(IDC_HugArm)->SetWindowTextW(_T("ӵ��"));
	GetDlgItem(IDC_InviteArm)->SetWindowTextW(_T("��������"));
	ShowCaFlg = false;//����ͷ���ر�־��λ
	PostFaceFlg = true;//post����face++���߳��Ƿ������־
	SpeechSwFlg = false;//����������Ӧ��رհ�ť�ı�־
	AISwFlg = true;//����Ӧ�𿪹ر�־
	AiCloseFlg = true;//�ر�Ai��ť�ı�־
	PhizInf[0] = NULL, PhizInf[1] = NULL;
	MoveInf[0] = NULL, MoveInf[1] = NULL;
	ArmInf[0] = NULL, ArmInf[1] = NULL, ArmInf[2] = NULL, ArmInf[3] = NULL;
	PhizFlg = false;//����ģ�鴮�ڴ򿪵ı�־
	MoveFlg = false;//�ƶ�ģ��򿪵ı�־
	ArmFlg = false;//��е�ֱ�ģ��򿪵ı�־
	SpeechWord = _T("");
	SpeechPlayFlg = false;//����������־
	StaSpeechPlay = false;//��������״̬��־
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSrobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSrobotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSrobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��������Ϣ��Ӧ����
void CSrobotDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	//CSliderCtrl *pSlidCtrl= (CSliderCtrl*)GetDlgItem(IDC_Sensitivity);
	int value;
	value = m_Sensitivity.GetPos();//��ȡ��������ֵ
	CString tem;
	tem.Format(_T("%d"), value);
	GetDlgItem(IDC_SensiSta)->SetWindowTextW(tem);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSrobotDlg::OnBnClickedEditsensi()
{
	// TODO: Add your control notification handler code here
	//CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_Sensitivity);
	int value;
	value = m_Sensitivity.GetPos();//��ȡ��������ֵ
	XunflySpe.sensitivity = value;
	XunflySpe.vda_count = 0;
	GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("¼��������������"));
}


void CSrobotDlg::OnBnClickedSrswitch()
{
	// TODO: Add your control notification handler code here
	int voicedevice=waveInGetNumDevs();//¼���豸������
	if (!voicedevice)
	{
		AfxMessageBox(L"δ��⵽¼���豸");
		return;
	}
	CString temp;
	/*UpdateData(true);*/
	m_SrSwitch.GetWindowTextW(temp);
	if (temp == "������ʶ��")
	{
		m_SrSwitch.EnableWindow(false);
		m_SrSwitch.SetWindowTextW(_T("���ڴ�����"));
		XunflySRSwitchFlg = false;//����ʶ�𿪹�ֹͣ��־
		XunflySpe.WriteFeil("myspeech.wav");//�趨.wav�ļ���������·��
		XunflySpe.VDA_Initialize();//¼����ʼ��
		XunflySpe.XflySR_Initialize();//Ѷ��ʶ���ʼ�� 
		XunflySpe.sensitivity = m_Sensitivity.GetPos();//����¼�����������
		XunflySpe.vda_count = 0;
		//��һ�����߳̽�������ʶ�����
		XunflySpeRFlag = true;
		AfxBeginThread(XunflySpeR, this, THREAD_PRIORITY_HIGHEST);
		m_SrSwitch.SetWindowTextW(_T("�ر�����ʶ��"));
		m_SrSwitch.EnableWindow(true);
		GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("����ʶ���Ѵ�"));
		GetDlgItem(IDC_AISwitch)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_SrSwitch)->EnableWindow(false);
		GetDlgItem(IDC_SrSwitch)->SetWindowTextW(_T("���ڹر�ʶ��"));
		XunflySRSwitchFlg = true;//����ʶ�𿪹�ֹͣ��־
		XunflySpe.stop = true;//¼���ص�����ֹͣ
		XunflySpeRFlag = false;//�رս�������ʶ����������߳�
		//XunflySpe.AutoRecord_Close();//�ر�¼��
		//m_SrSwitch.SetWindowTextW(_T("������ʶ��"));
		GetDlgItem(IDC_AISwitch)->GetWindowTextW(temp);
		if (temp == "�ر�����Ӧ��")
		{
			OnBnClickedAiswitch();//�ر�����Ӧ��
		}
	}

}

UINT CSrobotDlg::XunflySpeR(void *param)
{
	CSrobotDlg* dlg = (CSrobotDlg*)param;
	dlg->SpeechSwFlg = false;
	/*CEdit* SrResult = (CEdit*)dlg->GetDlgItem(IDC_SrResult);*/
	CString SResult;
	CString TulingRe;
	CString TemAISwitchFlg;
	CString temp;
	char* SpeResult;
	int lastLine;
	while (dlg->XunflySpeRFlag)
	{
		dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("��ʼ¼��"));
		SpeResult = dlg->SendToXfly();//��ȡʶ����
		if (0 == strcmp(SpeResult,"��Ǹ����û˵�����"))
			continue;
		dlg->GetDlgItem(IDC_ActionSw)->GetWindowTextW(temp);
		if (temp == "�ر���ʾ����")
			dlg->OrderAction(SpeResult);
		SResult = _T("�ң�");
		SResult += SpeResult;
		SResult += _T("\r\n");
		lastLine = dlg->m_SrResult.LineIndex(dlg->m_SrResult.GetLineCount() - 1);
		dlg->m_SrResult.SetSel(lastLine + 1, lastLine + 2, 1);
		dlg->m_SrResult.ReplaceSel(SResult);//�����һ�м��µ�����ʶ����
		dlg->m_SrResult.UpdateWindow();//������ʾ�����������ʱ�������ӳ�

		dlg->GetDlgItem(IDC_AISwitch)->GetWindowTextW(TemAISwitchFlg);
		if (TemAISwitchFlg == "�ر�����Ӧ��")
		{
			dlg->AISwFlg = false;
			if (0 == strcmp(SpeResult, "��Ǹ����û˵�����"))
				break;
			dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("��������ظ�"));
			TulingRe = dlg->SendToTuling(SpeResult).c_str();	//��ȡͼ�������Ӧ����
			while (!(dlg->PostFaceFlg)) Sleep(200);//�ȴ�����ʶ��������������
			lastLine = dlg->m_SrResult.LineIndex(dlg->m_SrResult.GetLineCount() - 1);
			dlg->m_SrResult.SetSel(lastLine + 1, lastLine + 2, 1);
			dlg->m_SrResult.ReplaceSel(L"AI��" + TulingRe + "\r\n");//�����һ�м��µ�ͼ������˷��ؽ��
			dlg->m_SrResult.UpdateWindow();//������ʾ�����������ʱ�������ӳ�
			dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("������������"));
			//dlg->TextToSpeech(TulingRe);//��ͼ�������Ӧ����ת��Ϊ����
			dlg->StaSpeechPlay = false;//ֹͣ��ǰ����
			while (dlg->SpeechPlayFlg)Sleep(10);//�ȴ���һ��ͼ��ظ������ϳɽ����˳�
			dlg->SpeechWord = TulingRe;//�����ϳ���丳�����ñ���
			AfxBeginThread(SpeechPlay, dlg, THREAD_PRIORITY_HIGHEST);//��һ��
			dlg->AISwFlg = true;
		}
	}
	dlg->XunflySpe.XflySR_Close();//�ر�Ѷ��ʶ��
	OutputDebugStringA("����ʶ����̽���\n\n");
	while (!(dlg->AiCloseFlg)) Sleep(200);
	CButton* tem = (CButton*)dlg->GetDlgItem(IDC_SrSwitch);
	tem->SetWindowTextW(_T("������ʶ��"));
	dlg->GetDlgItem(IDC_AISwitch)->EnableWindow(false);
	dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("����ʶ���ѹر�"));
	dlg->GetDlgItem(IDC_SrSwitch)->EnableWindow(true);
	dlg->SpeechSwFlg = true;
	return 0;
}

UINT CSrobotDlg::SpeechPlay(void *param)
{
	CSrobotDlg* dlg = (CSrobotDlg*)param;
	dlg->SpeechPlayFlg = true;
	if (!dlg->SpeechWord.IsEmpty())//����ϳ����ݲ�Ϊ��
	{
		dlg->StaSpeechPlay = true;//��������״̬���ƴ�
		dlg->TextToSpeech(dlg->SpeechWord);
	}
	dlg->SpeechPlayFlg = false;
	return 0;
}

void CSrobotDlg::OrderAction(const char* myorder)
{
	std::string theorder;
	theorder = myorder;
	OrderSearch ordersearch;
	int Device, Order;
	if (ordersearch.Search(theorder, Device, Order))
	{
		if (Device == 2)
		{
			switch(Order)
			{
			case 0:
				OnBnClickedGymsw();
				break;
			case 1:
				OnBnClickedPiaffesw();
				break;
			case 2:
				OnBnClickedActionres();
				break;
			case 3:
				OnBnClickedStopall();
				break;
			case 4:
				OnBnClickedHugarm();
				break;
			case 5:
				OnBnClickedInvitearm();
				break;
			default:
				break;
			}
		}
		else
		{
			char sendorder = Order;
			if(Device==0)
				PhizModule.WriteToPort(&sendorder, sizeof(sendorder));
			else
				MoveModule.WriteToPort(&sendorder, sizeof(sendorder));
		}
	}
	return;
}


char* CSrobotDlg::SendToXfly()
{
	XunflySpe.Detection();//��ʼ¼�����Զ����ݶ˵���ץȡ
	if (XunflySRSwitchFlg == true)//��ϵ�����������ĺ�����ֹ����ر�����ʶ��ʱ�ٴ�ʶ��
		return "��Ǹ����û˵�����";
	XunflySpe.WriteToWav();//������Ƶ�ļ�
	XunflySpe.AutoRecord_Reset();//������λ
	XunflySpe.RecoSpere();//�������ϴ���Ѷ��
	char* result;
	result = XunflySpe.GetResult();
	return result;//��ȡʶ����
}

std::string CSrobotDlg::SendToTuling(char* mytext)
{
	std::string TSpeResult;
	TSpeResult = mytext;
	Tuling.InputText(TSpeResult);
	Tuling.PostToTuling();
	TSpeResult = Tuling.TulingResult();
	return TSpeResult;
}

//void CSrobotDlg::TextToSpeech(CString mytext)
//{
//	::CoInitialize(NULL);//��ʼ����������
//	ISpVoice * pSpVoice = NULL;//��ʼ����������
//	CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice);
//	//���������������������൱�ڴ�������������FAILED�Ǹ��궨�壬�������ж�CoCreateInstance���������û�гɹ��������������������ǲ��ɹ�����ʾ��Ϣ��
//
//	std::string temp;
//	temp = (LPCSTR)(CStringA)(mytext);
//	std::wstring wtemp;
//	wtemp = Tuling.StringToWstring(temp);
//
//	pSpVoice->Speak((wtemp.c_str()), SPF_DEFAULT, NULL);
//
//	pSpVoice->Release(); //�ͷ���������
//	::CoUninitialize();//�ͷ���������
//}

void CSrobotDlg::TextToSpeech(CString mytext)
{
	::CoInitialize(NULL);//��ʼ����������
	ISpVoice * pSpVoice = NULL;//��ʼ����������
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice);
	HRESULT status = true;//��ʼ��״̬����������ڼ�����������
						  //���������������������൱�ڴ�������������FAILED�Ǹ��궨�壬�������ж�CoCreateInstance���������û�гɹ��������������������ǲ��ɹ�����ʾ��Ϣ��

	std::string temp;
	temp = (LPCSTR)(CStringA)(mytext);
	std::wstring wtemp;
	wtemp = Tuling.StringToWstring(temp);

	CComPtr<ISpObjectToken> pOldToken;
	hr = pSpVoice->GetVoice(&pOldToken);

	// ���Ƚ�����ǰ���ʶ���
	hr = pSpVoice->Speak(NULL, SPF_PURGEBEFORESPEAK, 0);
	if (SUCCEEDED(hr))
	{
		hr = pSpVoice->Speak((wtemp.c_str()), SPF_ASYNC | SPF_IS_NOT_XML, 0);//��ʼ�ʶ�
		while (status&&StaSpeechPlay)//��������ֱ��������ɻ��ⲿ���д��
		{
			status = pSpVoice->WaitUntilDone(200); //ÿ�εȴ��ʶ�200ms(������ΪINFINITE��Ϊ�ȴ��ʶ�����)���ʶ������ú�������0
		}
	}
	pSpVoice->Release(); //�ͷ���������
	::CoUninitialize();//�ͷ���������
	SpeechWord.Empty();//��ձ��κϳ����
	StaSpeechPlay = false;
	return;
}

void CSrobotDlg::OnBnClickedAiswitch()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_AISwitch)->GetWindowTextW(temp);
	if (temp == "������Ӧ��")
	{
		GetDlgItem(IDC_AISwitch)->EnableWindow(false);
		GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("���ڴ�Ӧ��"));
		GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("�ر�����Ӧ��"));
		GetDlgItem(IDC_AISwitch)->EnableWindow(true);
		GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("����Ӧ���Ѵ�"));
	}
	else
	{
		GetDlgItem(IDC_AISwitch)->EnableWindow(false);
		GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("���ڹر�Ӧ��"));
		AfxBeginThread(AiClose, this, THREAD_PRIORITY_HIGHEST);
	}
}


void CSrobotDlg::OnBnClickedExitall()
{
	// TODO: Add your control notification handler code here
	unsigned char ReValue;
	ReValue = MessageBox(_T("   ȷ��Ҫ�˳�����ȷ�����������ѹر�!"), _T("�˳�"), MB_YESNO | MB_ICONQUESTION);
	if (ReValue == 6)
		CDialog::OnOK();
	else 
	{
		return;
	}
	//AfxGetMainWnd()->SendMessage(WM_CLOSE);
	//�˳�������AfxGetMainWnd()->SendMessage(WM_CLOSE);

	//�رյ�ǰ������DestroyWindow();

	//�ر�ģʽ�Ի�����EndDialog(0);
}
UINT CSrobotDlg::AiClose(void* param)
{
	CSrobotDlg* dlg = (CSrobotDlg*)param;
	dlg->AiCloseFlg = false;
	dlg->StaSpeechPlay = false;
	while (!(dlg->AISwFlg)) Sleep(200);
	while (dlg->SpeechPlayFlg) Sleep(200);
	dlg->GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("������Ӧ��"));
	dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("����Ӧ���ѹر�"));
	CString temp;
	dlg->GetDlgItem(IDC_SrSwitch)->GetWindowTextW(temp);
	dlg->GetDlgItem(IDC_AISwitch)->EnableWindow(true);
	dlg->AiCloseFlg = true;
	return 0;
}

void CSrobotDlg::DrawMat(cv::Mat& img, UINT nID)//����ͷ��ʾ������
{
	cv::Mat imgTmp;
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect);  // ��ȡ�ؼ���С
	cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// ����Mat������
																   // תһ�¸�ʽ ,��ο��Է�����,
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA); // GRAY��ͨ��
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);  // BGR��ͨ��   ��RGB,��case 3�����һ��,�ĳ�CV_RGB2BGRA.
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1); // ����һ�����ض��ٸ��ֽ�
															 // ����bitmapinfo(����ͷ)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	// Mat.data + bitmap����ͷ -> MFC
	CDC *pDC = GetDlgItem(nID)->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}

void CSrobotDlg::OnBnClickedCamerasw()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_CameraSw)->GetWindowTextW(temp);
	if (temp == "������ͷ")
	{
		ShowCaFlg = true;
		GetDlgItem(IDC_CameraSw)->EnableWindow(false);
		GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("���ڴ�����ͷ"));
		AfxBeginThread(ShowCamera, this, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		GetDlgItem(IDC_CameraSw)->EnableWindow(false);
		GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("���ڹر�����ͷ"));
		while (!(PostFaceFlg)) Sleep(200);//�ȴ�����ʶ����������
		ShowCaFlg = false;
	}
}

UINT CSrobotDlg::ShowCamera(void* param)
{
	CSrobotDlg* dlg = (CSrobotDlg*)param;

	std::vector<cv::Rect> faces;
	cv::CascadeClassifier face_cascade;
	cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
	face_cascade.load(face_cascade_name);

	cv::VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened())
	{
		AfxMessageBox(L"������ͷʧ�ܣ������豸");
		dlg->GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("������ͷ"));
		dlg->GetDlgItem(IDC_CameraSw)->EnableWindow(true);
		return 0;
	}
	cv::Mat frame;
	cv::Mat flipframe;
	cv::Mat grayImage;

	long secs = 5;
	clock_t delay = secs*CLOCKS_PER_SEC;
	clock_t lastphoto = clock() - delay;
	bool buttontext = true;
	while (dlg->ShowCaFlg)
	{
		capture >> frame;
		grayImage.create(frame.size(), frame.type());
		cv::cvtColor(frame, grayImage, CV_BGR2GRAY);
		face_cascade.detectMultiScale(grayImage, faces, 1.1, 3, 0 | CV_HAAR_DO_ROUGH_SEARCH, cv::Size(100, 100));
		for (size_t i = 0; i < faces.size(); i++)
		{
			if ((clock() - lastphoto > delay)&&(dlg->PostFaceFlg))
			{
				imwrite("faces.jpg", frame);
				lastphoto = clock();
				AfxBeginThread(PostFacepp, dlg, THREAD_PRIORITY_HIGHEST);
			}
			cv::Rect select;//��������  
			select.x = faces[i].x;
			select.y = faces[i].y;
			select.width = faces[i].width;
			select.height = faces[i].height;
			rectangle(frame, select, cv::Scalar(0, 255, 0), 2, 8, 0);//�����������Σ���������ʽvoid rectangle(Mat& img, Rect rec, const Scalar& color, int thickness=1, int lineType=8, int shift=0 )
			lastphoto = clock();
		}
		cv::flip(frame, flipframe, 1);//opencv3.x��ͼ��ת����
		dlg->DrawMat(flipframe, IDC_CameraDis);
		if (buttontext)
		{
			dlg->GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("�ر�����ͷ"));
			dlg->GetDlgItem(IDC_CameraSw)->EnableWindow(true);
			buttontext = false;
		}
		cv::waitKey(30);
	}
	dlg->GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("������ͷ"));
	dlg->GetDlgItem(IDC_CameraSw)->EnableWindow(true);
	return 0;
}

UINT CSrobotDlg::PostFacepp(void *param)
{
	CSrobotDlg* dlgt = (CSrobotDlg*)param;
	dlgt->PostFaceFlg = false;
	bool ErroFlg;
	Facepp PostFace;
	std::string PostRe;
	ErroFlg = PostFace.FaceDetect("faces.jpg");
	if (ErroFlg) PostRe = PostFace.GetFaPostRe();
	else
	{
		dlgt->PostFaceFlg = true;
		return 0;
	}
	ErroFlg = PostFace.SearchFFSet("faces.jpg", "myface");
	if (ErroFlg)
	{
		CString SayY;
		PostRe = PostFace.GetSearchRe();
		SayY = PostRe.c_str();
		SayY += "�ܸ��˼�������";
		while (!(dlgt->AISwFlg)) Sleep(200);
		while (dlgt->SpeechPlayFlg) Sleep(200);
		int lastLine = dlgt->m_SrResult.LineIndex(dlgt->m_SrResult.GetLineCount() - 1);
		dlgt->m_SrResult.SetSel(lastLine + 1, lastLine + 2, 1);
		dlgt->m_SrResult.ReplaceSel(L"AI��" + SayY + "\r\n");//�����һ�м��µ�ͼ������˷��ؽ��
		dlgt->m_SrResult.UpdateWindow();//������ʾ�����������ʱ�������ӳ�
		dlgt->StaSpeechPlay = true;
		dlgt->TextToSpeech(SayY);
	}
	else
	{
		dlgt->PostFaceFlg = true;
		return 0;
	}
	dlgt->PostFaceFlg = true;
	return 0;
}

void CSrobotDlg::OnBnClickedAdvacedset()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_CameraSw)->GetWindowTextW(temp);
	if (!(temp == "������ͷ"))
		AfxMessageBox(L"���ȹر�����ͷ");
	else
	{
		CManageSetDlg Dlg;
		Dlg.DoModal();
	}
}

void CSrobotDlg::OnBnClickedActionsw()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_ActionSw)->GetWindowTextW(temp);
	if (temp == "����ʾ����")
	{
		if (PhizFlg == false || MoveFlg == false|| ArmFlg==false)
			AfxMessageBox(L"�����úø�����");
		else
			GetDlgItem(IDC_ActionSw)->SetWindowTextW(_T("�ر���ʾ����"));
		return;
	}
	else
	{
		GetDlgItem(IDC_ActionSw)->SetWindowTextW(_T("����ʾ����"));
	}
}


void CSrobotDlg::OnBnClickedRelasetorque()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg)
	{
		AfxMessageBox(_T("���ȴ򿪻�е�۴���"));
		return;
	}
	OnBnClickedStopall();
	unsigned char sum = 0, ReValue;
	unsigned char UndoTorque[] = { 0xff, 0xff, 0xfe, 4, 0x03, 0x18, 0x00, 0x00 };//���ָ���
	for (unsigned char i = 2; i < sizeof(UndoTorque) - 1; i++) sum += UndoTorque[i];
	UndoTorque[sizeof(UndoTorque) - 1] = ~sum;//����SetTorque��У����
	sum = 0;
	ReValue = MessageBox(_T("   ȷ��Ҫ����Ť����"), _T("����Ť��"), MB_YESNO | MB_ICONQUESTION);
	if (ReValue == 6)
	{
		ArmModuleF.WriteToPort(UndoTorque, sizeof(UndoTorque));
		ArmModuleS.WriteToPort(UndoTorque, sizeof(UndoTorque));
	}
	return;
}

void CSrobotDlg::OnBnClickedActionres()
{
	// TODO: Add your control notification handler code here
	unsigned char Id_Tx749[] = { 0xff, 0xff, 0xfe, 19, 0x83, 0x1e, 4, 7, 230, 7, 10, 0, 4, 205, 6, 10, 0, 9, 197, 1, 10, 0, 0x00 };
	unsigned char Id_Tx865[] = { 0xff, 0xff, 0xfe, 19, 0x83, 0x1e, 4, 8, 146, 8, 10, 0, 6, 247, 7, 10, 0, 5, 209, 1, 10, 0, 0x00 };
	if (!ArmFlg)
	{
		AfxMessageBox(_T("��е�۴���δ���ӣ�"));
		return;
	}
	else
	{
		OnBnClickedStopall();
		unsigned char sum = 0;
		unsigned char Single[] = { 0xff, 0xff, 0x01, 0x07, 0x03, 0x1e, 244, 1, 100, 0, 0x00 };
		unsigned char Original[] = { 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 4, 7, 230, 7, 100, 0, 4, 205, 6, 100, 0, 9, 197, 1, 100, 0,
			8, 146, 8, 100, 0, 6, 247, 7, 100, 0, 5, 209, 1, 100, 0, 0x00 };
		for (unsigned char i = 2; i < sizeof(Original) - 1; i++) sum += Original[i];
		Original[sizeof(Original) - 1] = ~sum;//����У����
		sum = 0;
		for (unsigned char i = 2; i < sizeof(Single) - 1; i++) sum += Single[i];
		Single[sizeof(Single) - 1] = ~sum;//����У����
		sum = 0;
		ArmModuleF.WriteToPort(Original, sizeof(Original));//����ָ��
		Sleep(10);
		ArmModuleS.WriteToPort(Single, sizeof(Single));//����ָ��
	}
	return;
}


void CSrobotDlg::OnBnClickedGymsw()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg)
	{
		AfxMessageBox(_T("���ȴ򿪻�е�۴��ڣ�"));
		return;
	}
	CString temp;
	GetDlgItem(IDC_PiaffeSw)->GetWindowTextW(temp);
	if (temp == "ֹͣԭ��̤��")
		OnBnClickedPiaffesw();
	GetDlgItem(IDC_GymSw)->GetWindowTextW(temp);
	if (temp == "��ʼ�㲥���")
	{
		SetTimer(1, 2000, NULL);//�㲥�����3000
		GetDlgItem(IDC_GymSw)->SetWindowTextW(_T("ֹͣ�㲥���"));
	}
	else
	{
		KillTimer(1);
		Gymnastics = 0;
		GetDlgItem(IDC_GymSw)->SetWindowTextW(_T("��ʼ�㲥���"));
	}
}


void CSrobotDlg::OnBnClickedPiaffesw()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg)
	{
		AfxMessageBox(_T("���ȴ򿪻�е�۴��ڣ�"));
		return;
	}
	CString temp;
	GetDlgItem(IDC_GymSw)->GetWindowTextW(temp);
	if (temp == "ֹͣ�㲥���")
		OnBnClickedGymsw();
	GetDlgItem(IDC_PiaffeSw)->GetWindowTextW(temp);
	if (temp == "��ʼԭ��̤��")
	{
		SetTimer(2, 1000, NULL);//�㲥�����3000
		GetDlgItem(IDC_PiaffeSw)->SetWindowTextW(_T("ֹͣԭ��̤��"));
	}
	else
	{
		KillTimer(2);
		Gymnastics = 0;
		GetDlgItem(IDC_PiaffeSw)->SetWindowTextW(_T("��ʼԭ��̤��"));
	}
}


void CSrobotDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int n = nIDEvent;
	unsigned char sum = 0;
	unsigned char GymTx[3][38] = {
		{ 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 0x04, 0x04, 0x9e, 0x0c, 150, 0, 0x05, 0xc2, 0x01, 150, 0,
		0x06, 0xc3, 0x0d, 150, 0, 0x07, 0xd3, 0x07, 150, 0, 0x08, 0xcd, 0x08, 150, 0, 0x09, 0xb6, 0x01, 150, 0, 0x00 },
		{ 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 0x04, 0x04, 0x06, 0x07, 150, 0, 0x05, 0xc9, 0x01, 150, 0,
		0x06, 0x35, 0x08, 150, 0, 0x07, 0x00, 0x02, 150, 0, 0x08, 0x79, 0x0e, 150, 0, 0x09, 0xc0, 0x01, 150, 0, 0x00 },
		{ 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 0x04, 0x04, 0x9e, 0x0c, 150, 0, 0x05, 0x95, 0x00, 150, 0,
		0x06, 0xc3, 0x0d, 150, 0, 0x07, 0x00, 0x02, 150, 0, 0x08, 0x69, 0x0e, 150, 0, 0x09, 0x95, 0x00, 150, 0, 0x00 } };
	unsigned char GymTx1[][18] = {
		{ 0xff, 0xff, 0xfe, 14, 0x83, 0x1e, 0x04, 0x07, 0x00, 0x0a, 200, 0, 0x08, 0xa8, 0x0a, 200, 0, 0x00 },
		{ 0xff, 0xff, 0xfe, 14, 0x83, 0x1e, 0x04, 0x07, 0x6a, 0x05, 200, 0, 0x08, 0x6a, 0x06, 200, 0, 0x00 } };
	switch (n)
	{
	case 1:
		for (unsigned char i = 2; i < sizeof(GymTx[Gymnastics]) - 1; i++) sum += GymTx[Gymnastics][i];
		GymTx[Gymnastics][sizeof(GymTx[Gymnastics]) - 1] = ~sum;//����У����
		sum = 0;
		ArmModuleF.WriteToPort(GymTx[Gymnastics], sizeof(GymTx[Gymnastics]));//����ָ��
		Gymnastics++;
		if (Gymnastics == 3)Gymnastics = 0;
		break;
	case 2:
		for (unsigned char i = 2; i < sizeof(GymTx1[Gymnastics]) - 1; i++) sum += GymTx1[Gymnastics][i];
		GymTx1[Gymnastics][sizeof(GymTx1[Gymnastics]) - 1] = ~sum;//����У����
		sum = 0;
		ArmModuleF.WriteToPort(GymTx1[Gymnastics], sizeof(GymTx1[Gymnastics]));//����ָ��
		Gymnastics++;
		if (Gymnastics == 2)Gymnastics = 0;
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CSrobotDlg::OnBnClickedStopall()
{
	// TODO: Add your control notification handler code here
	char sendorder = 0;
	MoveModule.WriteToPort(&sendorder, sizeof(sendorder));
	if (!ArmFlg)
	{
		AfxMessageBox(_T("���ȴ򿪻�е�۴��ڣ�"));
		return;
	}
	unsigned char sum = 0;
	unsigned char STOP[] = { 0xff, 0xff, 0xfe, 5, 0x03, 0x20, 0X01, 0X00, 0x00 };
	for (unsigned char i = 2; i < sizeof(STOP) - 1; i++) sum += STOP[i];
	STOP[sizeof(STOP) - 1] = ~sum;//����STOP��У����
	CString temp;
	GetDlgItem(IDC_GymSw)->GetWindowTextW(temp);
	if (temp == "ֹͣ�㲥���")OnBnClickedGymsw();
	GetDlgItem(IDC_PiaffeSw)->GetWindowTextW(temp);
	if (temp == "ֹͣԭ��̤��")OnBnClickedPiaffesw();
	ArmModuleF.WriteToPort(STOP, sizeof(STOP));
	ArmModuleS.WriteToPort(STOP, sizeof(STOP));
}


void CSrobotDlg::OnBnClickedHugarm()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg)
	{
		AfxMessageBox(_T("���ȴ򿪻�е�۴��ڣ�"));
	}
	else
	{
		OnBnClickedStopall();
		unsigned char sum = 0;
		unsigned char Hug[] = { 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 4, 7, 55, 4, 100, 0, 4, 200, 7, 100, 0, 9, 238, 0, 100, 0,
			8, 55, 12, 100, 0, 6, 15, 9, 100, 0, 5, 249, 0, 100, 0, 0x00 };
		for (unsigned char i = 2; i < sizeof(Hug) - 1; i++) sum += Hug[i];
		Hug[sizeof(Hug) - 1] = ~sum;//����У����
		sum = 0;
		ArmModuleF.WriteToPort(Hug, sizeof(Hug));//����ָ��
	}
	return;
}


void CSrobotDlg::OnBnClickedInvitearm()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg||!PhizFlg)
	{
		AfxMessageBox(_T("���ȴ򿪻�е�ۺ��������ڣ�"));
	}
	else
	{
		OnBnClickedStopall();
		char order = 6;
		PhizModule.WriteToPort(&order, sizeof(order));
		unsigned char sum = 0;
		unsigned char Single[] = { 0xff, 0xff, 0x01, 0x07, 0x03, 0x1e, 104, 1, 100, 0, 0x00 };
		unsigned char Hug[] = { 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 4, 7, 188, 3, 100, 0, 4, 190, 8, 100, 0, 9, 181, 1, 100, 0,
			8, 146, 8, 100, 0, 6, 247, 7, 100, 0, 5, 209, 1, 100, 0, 0x00 };
		for (unsigned char i = 2; i < sizeof(Hug) - 1; i++) sum += Hug[i];
		Hug[sizeof(Hug) - 1] = ~sum;//����У����
		sum = 0;
		for (unsigned char i = 2; i < sizeof(Single) - 1; i++) sum += Single[i];
		Single[sizeof(Single) - 1] = ~sum;//����У����
		sum = 0;
		ArmModuleF.WriteToPort(Hug, sizeof(Hug));//����ָ��
		ArmModuleS.WriteToPort(Single, sizeof(Single));//����ָ��
	}
	return;
}
