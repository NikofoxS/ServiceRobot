// ManageSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Srobot.h"
#include "ManageSetDlg.h"
#include "afxdialogex.h"


// CManageSetDlg dialog

IMPLEMENT_DYNAMIC(CManageSetDlg, CDialog)

CManageSetDlg::CManageSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MANAGESET_DIALOG, pParent)
{

}

CManageSetDlg::~CManageSetDlg()
{
}

void CManageSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BaudPhiz, m_BaudPhiz);
	DDX_Control(pDX, IDC_ComPhiz, m_ComPhiz);
	DDX_Control(pDX, IDC_ComMove, m_ComMove);
	DDX_Control(pDX, IDC_BaudMove, m_BaudMove);
	DDX_Control(pDX, IDC_BaudArmF, m_BaudArmF);
	DDX_Control(pDX, IDC_BaudArmS, m_BaudArmS);
	DDX_Control(pDX, IDC_ComArmF, m_ComArmF);
	DDX_Control(pDX, IDC_ComArmS, m_ComArmS);
}


BEGIN_MESSAGE_MAP(CManageSetDlg, CDialog)
	ON_BN_CLICKED(IDC_ExitMana, &CManageSetDlg::OnBnClickedExitmana)
	ON_BN_CLICKED(IDC_LensSw, &CManageSetDlg::OnBnClickedLenssw)
	ON_BN_CLICKED(IDC_ChoosePic, &CManageSetDlg::OnBnClickedChoosepic)
	ON_BN_CLICKED(IDC_TakePic, &CManageSetDlg::OnBnClickedTakepic)
	ON_BN_CLICKED(IDC_UploadPic, &CManageSetDlg::OnBnClickedUploadpic)
	ON_BN_CLICKED(IDC_DetecCom, &CManageSetDlg::OnBnClickedDeteccom)
	ON_BN_CLICKED(IDC_SwPhiz, &CManageSetDlg::OnBnClickedSwphiz)
	ON_BN_CLICKED(IDC_SwMove, &CManageSetDlg::OnBnClickedSwmove)
	ON_BN_CLICKED(IDC_SwArm, &CManageSetDlg::OnBnClickedSwarm)
	ON_BN_CLICKED(IDC_ActTorque, &CManageSetDlg::OnBnClickedActtorque)
END_MESSAGE_MAP()


// CManageSetDlg message handlers

BOOL CManageSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_ExitMana)->SetWindowTextW(_T("�˳�����"));
	GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("��ʼ����"));
	GetDlgItem(IDC_TakePic)->SetWindowTextW(_T("ȷ����Ϣ"));
	GetDlgItem(IDC_ChoosePic)->SetWindowTextW(_T("ѡ����Ƭ"));
	GetDlgItem(IDC_UploadPic)->SetWindowTextW(_T("¼����Ϣ"));
	GetDlgItem(IDC_ActTorque)->SetWindowTextW(_T("����Ť��"));
	GetDlgItem(IDC_TakePic)->EnableWindow(false);
	GetDlgItem(IDC_UploadPic)->EnableWindow(false);
	ShowLeFlg = false;
	GetDlgItem(IDC_SwPhiz)->SetWindowTextW(_T("�򿪱��鴮��"));
	PhizModuleIniti();
	GetDlgItem(IDC_SwMove)->SetWindowTextW(_T("���ƶ�����"));
	MoveModuleIniti();
	GetDlgItem(IDC_SwArm)->SetWindowTextW(_T("�򿪻�е�ֱ۴���"));
	ArmModuleIniti();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageSetDlg::PhizModuleIniti()
{
	CString temp;
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//ָ�򸸴��ڵ�ַ��ָ�롣
	//��ӱ����ʵ�ֵ������ģ�������ѡ��combopx�ؼ�
	for (int i = 0; i < sizeof(BaudRate) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), BaudRate[i]);
		m_BaudPhiz.AddString((LPCTSTR)temp);
	}
	temp.Format(_T("%d"), 9600);
	m_BaudPhiz.SetCurSel(m_BaudPhiz.FindStringExact(0, temp));//�趨����ģ�������Ĭ��ѡ��Ϊ9600
	pDlg->PhizModule.Hkey2ComboBox(m_ComPhiz);//��鴮�ںŲ���������ģ���COM��ѡ��combobox�ؼ�
	if (pDlg->PhizFlg)//�����ģ�鴮�������ã������ϴε����ý��г�ʼ��
	{
		temp.Format(_T("%d"), pDlg->PhizInf[1]);
		m_BaudPhiz.SetCurSel(m_BaudPhiz.FindStringExact(0, temp));
		GetDlgItem(IDC_BaudPhiz)->EnableWindow(false);
		CString CTemp;
		CTemp.Format(_T("COM%d"), pDlg->PhizInf[0]);
		m_ComPhiz.SetCurSel(m_ComPhiz.FindStringExact(0, CTemp));
		GetDlgItem(IDC_ComPhiz)->EnableWindow(false);
		GetDlgItem(IDC_SwPhiz)->SetWindowTextW(_T("�رձ��鴮��"));
	}
	return;
}

