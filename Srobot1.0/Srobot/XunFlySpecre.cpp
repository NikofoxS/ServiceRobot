#include"stdafx.h"
#include"XunFlySpecre.h"



//-----------------------��AutoRecord-------------------
#pragma region ��AutoRecord
//��̬������ʼ��
unsigned char AutoRecord::vda_buffer0[ARFRAGMENT_SIZE] = { 0 };
unsigned char AutoRecord::vda_buffer01[ARFRAGMENT_SIZE] = { 0 };
unsigned char AutoRecord::vda_buffer02[ARFRAGMENT_SIZE] = { 0 };
short AutoRecord::vda_buffer2[ARFRAGMENT_SIZE / 2] = { 0 };
float AutoRecord::vda_buffer1[4] = { 0 };
int AutoRecord::vda_buf_count = 0;
int AutoRecord::vda_count = 0;
volatile int AutoRecord::vda_bos = 0, AutoRecord::vda_eos = 0, AutoRecord::eos_flag = 0;
float AutoRecord::vda_start = 0, AutoRecord::vda_end = 0;
int AutoRecord::sensitivity;

std::vector<unsigned char>AutoRecord::vda_buffer = {};
WAVEHDR AutoRecord::wh[ARFRAGMENT_NUM] = {};
volatile bool AutoRecord::stop = false;

AutoRecord::AutoRecord()
{
	dest_path = "myvioce.wav";//��ʼ��.wav����·��ΪĬ�Ϲ���·�������ļ���Ϊ��myvioce.wav��
	fp = NULL;
}

AutoRecord::~AutoRecord()
{
	vda_bos = 0; vda_eos = 0, eos_flag = 0;
}

void AutoRecord::WriteFeil(std::string Feilna)
{
	dest_path = Feilna;
}

