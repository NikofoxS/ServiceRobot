#include "stdafx.h"
#include "Public.h"

UINT com[2] = { 0 };
bool PortOpened = false;

CPublic::CPublic() :PortIsOpened(false){}//���캯�����ڱ�����ʼ��
CPublic::~CPublic()
{
	if (PortIsOpened)Clearcom();
	else return;
}

bool CPublic::InitPort_1()
{
	CString str;
	if (PortIsOpened && com[1]==com[0])
	{
		str.Format(_T("%s %u %s"), _T("����"), com[0], _T("�����ӣ�"));
		AfxMessageBox(str);
		return true;
	}
	else
	{
		Clearcom();
		DCB	dcb;//DCB��һ���ṹ��
		TCHAR cscom[10];//�ַ�����
		COMMTIMEOUTS commTimeouts;//��Ҳ��һ���ṹ������
		
		m_rdos1.Offset = 0;
		m_rdos1.OffsetHigh = 0;
		m_rdos1.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_rdos1.hEvent == NULL)return false;
		m_wtos1.Offset = 0;
		m_wtos1.OffsetHigh = 0;
		m_wtos1.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_wtos1.hEvent == NULL)
		{
			CloseHandle(m_rdos1.hEvent);
			return false;
		}
	
		wsprintf(cscom, _T("com%u"), com[0]);	//�˴��������ںţ������޸�
	
		m_hcom1 = CreateFile(cscom, GENERIC_READ | GENERIC_WRITE,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			NULL);
		//�򿪴��ڣ��첽I/O��ʽ
		if (m_hcom1 == INVALID_HANDLE_VALUE)
		{
			str.Format(_T("%s %u %s"), _T("����"), com[0], _T("��ռ�ã�"));
			AfxMessageBox(str);
			return false;
		}//������ڱ�ռ�ã���������������
		else
		{
			SetCommMask(m_hcom1, EV_RXCHAR);//EV_RXCHAR��ʾ���뻺���������յ����ݣ������յ�һ���ֽڲ��������뻺����
			// �����豸������
			SetupComm(m_hcom1, 4096, 4096);
			// ��ʼ���������е���Ϣ
			PurgeComm(m_hcom1, PURGE_TXABORT | PURGE_RXABORT |
				PURGE_TXCLEAR | PURGE_RXCLEAR);
			// set up for overlapped I/O

			commTimeouts.ReadIntervalTimeout = 100;
			commTimeouts.ReadTotalTimeoutConstant = 100;
			commTimeouts.ReadTotalTimeoutMultiplier = 100;
			commTimeouts.WriteTotalTimeoutConstant = 100;
			commTimeouts.WriteTotalTimeoutMultiplier = 100;//����msΪ��λ
			SetCommTimeouts(m_hcom1, &commTimeouts);
		}

		EscapeCommFunction(m_hcom1, SETDTR);//���øߵ�ƽ
		if (!GetCommState(m_hcom1, &dcb))//��ȡ��������
		{
			AfxMessageBox(_T("�޷���ȡ����"));
			//����ͨ������ִ�У���Ϊ������ڱ�ռ���������ǰ��������
			return false;
		}
		else
		{
			dcb.BaudRate = 57600;
			dcb.fBinary = TRUE;
			dcb.fParity = FALSE;
			dcb.StopBits = ONESTOPBIT;
			dcb.ByteSize = 8;
			dcb.Parity = NOPARITY;
			dcb.fOutxCtsFlow = FALSE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
			dcb.fDsrSensitivity = FALSE;
			dcb.fTXContinueOnXoff = FALSE;
			dcb.fOutX = FALSE;
			dcb.fInX = FALSE;
			dcb.fErrorChar = FALSE;
			dcb.fNull = FALSE;
			dcb.fRtsControl = RTS_CONTROL_DISABLE;
			dcb.fAbortOnError = TRUE;
			//�ο���ҳ����http://blog.csdn.net/wangshubo1989/article/details/47746401
		}
		if (!SetCommState(m_hcom1, &dcb))//����DCB����
		{
			AfxMessageBox(_T("�޷����ô���״̬"));
			//����ͨ������ִ�У���Ϊ������ڱ�ռ���������ǰ��������
			return false;
		}
		else
		{
			str.Format(_T("%s %u %s"), _T("����"), com[0], _T("���ӳɹ���"));
			AfxMessageBox(str, MB_ICONINFORMATION);
			com[1] = com[0]; com[0] = 0;
			PortIsOpened = true;//���ô��ڱ�־λ
			PortOpened = true;
			return true;
		}		
	}
}

bool CPublic::WriteToPort_1(unsigned char*buff, int length)
{
	BOOL fWriteStat;
	DWORD dwBytesWritten;
	DWORD dwErrorFlags;
	DWORD dwError;
	DWORD dwBytesSent = 0;
	COMSTAT comStat;
	TCHAR szError[128];

	fWriteStat = WriteFile(m_hcom1, buff, length, &dwBytesWritten, &(m_wtos1));

	if (!fWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			while (!GetOverlappedResult(m_hcom1, &(m_wtos1), &dwBytesWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError == ERROR_IO_INCOMPLETE)
				{
					dwBytesSent += dwBytesWritten;
					continue;
				}
				else
				{
					wsprintf(szError, _T("<CE-%u>"), dwError);
					ClearCommError(m_hcom1, &dwErrorFlags, &comStat);
					break;
				}
			}
			dwBytesSent += dwBytesWritten;

			if (dwBytesSent != length)
				wsprintf(szError, _T("\nProbable Write Timeout: Total of %ld bytes sent"), dwBytesSent);
		}
		else
		{
			ClearCommError(m_hcom1, &dwErrorFlags, &comStat);
			return (FALSE);
		}
	}
	return true;
}

bool CPublic::ReceiveFromPort_1(unsigned char*buffer, int BytesToRead)
{
	BOOL       fReadStat;
	COMSTAT    comStat;
	DWORD      dwErrorFlags;
	DWORD      dwLength = BytesToRead;
	DWORD      dwActulLength = 0;
	DWORD      dwError;
	TCHAR      szError[10];

	ClearCommError(m_hcom1, &dwErrorFlags, &comStat);

	if (dwLength > 0)
	{
		fReadStat = ReadFile(m_hcom1, buffer, dwLength, &dwActulLength, &(m_rdos1));
		if (!fReadStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				OutputDebugString(_T("\n\rIO Pending"));
				// ����ȵ����ݶ�ȡ��ɣ�����������
				// commTimeOuts.ReadTotalTimeoutConstant����
				// ��������ܳ�ʱ����
				while (!GetOverlappedResult(m_hcom1, &(m_rdos1), &dwLength, TRUE))
				{
					dwError = GetLastError();
					if (dwError == ERROR_IO_INCOMPLETE)
						continue;
					else
					{
						wsprintf(szError, _T("<CE-%u>"), dwError);
						ClearCommError(m_hcom1, &dwErrorFlags, &comStat);
						break;
					}
				}
			}
			else
			{
				// ������������
				dwLength = 0;
				ClearCommError(m_hcom1, &dwErrorFlags, &comStat);
			}
		}
	}
	return true;
}

void CPublic::Clearcom()
{
	if (PortIsOpened == true)
	{
		SetCommMask(m_hcom1, 0);
		PurgeComm(m_hcom1, PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(m_hcom1);
		m_hcom1 = NULL;
		CloseHandle(m_rdos1.hEvent);
		CloseHandle(m_wtos1.hEvent);
		PortIsOpened = false;
	}
}