void CManageSetDlg::MoveModuleIniti()
{
	CString temp;
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//ָ�򸸴��ڵ�ַ��ָ�롣
													  //��ӱ����ʵ�ֵ������ģ�������ѡ��combopx�ؼ�
	for (int i = 0; i < sizeof(BaudRate) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), BaudRate[i]);
		m_BaudMove.AddString((LPCTSTR)temp);
	}
	temp.Format(_T("%d"), 9600);
	m_BaudMove.SetCurSel(m_BaudMove.FindStringExact(0, temp));//�趨����ģ�������Ĭ��ѡ��Ϊ9600
	pDlg->PhizModule.Hkey2ComboBox(m_ComMove);//��鴮�ںŲ���������ģ���COM��ѡ��combobox�ؼ�
	if (pDlg->MoveFlg)//�����ģ�鴮�������ã������ϴε����ý��г�ʼ��
	{
		temp.Format(_T("%d"), pDlg->MoveInf[1]);
		m_BaudMove.SetCurSel(m_BaudMove.FindStringExact(0, temp));
		GetDlgItem(IDC_BaudMove)->EnableWindow(false);
		CString CTemp;
		CTemp.Format(_T("COM%d"), pDlg->MoveInf[0]);
		m_ComMove.SetCurSel(m_ComMove.FindStringExact(0, CTemp));
		GetDlgItem(IDC_ComMove)->EnableWindow(false);
		GetDlgItem(IDC_SwMove)->SetWindowTextW(_T("�ر��ƶ�����"));
	}
	return;
}

void CManageSetDlg::ArmModuleIniti()
{
	CString temp;
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//ָ�򸸴��ڵ�ַ��ָ�롣
													  //��ӱ����ʵ�ֵ������ģ�������ѡ��combopx�ؼ�
	for (int i = 0; i < sizeof(BaudRate) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), BaudRate[i]);
		m_BaudArmF.AddString((LPCTSTR)temp);
		m_BaudArmS.AddString((LPCTSTR)temp);
	}
	temp.Format(_T("%d"), 9600);
	m_BaudArmF.SetCurSel(m_BaudArmF.FindStringExact(0, temp));//�趨��е��ģ��F������Ĭ��ѡ��Ϊ9600
	m_BaudArmS.SetCurSel(m_BaudArmS.FindStringExact(0, temp));//�趨��е��ģ��S������Ĭ��ѡ��Ϊ9600
	pDlg->ArmModuleF.Hkey2ComboBox(m_ComArmF);//��鴮�ںŲ�������е��ģ��F��COM��ѡ��combobox�ؼ�
	pDlg->ArmModuleS.Hkey2ComboBox(m_ComArmS);//��鴮�ںŲ�������е��ģ��F��COM��ѡ��combobox�ؼ�
	if (pDlg->ArmFlg)//�����ģ�鴮�������ã������ϴε����ý��г�ʼ��
	{
		temp.Format(_T("%d"), pDlg->ArmInf[1]);
		m_BaudArmF.SetCurSel(m_BaudArmF.FindStringExact(0, temp));
		GetDlgItem(IDC_BaudArmF)->EnableWindow(false);
		CString CTemp;
		CTemp.Format(_T("COM%d"), pDlg->ArmInf[0]);
		m_ComArmF.SetCurSel(m_ComArmF.FindStringExact(0, CTemp));
		GetDlgItem(IDC_ComArmF)->EnableWindow(false);

		temp.Format(_T("%d"), pDlg->ArmInf[3]);
		m_BaudArmS.SetCurSel(m_BaudArmS.FindStringExact(0, temp));
		GetDlgItem(IDC_BaudArmS)->EnableWindow(false);
		CTemp.Format(_T("COM%d"), pDlg->ArmInf[2]);
		m_ComArmS.SetCurSel(m_ComArmS.FindStringExact(0, CTemp));
		GetDlgItem(IDC_ComArmS)->EnableWindow(false);
		GetDlgItem(IDC_SwArm)->SetWindowTextW(_T("�رջ�е�۴���"));
	}
	return;
}