DWORD AutoRecord::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	LPWAVEHDR pwh = (LPWAVEHDR)dwParam1;
	int temp = ARFRAGMENT_SIZE;
	if ((WIM_DATA == uMsg) && (!vda_eos))
	{
		memcpy(vda_buffer02, vda_buffer01, temp);
		memcpy(vda_buffer01, vda_buffer0, temp);
		memcpy(vda_buffer0, pwh->lpData, temp);
		memcpy(vda_buffer2, pwh->lpData, temp);
		if (!stop)//���¼��ֹͣ�򲻼�����ӻ���
		{
			waveInAddBuffer(hwi, pwh, sizeof(WAVEHDR));
		}
		if (vda_count < 10)//��ÿһ֡��ƽ����ֵ
		{
			//OutputDebugStringA("��ʼ��\n");
			vda_count++;
			for (int i = 0; i < ARFRAGMENT_SIZE / 2; i++)vda_start += abs(vda_buffer2[i]);
			if (vda_count == 10)
			{
				vda_start /= (vda_count * 4);
				/*vda_end = vda_start * 3;*/
				vda_end = vda_start * 3;
				vda_start *= sensitivity;
				/*****************�˴��ɸ���ֵ�Ĵ�С****************/
				//sprintf_s(msgbuf, "vda_start=%f\n", vda_start);
				//OutputDebugStringA(msgbuf);
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)vda_buffer1[i] = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < ARFRAGMENT_SIZE / 2 / 4; j++)
				{
					vda_buffer1[i] += abs(vda_buffer2[j + ARFRAGMENT_SIZE / 2 / 4 * i]);
				}
				/*sprintf_s(msgbuf, "buffer1[%d]=%f\n", i, buffer1[i]);
				OutputDebugStringA(msgbuf);*/
			}
			if ((vda_buffer1[0] > vda_start) && (vda_buffer1[1] > vda_start) && (vda_buffer1[2] > vda_start) && (vda_buffer1[3] > vda_start) && vda_bos == 0)
			{
				vda_bos = 1;
				OutputDebugStringA("��⵽ǰ�˵�\n\n");
				//MCIWndStop(hMCI);
				//PlaySound(NULL, NULL, NULL);
			}
			if (vda_bos)
			{
				unsigned int ls = sizeof(vda_buffer02);
				for (unsigned int i = 0; i < ls; i++) vda_buffer.push_back(vda_buffer02[i]);
				//memcpy(vda_buffer + vda_buf_count, vda_buffer02, temp);//������һ�������������ݿ�ʼ����
				vda_buf_count += temp;

				if ((vda_buffer1[0] < vda_end) && (vda_buffer1[1] < vda_end) && (vda_buffer1[2] < vda_end) && (vda_buffer1[3] < vda_end))
				{
					eos_flag++;
					if (eos_flag == 3)
					{
						vda_eos = 1;
						eos_flag = 0;
						unsigned int ls0 = sizeof(vda_buffer01);
						for (unsigned int i = 0; i < ls; i++) vda_buffer.push_back(vda_buffer01[i]);
						/*memcpy(vda_buffer + vda_buf_count, vda_buffer01, temp);*/
						OutputDebugStringA("��⵽��˵�\n\n");
					}
				}
			}
		}
	}
	return 0;
}
void AutoRecord::VDA_Initialize()
{
	stop = false;
	int nReturn = waveInGetNumDevs();//���������豸����Ŀ
									 //printf("�����豸��Ŀ��%d\n", nReturn);

									 //ʶ��������豸
	for (int i = 0; i < nReturn; i++)
	{
		WAVEINCAPS wic;  //WAVEINCAPS�ṹ����������Ƶ�����豸������
		waveInGetDevCaps(i, &wic, sizeof(WAVEINCAPS)); //waveInGetDevCaps���ܼ���һ�������Ĳ�����Ƶ�����豸������
													   //printf("#%d\t�豸����%s\n", i, wic.szPname);
	}

	wavform.wFormatTag = WAVE_FORMAT_PCM;  //WAVE_FORMAT_PCM���������
	wavform.nChannels = 1;  // ����
	wavform.nSamplesPerSec = 16000; // ����Ƶ��
	wavform.nAvgBytesPerSec = 16000 * 16 / 8;  // ÿ��������
	wavform.nBlockAlign = 2;
	wavform.wBitsPerSample = 16; // ������С
	wavform.cbSize = 0;  //��С�����ֽڣ����Ӷ���ĸ�ʽ��ϢWAVEFORMATEX�ṹ

						 //��¼���豸����
	waveInOpen(&hWaveIn, WAVE_MAPPER, &wavform, (DWORD_PTR)(AutoRecord::waveInProc), 0, CALLBACK_FUNCTION);

	//ʶ��򿪵�¼���豸
	WAVEINCAPS wic;
	waveInGetDevCaps((UINT_PTR)hWaveIn, &wic, sizeof(WAVEINCAPS));
	//printf("�򿪵������豸��%s\n", wic.szPname);

	//for (int i = 0; i < ARFRAGMENT_NUM; i++)
	//{
	//	wh[i].lpData = new char[ARFRAGMENT_SIZE];
	//	wh[i].dwBufferLength = ARFRAGMENT_SIZE;
	//	wh[i].dwBytesRecorded = 0;
	//	wh[i].dwUser = NULL;
	//	wh[i].dwFlags = 0;
	//	wh[i].dwLoops = 1;
	//	wh[i].lpNext = NULL;
	//	wh[i].reserved = 0;
	//	//Ϊ¼���豸׼�����溯����
	//	waveInPrepareHeader(hWaveIn, &wh[i], sizeof(WAVEHDR));
	//	//�������豸����һ�����棺
	//	waveInAddBuffer(hWaveIn, &wh[i], sizeof(WAVEHDR));
	//}
}

void AutoRecord::Detection()
{
	/*********************************** ¼�� *************************************/ // Device  

																				   // prepare buffer
																				   //static WAVEHDR wh[FRAGMENT_NUM];
	for (int i = 0; i < ARFRAGMENT_NUM; i++)
	{
		wh[i].lpData = new char[ARFRAGMENT_SIZE];
		wh[i].dwBufferLength = ARFRAGMENT_SIZE;
		wh[i].dwBytesRecorded = 0;
		wh[i].dwUser = NULL;
		wh[i].dwFlags = 0;
		wh[i].dwLoops = 0;
		wh[i].lpNext = NULL;
		wh[i].reserved = 0;

		//Ϊ¼���豸׼�����溯����
		waveInPrepareHeader(hWaveIn, &wh[i], sizeof(WAVEHDR));

		//�������豸����һ�����棺
		waveInAddBuffer(hWaveIn, &wh[i], sizeof(WAVEHDR));
	}


	// record  
	//��ʼ�������ݻ���
	vda_buffer.clear();
	vda_buffer.reserve(10);
	vda_eos = 0;//��ʼ��ĩ�˱�־
	vda_buf_count = 0; //�տ�ʼ¼����ʱ�򻺳����ĸ�����ʼ��Ϊ
					   //��ʼ¼������
					   //MMRESULT waveInStart(  HWAVEIN hwi  );  
	waveInStart(hWaveIn); //��ʼ¼��
	OutputDebugStringA("Start to Record...\n");
	//while (vda_buf_count < ARBUFFER_SIZE)
	while (!vda_eos)//ֱ����⵽ĩ�˶˵���������while
	{
		if (stop)
			break;
		Sleep(1);
	}

	//printf("Record Over!\n\n");

	OutputDebugStringA("Record Over!\n\n");
	Stop();

}

