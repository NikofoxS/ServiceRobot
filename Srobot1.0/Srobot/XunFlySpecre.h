#pragma once
#ifndef _XunFlySpecre_H_
#define _XunFlySpecre_H_
//#define _CRT_SECURE_NO_WARNINGS//�ر�windows����
//------------------¼��------------------------
#pragma region ¼��ͷ�ļ��ͺ�
#include <stdlib.h>
#include <cmath>
#include <stdio.h> 
#include <windows.h>  
#include <mmsystem.h>  
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msc.lib")

#include<string>
#include<vector>

#define ARBUFFER_SIZE (16000*16/8*30)    // ¼����������
#define ARFRAGMENT_SIZE 1024              // ��������С
#define ARFRAGMENT_NUM 2                  // ���������� 
#pragma endregion
//--------------------------------------------

//------------------Ѷ��------------------------
#pragma region Ѷ��ͷ�ļ��ͺ�
//#include <stdlib.h>
//#include <stdio.h>
//#include <windows.h>
#include <conio.h>
#include <errno.h>


#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"



#define	BUFFER_SIZE	4096
#define FRAME_LEN	640 
#define HINTS_SIZE  100
#pragma endregion
//---------------------------------------------


//static unsigned char vda_buffer[BUFFER_SIZE] = { 0 };
class AutoRecord//¼����
{
#pragma region ����
public://---------------------------------------------------------------
	AutoRecord();
	virtual ~AutoRecord();
	static std::vector<unsigned char> vda_buffer;//�ɼ�������Ƶ�Ļ�����
	static unsigned char vda_buffer0[ARFRAGMENT_SIZE];
	static unsigned char vda_buffer01[ARFRAGMENT_SIZE];
	static unsigned char vda_buffer02[ARFRAGMENT_SIZE];
	static short vda_buffer2[ARFRAGMENT_SIZE / 2];
	static float vda_buffer1[4];
	static int vda_buf_count;
	static int vda_count;
	static volatile int vda_bos, vda_eos, eos_flag;//vda_bosΪ�˵��׶˱�־ vda_eosΪ�˵�ĩ�˱�־
	static float vda_start, vda_end;
	static int sensitivity;
	static volatile bool stop;//�Ƿ񴥷�ֹͣ
	std::string dest_path;								// �洢·��
	static WAVEHDR wh[ARFRAGMENT_NUM];

	HWAVEIN hWaveIn;//������Ƶ���ݸ�ʽWave_audio���ݸ�ʽ
	WAVEFORMATEX wavform;//WAVEFORMATEX�ṹ�����˲�����Ƶ���ݸ�ʽ������������ṹ��Ψһ�ĸ�ʽ��Ϣ����ͬ���в�����Ƶ���ݸ�ʽ��������Ҫ�������Ϣ�ĸ�ʽ������ṹ��������һ���ṹ�ĵ�һ����Ա���Լ���������Ϣ

	void VDA_Initialize();////////////////////==============¼����ʼ������=============///////////////////////////
	static DWORD(CALLBACK waveInProc)(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);//¼���ص�����
	void WriteFeil(std::string Feilna);
	void Detection();//�Զ�¼��
	void Stop();//ֹͣ¼��
	void WriteToWav();//����Ϊ.wav
	void AutoRecord_Reset();//��λ����
	void AutoRecord_Close();//�ر��豸
#pragma endregion

private://-------------------------------------------------------------
#pragma region ˽��
	typedef struct WAVEPCMHDR
	{
		char            riff[4];					// = "RIFF"
		UINT32			size_8;						// = FileSize - 8
		char            wave[4];					// = "WAVE"
		char            fmt[4];						// = "fmt "
		UINT32			fmt_size;					// = PCMWAVEFORMAT�Ĵ�С : 
													//PCMWAVEFORMAT
		UINT16	        format_tag;					// = PCM : 1
		UINT16	        channels;					// = ͨ���� : 1
		UINT32			samples_per_sec;			// = ������ : 8000 | 6000 | 11025 | 16000
		UINT32			avg_bytes_per_sec;			// = ÿ��ƽ���ֽ��� : samples_per_sec * bits_per_sample / 8
		UINT16		    block_align;				// = ÿ�������ֽ��� : wBitsPerSample / 8
		UINT16			bits_per_sample;			// = ��������: 8 | 16
		char            data[4];					// = "data";
													//DATA
		UINT32			data_size;					// = �����ݳ��� 
	} WAVEPCMHDR;
	/* Ĭ��wav��Ƶͷ������ */
	WAVEPCMHDR WavHeader =
	{
		{ 'R', 'I', 'F', 'F' },
		0,
		{ 'W', 'A', 'V', 'E' },
		{ 'f', 'm', 't', ' ' },
		sizeof(PCMWAVEFORMAT) ,
		WAVE_FORMAT_PCM,
		1,
		16000,
		16000 * (16 / 8),
		16 / 8,
		16,
		{ 'd', 'a', 't', 'a' },
		0
	};
	/* wav��Ƶ�����ݷ��ڹ��б��� */
	FILE* fp;								// wave�ļ�ָ��

};
#pragma endregion

class SpecRe:public AutoRecord//Ѷ������ʶ�����Զ�¼����
{
public:
	SpecRe();
	~SpecRe();
	void RecoSpere();//����ȡ��¼���ϴ�Ѷ��ʶ�� ��Ҫ����¼��
	void XflySR_Initialize();//==============Ѷ��ʶ���ʼ������=============
	void XflySR_Close();//=============Ѷ��ʶ��ر�================
	int upload_userwords();//�ϴ��û��ʱ�
	void run_iat(const char* audio_file, const char* session_begin_params);//Ѷ������ʶ������
	char* GetResult();

	int ret;//��½�Ƿ�ɹ��ı�־
	int upload_on;//�ϴ��û��ʱ�ı�־
	const char* session_begin_params;//��������ʶ��Ĺ�������
	char rsec_result[BUFFER_SIZE] ;//����ʶ��Ľ��
private:
	const char* login_params;//��¼������appid��msc���,��Ҫ����Ķ�
	
};

#endif