void CManageSetDlg::OnBnClickedExitmana()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_LensSw)->GetWindowTextW(temp);
	if (temp == "����" || temp == "��������")
	{
		AfxMessageBox(L"���������ա������˳�");
	}
	else
		CDialog::OnOK();
}

void CManageSetDlg::OnBnClickedLenssw()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Called)->EnableWindow(true);
	GetDlgItem(IDC_UploadPic)->EnableWindow(false);
	CString temptext;
	GetDlgItem(IDC_LensSw)->GetWindowTextW(temptext);
	if ((temptext == "��ʼ����")||(temptext == "����"))
	{
		GetDlgItem(IDC_TakePic)->EnableWindow(false);
		GetDlgItem(IDC_ChoosePic)->EnableWindow(false);
		GetDlgItem(IDC_LensSw)->EnableWindow(false);
		GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("��������"));
		ShowLeFlg = true;
		AfxBeginThread(ShowLens, this, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		GetDlgItem(IDC_LensSw)->EnableWindow(false);
		ShowLeFlg = false;
	}
}

UINT CManageSetDlg::ShowLens(void* param)
{
	CManageSetDlg*dlg = (CManageSetDlg*)param;
	cv::VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened())
	{
		AfxMessageBox(L"������ͷʧ�ܣ������豸");
		dlg->GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("��ʼ����"));
		dlg->GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
		dlg->GetDlgItem(IDC_LensSw)->EnableWindow(true);
		return 0;
	}
	cv::Mat frame;
	cv::Mat flipframe;
	cv::Mat grayImage;
	bool buttonflg = true;
	while (dlg->ShowLeFlg)
	{
		capture >> frame;
		cv::flip(frame, flipframe, 1);//opencv3.x��ͼ��ת����
		dlg->DrawPic(flipframe, IDC_FacePic);
		if (buttonflg)
		{
			dlg->GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("����"));
			dlg->GetDlgItem(IDC_LensSw)->EnableWindow(true);
			buttonflg++;
		}
		cv::waitKey(30);
	}
	cv::imwrite("newface.jpg", frame);
	dlg->GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("����"));
	dlg->GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
	dlg->GetDlgItem(IDC_LensSw)->EnableWindow(true);
	dlg->GetDlgItem(IDC_TakePic)->EnableWindow(true);
	return 0;
}

void CManageSetDlg::DrawPic(cv::Mat& img, UINT nID)//��ͷ��ʾ������
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
	return;
}

void CManageSetDlg::OnBnClickedChoosepic()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Called)->EnableWindow(true);
	GetDlgItem(IDC_UploadPic)->EnableWindow(false);
	GetDlgItem(IDC_ChoosePic)->EnableWindow(false);
	GetDlgItem(IDC_ChoosePic)->SetWindowTextW(_T("��������"));
	CFileDialog fileDialog(TRUE, L"jpg", L"*.jpg", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"JPG�ļ�(*.jpg)|*.jpg|JPEG�ļ�(*.jpg)|*.jpeg|PNG�ļ�(*.png)|*.png|", NULL);
	//fileDialog.DoModal();//fileDialog.DoModal()=IDOK
	fileDialog.GetOFN().lpstrInitialDir = L"C:\\";
	INT_PTR result = fileDialog.DoModal();
	CString tempname = fileDialog.GetPathName();
	if (tempname == "*.jpg")
	{
		GetDlgItem(IDC_ChoosePic)->SetWindowTextW(_T("ѡ����Ƭ"));
		GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
		return;
	}
	std::string filename = (LPCSTR)(CStringA)(tempname);
	cv::Mat img = cv::imread(filename);
	imwrite("newface.jpg", img);
	DrawPic(img, IDC_FacePic);
	GetDlgItem(IDC_TakePic)->EnableWindow(true);
	GetDlgItem(IDC_ChoosePic)->SetWindowTextW(_T("ѡ����Ƭ"));
	GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
}