void AutoRecord::Stop()
{
	stop = true;
	waveInStop(hWaveIn);//waveInStop����ֹͣ�Ĳ�����Ƶ����
						//ֹͣ¼��������
						//MMRESULT waveInReset( HWAVEIN hwi ); 
	waveInReset(hWaveIn);//ֹͣ¼��
						 /*������溯����*/
						 /*MMRESULT waveInUnprepareHeader( HWAVEIN hwi,LPWAVEHDR pwh, UINT cbwh); */
	for (int i = 0; i < ARFRAGMENT_NUM; i++)
	{
		waveInUnprepareHeader(hWaveIn, &wh[i], sizeof(WAVEHDR));
		delete wh[i].lpData;
	}

}
void AutoRecord::WriteToWav()
{
	fopen_s(&fp, dest_path.c_str(), "wb");
	WavHeader.data_size = vda_buffer.size();
	WavHeader.size_8 = WavHeader.data_size + 32;
	fwrite(&WavHeader, sizeof(WavHeader), 1, fp);
	// ׷��RawData
	/*std::cout << (vda_buffer.size()) << std::endl;*/
	fwrite(vda_buffer.data(), vda_buffer.size(), 1, fp);
	//fwrite(vda_buffer.data(),vda_buffer.size(), 1, fp);
	// д�����
	fclose(fp);
	OutputDebugStringA("write to .wav over !\n\n");
}
void AutoRecord::AutoRecord_Reset()
{
	vda_buffer.clear();
	stop = false;
	vda_bos = 0, vda_eos = 0, eos_flag = 0;
}
void AutoRecord::AutoRecord_Close()
{
	//�ر�¼���豸������
	//MMRESULT waveInClose( HWAVEIN hwi );
	AutoRecord_Reset();
	waveInClose(hWaveIn);
	vda_buffer.clear();
	vda_count = 0;
}
#pragma endregion
//------------------------------------------------------



//------------------��SpecRe:public AutoRecord----------
#pragma region ��SpecRe:public AutoRecord
SpecRe::SpecRe()
{
	rsec_result[BUFFER_SIZE] = { 0 };//����ʶ��Ľ����ʼ��
	ret = MSP_SUCCESS;
	int	upload_on = 1; //�Ƿ��ϴ��û��ʱ�,1�ϴ���0���ϴ�
	login_params = "appid = , work_dir = ."; // ��¼������appid��msc���,��������Ķ�
	session_begin_params = "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = gb2312";
	//sub: ����ҵ������   domain: ����   language: ����  accent:	����  sample_rate: ��Ƶ������  result_type: ʶ������ʽ  result_encoding: ��������ʽ
}
SpecRe::~SpecRe()
{
	//MSPLogout(); //�˳���¼
	XflySR_Close();
}

void SpecRe::XflySR_Initialize()
{
	VDA_Initialize();
	rsec_result[BUFFER_SIZE] = { 0 };//����ʶ��Ľ����ʼ��
	ret = MSP_SUCCESS;
	int	upload_on = 1; //�Ƿ��ϴ��û��ʱ�,1�ϴ���0���ϴ�
	login_params = "appid = , , work_dir = ."; // ��¼������appid��msc���,��������Ķ�
	session_begin_params = "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = gb2312";
	//sub: ����ҵ������   domain: ����   language: ����  accent:	����  sample_rate: ��Ƶ������  result_type: ʶ������ʽ  result_encoding: ��������ʽ
	ret = MSPLogin(NULL, NULL, login_params); //�û���½����һ���������û������ڶ������������룬����NULL���ɣ������������ǵ�¼����	
	if (MSP_SUCCESS != ret)
	{
		OutputDebugStringA("MSPLogin failed , Error code :");
		char ssret[100] = {};
		sprintf_s(ssret, 100, "%d", ret);
		OutputDebugStringA(ssret);
		OutputDebugStringA("\n\n");
		MSPLogout(); //��¼ʧ�ܣ��˳���¼
	}
	ret = upload_userwords();//�ϴ��û�����
	if (MSP_SUCCESS != ret)//����ϴ��û�����ʧ��
		MSPLogout();//�˳���¼
}