void CManageSetDlg::OnBnClickedTakepic()
{
	// TODO: Add your control notification handler code here
	CString UserCalled;
	GetDlgItem(IDC_Called)->GetWindowTextW(UserCalled);
	if (!(UserCalled.GetLength()))
	{
		AfxMessageBox(L"����д���ĳƺ�");
		return;
	}
	else
	{
		GetDlgItem(IDC_UploadPic)->EnableWindow(true);
		GetDlgItem(IDC_Called)->EnableWindow(false);
		AfxMessageBox(L"��Ϣ��ȷ�ϣ�����¼����Ϣ");
	}
}


void CManageSetDlg::OnBnClickedUploadpic()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ExitMana)->EnableWindow(false);
	GetDlgItem(IDC_UploadPic)->EnableWindow(false);
	GetDlgItem(IDC_LensSw)->EnableWindow(false);
	GetDlgItem(IDC_TakePic)->EnableWindow(false);
	GetDlgItem(IDC_ChoosePic)->EnableWindow(false);
	GetDlgItem(IDC_UploadPic)->SetWindowTextW(_T("����¼��"));
	AfxBeginThread(UploadFace, this, THREAD_PRIORITY_HIGHEST);
}

UINT CManageSetDlg::UploadFace(void* param)
{
	CManageSetDlg*dlg = (CManageSetDlg*)param;
	Facepp upload;
	std::string facetoken;
	std::string name;
	CString namere;
	if (!(upload.FaceDetect("newface.jpg")))
	{
		AfxMessageBox(L"δ��ͼ���м�⵽������������ѡ��ͼ��");
		goto end;
	}
	else
	{
		facetoken = upload.FDetectToken();
		if (upload.SearchFFSet("newface.jpg", "myface"))
		{
			name=upload.GetSearchRe();
			namere = name.c_str();
			namere += "��������Ϣ��¼�������ظ�¼��";
			AfxMessageBox(namere);
			goto end;
		}
		dlg->GetDlgItem(IDC_Called)->GetWindowTextW(namere);
		name= (LPCSTR)(CStringA)(namere);
		if (upload.FaceTokenID(facetoken, name))
		{
			if ((upload.FSetAddFace(facetoken, "myface")))
				AfxMessageBox(L"������¼��ɹ�");
			else AfxMessageBox(L"¼��ʧ�ܣ�������Ϣ������");
		}
		else AfxMessageBox(L"¼��ʧ�ܣ�������Ϣ������");
	}
end:
	dlg->GetDlgItem(IDC_UploadPic)->SetWindowTextW(_T("¼����Ϣ"));
	dlg->GetDlgItem(IDC_ExitMana)->EnableWindow(true);
	dlg->GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
	dlg->GetDlgItem(IDC_LensSw)->EnableWindow(true);
	dlg->GetDlgItem(IDC_Called)->EnableWindow(true);
	return 0;
}

void CManageSetDlg::OnBnClickedDeteccom()
{
	// TODO: Add your control notification handler code here
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//ָ�򸸴��ڵ�ַ��ָ��
	if ((pDlg->PhizFlg)||(pDlg->MoveFlg)||(pDlg->ArmFlg))
	{
		AfxMessageBox(L"���ȹر����д���");
		return;
	}
	pDlg->PhizModule.Hkey2ComboBox(m_ComPhiz);//��鴮�ںŲ���������ģ���COM��ѡ��combobox�ؼ�
	pDlg->MoveModule.Hkey2ComboBox(m_ComMove);//��鴮�ںŲ������ƶ�ģ���COM��ѡ��combobox�ؼ�
	pDlg->ArmModuleF.Hkey2ComboBox(m_ComArmF);//��鴮�ںŲ�������е��ģ��F��COM��ѡ��combobox�ؼ�
	pDlg->ArmModuleS.Hkey2ComboBox(m_ComArmS);//��鴮�ںŲ�������е��ģ��S��COM��ѡ��combobox�ؼ�
}