void SpecRe :: XflySR_Close()
{
	MSPLogout();//�˳���¼
	AutoRecord_Close();//¼���ر�
	ret = MSP_SUCCESS;
	int	upload_on = 1; //�Ƿ��ϴ��û��ʱ�,1�ϴ���0���ϴ�
	login_params = "appid = , work_dir = ."; // ��¼������appid��msc���,��������Ķ�
	session_begin_params = "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = gb2312";
	//sub: ����ҵ������   domain: ����   language: ����  accent:	����  sample_rate: ��Ƶ������  result_type: ʶ������ʽ  result_encoding: ��������ʽ
}

int SpecRe::upload_userwords()
{
	char*			userwords = NULL;
	unsigned int	len = 0;
	unsigned int	read_len = 0;
	FILE*			fp = NULL;
	int				ret = -1;

	fp = fopen("userwords.txt", "rb");
	if (NULL == fp)
	{
		OutputDebugStringA("open [userwords.txt] failed! \n\n");
		goto upload_exit;
	}

	fseek(fp, 0, SEEK_END);
	len = ftell(fp); //��ȡ��Ƶ�ļ���С
	fseek(fp, 0, SEEK_SET);

	userwords = (char*)malloc(len + 1);
	if (NULL == userwords)
	{
		OutputDebugStringA("\nout of memory! \n");
		goto upload_exit;
	}

	read_len = fread((void*)userwords, 1, len, fp); //��ȡ�û��ʱ�����
	if (read_len != len)
	{
		OutputDebugStringA("read [userwords.txt] failed!\n");
		goto upload_exit;
	}
	userwords[len] = '\0';

	MSPUploadData("userwords", userwords, len, "sub = uup, dtt = userword", &ret); //�ϴ��û��ʱ�
	if (MSP_SUCCESS != ret)
	{
		char ssret[100] = {};
		sprintf_s(ssret, 100, "%d", ret);
		OutputDebugStringA(ssret);
		OutputDebugStringA("\nMSPUploadData failed ! errorCode:");
		OutputDebugStringA(ssret);
		OutputDebugStringA("\n\n");
		goto upload_exit;
	}

upload_exit:
	if (NULL != fp)
	{
		fclose(fp);
		fp = NULL;
	}
	if (NULL != userwords)
	{
		free(userwords);
		userwords = NULL;
	}

	return ret;
}

void SpecRe::run_iat(const char* audio_file, const char* session_begin_params)
{

	const char*		session_id = NULL;
	char			rec_result[BUFFER_SIZE] = { NULL };
	char			hints[HINTS_SIZE] = { NULL }; //hintsΪ�������λỰ��ԭ�����������û��Զ���
	unsigned int	total_len = 0;
	int				aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;		//��Ƶ״̬
	int				ep_stat = MSP_EP_LOOKING_FOR_SPEECH;		//�˵���
	int				rec_stat = MSP_REC_STATUS_SUCCESS;			//ʶ��״̬
	int				errcode = MSP_SUCCESS;

	FILE*			f_pcm = NULL;
	char*			p_pcm = NULL;
	long			pcm_count = 0;
	long			pcm_size = 0;
	long			read_size = 0;


	if (NULL == audio_file)
		goto iat_exit;

	f_pcm = fopen(audio_file, "rb");
	if (NULL == f_pcm)
	{
		OutputDebugStringA("\nopen userword.txt failed! \n\n");
		goto iat_exit;
	}

	fseek(f_pcm, 0, SEEK_END);
	pcm_size = ftell(f_pcm); //��ȡ��Ƶ�ļ���С 
	fseek(f_pcm, 0, SEEK_SET);

	p_pcm = (char *)malloc(pcm_size);
	if (NULL == p_pcm)
	{
		OutputDebugStringA("\nout of memory! \n\n");
		goto iat_exit;
	}

	read_size = fread((void *)p_pcm, 1, pcm_size, f_pcm); //��ȡ��Ƶ�ļ�����
	if (read_size != pcm_size)
	{
		OutputDebugStringA("read [%s] error!\n\n");
		goto iat_exit;
	}

	OutputDebugStringA("start speech re ...\n\n");
	session_id = QISRSessionBegin(NULL, session_begin_params, &errcode); //��д����Ҫ�﷨����һ������ΪNULL
	if (MSP_SUCCESS != errcode)//���������벻Ϊ0
	{
		OutputDebugStringA("QISRSessionBegin failed! \n\n");
		goto iat_exit;
	}

	while (1)
	{
		//unsigned int len = 10 * FRAME_LEN; // ÿ��д��200ms��Ƶ(16k��16bit)��1֡��Ƶ20ms��10֡=200ms��16k�����ʵ�16λ��Ƶ��һ֡�Ĵ�СΪ640Byte
		long len = 10 * FRAME_LEN;
		int ret = 0;

		if (pcm_size < 2 * len)
			len = pcm_size;
		if (len <= 0)
			break;

		aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;
		if (0 == pcm_count)
			aud_stat = MSP_AUDIO_SAMPLE_FIRST;

		//printf(">");
		ret = QISRAudioWrite(session_id, (const void *)&p_pcm[pcm_count], len, aud_stat, &ep_stat, &rec_stat);
		if (MSP_SUCCESS != ret)
		{
			OutputDebugStringA("QISRAudioWrite failed! \n\n");
			goto iat_exit;
		}

		pcm_count += (long)len;
		pcm_size -= (long)len;

		if (MSP_REC_STATUS_SUCCESS == rec_stat) //�Ѿ��в�����д���
		{
			const char *rslt = QISRGetResult(session_id, &rec_stat, 0, &errcode);
			if (MSP_SUCCESS != errcode)
			{
				OutputDebugStringA("\nQISRGetResult failed!\n\n");
				goto iat_exit;
			}
			if (NULL != rslt)
			{
				unsigned int rslt_len = strlen(rslt);
				total_len += rslt_len;
				if (total_len >= BUFFER_SIZE)
				{
					OutputDebugStringA("no enough buffer for rec_result !\n\n");
					goto iat_exit;
				}
				strncat(rec_result, rslt, rslt_len);
			}
		}

		if (MSP_EP_AFTER_SPEECH == ep_stat)
			break;
		Sleep(1); //ԭΪ200ģ����˵��ʱ���϶��200ms��Ӧ10֡����Ƶ
	}
	errcode = QISRAudioWrite(session_id, NULL, 0, MSP_AUDIO_SAMPLE_LAST, &ep_stat, &rec_stat);
	if (MSP_SUCCESS != errcode)
	{
		OutputDebugStringA("QISRAudioWrite failed! \n\n");
		goto iat_exit;
	}

	while (MSP_REC_STATUS_COMPLETE != rec_stat)
	{
		const char *rslt = QISRGetResult(session_id, &rec_stat, 0, &errcode);
		if (MSP_SUCCESS != errcode)
		{
			OutputDebugStringA("QISRGetResult failed, error code: \n\n");
			goto iat_exit;
		}
		if (NULL != rslt)
		{
			unsigned int rslt_len = strlen(rslt);
			total_len += rslt_len;
			if (total_len >= BUFFER_SIZE)
			{
				OutputDebugStringA("no enough buffer for rec_result \n\n");
				goto iat_exit;
			}
			strncat(rec_result, rslt, rslt_len);
		}
		Sleep(150); //��ֹƵ��ռ��CPU
	}
	OutputDebugStringA("speech re over \n\n");
	
	//printf("%s\n", rec_result);//������Ըģ��ĳɺ��������ַ�������
	if (rec_result[0] == '\0') strcpy_s(rec_result, "��Ǹ����û˵�����");
	strcpy_s(rsec_result, rec_result);//��ʶ����������rsec_result
iat_exit:
	if (NULL != f_pcm)
	{
		fclose(f_pcm);
		f_pcm = NULL;
	}
	if (NULL != p_pcm)
	{
		free(p_pcm);
		p_pcm = NULL;
	}

	QISRSessionEnd(session_id, hints);
	memset(rec_result, 0, sizeof(rec_result));
}

void SpecRe::RecoSpere()
{
	const char* feil_name = dest_path.data();//���¼���ļ���
	run_iat(feil_name, session_begin_params);//ʶ�𲢽�ʶ��������rsec_result[BUFFER_SIZE] 
}

char* SpecRe::GetResult()
{
	return rsec_result;
}
#pragma endregion
//------------------------------------------------------