void CManageSetDlg::OnBnClickedSwphiz()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SwPhiz)->EnableWindow(false);
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//ָ�򸸴��ڵ�ַ��ָ��
	CString temp;
	int com_no, baudrate;
	UpdateData(true);
	m_ComPhiz.GetWindowText(temp);
	temp.Delete(0, 3);
	com_no = _tstoi(temp);
	m_BaudPhiz.GetWindowText(temp);
	baudrate = _tstoi(temp);
	GetDlgItem(IDC_SwPhiz)->GetWindowTextW(temp);
	if (temp == _T("�رձ��鴮��"))
	{
		m_ComPhiz.EnableWindow(true);
		m_BaudPhiz.EnableWindow(true);
		pDlg->PhizModule.ClosePort();
		pDlg->PhizFlg = false;
		pDlg->PhizInf[0] = 0, pDlg->PhizInf[1] = 0;
		GetDlgItem(IDC_SwPhiz)->SetWindowTextW(_T("�򿪱��鴮��"));
	}
	else if (m_ComPhiz.GetCount() > 0)
	{
		if (pDlg->PhizModule.InitPort(this->GetSafeHwnd(), com_no, baudrate))//�򿪴���
		{
			/*temp.Format(_T("���� %d �Ѵ򿪣��趨�ô��ڵĲ�����Ϊ %d\n "), com_no, baudrate);*/
			pDlg->PhizModule.StartMonitoring();
			pDlg->PhizInf[0] = com_no, pDlg->PhizInf[1] = baudrate;//��������Ϣ��¼��������
			m_ComPhiz.EnableWindow(false);
			m_BaudPhiz.EnableWindow(false);
			pDlg->PhizFlg = true;
			GetDlgItem(IDC_SwPhiz)->SetWindowTextW(_T("�رձ��鴮��"));
		}
		else
			AfxMessageBox(_T("���ڴ�ʧ�ܣ�����ô����Ƿ���ڻ��Ƿ��ѱ�ռ��"));
	}
	else
		AfxMessageBox(_T("û�м�⵽�ô���"));
	UpdateData(false);
	GetDlgItem(IDC_SwPhiz)->EnableWindow(true);
}


void CManageSetDlg::OnBnClickedSwmove()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SwMove)->EnableWindow(false);
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//ָ�򸸴��ڵ�ַ��ָ��
	CString temp;
	int com_no, baudrate;
	UpdateData(true);
	m_ComMove.GetWindowText(temp);
	temp.Delete(0, 3);
	com_no = _tstoi(temp);
	m_BaudMove.GetWindowText(temp);
	baudrate = _tstoi(temp);
	GetDlgItem(IDC_SwMove)->GetWindowTextW(temp);
	if (temp == _T("�ر��ƶ�����"))
	{
		m_ComMove.EnableWindow(true);
		m_BaudMove.EnableWindow(true);
		pDlg->MoveModule.ClosePort();
		pDlg->MoveFlg = false;
		pDlg->MoveInf[0] = 0, pDlg->MoveInf[1] = 0;
		GetDlgItem(IDC_SwMove)->SetWindowTextW(_T("���ƶ�����"));
	}
	else if (m_ComMove.GetCount() > 0)
	{
		if (pDlg->MoveModule.InitPort(this->GetSafeHwnd(), com_no, baudrate))//�򿪴���
		{
			/*temp.Format(_T("���� %d �Ѵ򿪣��趨�ô��ڵĲ�����Ϊ %d\n "), com_no, baudrate);*/
			pDlg->MoveModule.StartMonitoring();
			pDlg->MoveInf[0] = com_no, pDlg->MoveInf[1] = baudrate;//��������Ϣ��¼��������
			m_ComMove.EnableWindow(false);
			m_BaudMove.EnableWindow(false);
			pDlg->MoveFlg = true;
			GetDlgItem(IDC_SwMove)->SetWindowTextW(_T("�ر��ƶ�����"));
		}
		else
			AfxMessageBox(_T("���ڴ�ʧ�ܣ�����ô����Ƿ���ڻ��Ƿ��ѱ�ռ��"));
	}
	else
		AfxMessageBox(_T("û�м�⵽�ô���"));
	UpdateData(false);
	GetDlgItem(IDC_SwMove)->EnableWindow(true);
}


void CManageSetDlg::OnBnClickedSwarm()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SwArm)->EnableWindow(false);
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//ָ�򸸴��ڵ�ַ��ָ��
	CString temp;
	int com_noF, baudrateF, com_noS, baudrateS;
	UpdateData(true);
	m_ComArmF.GetWindowText(temp);
	temp.Delete(0, 3);
	com_noF = _tstoi(temp);
	m_BaudArmF.GetWindowText(temp);
	baudrateF = _tstoi(temp);
	m_ComArmS.GetWindowText(temp);
	temp.Delete(0, 3);
	com_noS = _tstoi(temp);
	m_BaudArmS.GetWindowText(temp);
	baudrateS = _tstoi(temp);

	GetDlgItem(IDC_SwArm)->GetWindowTextW(temp);
	if (temp == _T("�رջ�е�ֱ۴���"))
	{
		m_ComArmF.EnableWindow(true);
		m_BaudArmF.EnableWindow(true);
		m_ComArmS.EnableWindow(true);
		m_BaudArmS.EnableWindow(true);
		pDlg->ArmModuleF.ClosePort();
		pDlg->ArmModuleS.ClosePort();
		pDlg->ArmFlg = false;
		pDlg->ArmInf[0] = 0, pDlg->ArmInf[1] = 0, pDlg->ArmInf[2] = 0, pDlg->ArmInf[3] = 0;
		GetDlgItem(IDC_SwArm)->SetWindowTextW(_T("�򿪻�е�ֱ۴���"));
	}
	else if (m_ComArmF.GetCount() > 0&& m_ComArmS.GetCount() > 0)
	{
		if (pDlg->ArmModuleF.InitPort(this->GetSafeHwnd(), com_noF, baudrateF)&&
			pDlg->ArmModuleS.InitPort(this->GetSafeHwnd(), com_noS, baudrateS))//�򿪴���
		{
			/*temp.Format(_T("���� %d �Ѵ򿪣��趨�ô��ڵĲ�����Ϊ %d\n "), com_no, baudrate);*/
			pDlg->ArmModuleF.StartMonitoring();
			pDlg->ArmModuleS.StartMonitoring();
			pDlg->ArmInf[0] = com_noF, pDlg->ArmInf[1] = baudrateF,
				pDlg->ArmInf[2] = com_noS, pDlg->ArmInf[3] = baudrateS;//��������Ϣ��¼��������
			m_ComArmF.EnableWindow(false);
			m_BaudArmF.EnableWindow(false);
			m_ComArmS.EnableWindow(false);
			m_BaudArmS.EnableWindow(false);
			pDlg->ArmFlg = true;
			GetDlgItem(IDC_SwArm)->SetWindowTextW(_T("�رջ�е�ֱ۴���"));
		}
		else
			AfxMessageBox(_T("���ڴ�ʧ�ܣ�����ô����Ƿ���ڻ��Ƿ��ѱ�ռ��"));
	}
	else
		AfxMessageBox(_T("û�м�⵽�ô���"));
	UpdateData(false);
	GetDlgItem(IDC_SwArm)->EnableWindow(true);
}


void CManageSetDlg::OnBnClickedActtorque()
{
	// TODO: Add your control notification handler code here
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//ָ�򸸴��ڵ�ַ��ָ��
	if(!(pDlg->ArmFlg))
	{
		AfxMessageBox(_T("���ȴ򿪻�е�۴���"));
		return;
	}
	unsigned char sum = 0, ReValue;
	unsigned char SetTorque[] = { 0xff, 0xff, 0xfe, 4, 0x03, 0x18, 0x01, 0x00 };//���ָ���
	for (unsigned char i = 2; i < sizeof(SetTorque) - 1; i++) sum += SetTorque[i];
	SetTorque[sizeof(SetTorque)-1] = ~sum;//����SetTorque��У����
	sum = 0;
	ReValue = MessageBox(_T("   ȷ��Ҫ����Ť����"), _T("����Ť��"), MB_YESNO | MB_ICONQUESTION);
	int aaa = sizeof(SetTorque);
	if (ReValue == 6)
	{
		pDlg->ArmModuleF.WriteToPort(SetTorque, sizeof(SetTorque));
		pDlg->ArmModuleS.WriteToPort(SetTorque, sizeof(SetTorque));
	}
	return;
}
