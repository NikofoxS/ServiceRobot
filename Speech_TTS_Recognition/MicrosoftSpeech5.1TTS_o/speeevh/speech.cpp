#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>
#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance��Ҫ����ole32.dll
#pragma comment(lib,"sapi.lib")    //sapi.lib��SDK��libĿ¼,������ȷ����

#include<iostream>

int main()
{
	::CoInitialize(NULL);//��ʼ����������
	ISpVoice * pSpVoice = NULL;//��ʼ����������
	CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice);
		//���������������������൱�ڴ�������������FAILED�Ǹ��궨�壬�������ж�CoCreateInstance���������û�гɹ��������������������ǲ��ɹ�����ʾ��Ϣ��

	pSpVoice->Speak(L"speek english��һλ����Ů�赸�Ҹ�������д��һ������������ţ����н��飺�����������飬�ǽ��Ժ��������ѧ���Ǽ����¡�������ָ�������������������������������ǻۺ�����������ò�����ж�ô������������ڻ����б�ʾ���ܽ����ⷬ���⣬��˵�����Ǹ��������ֻ������������ò�����������ǻۣ��Ͳ�͸�ˡ�", SPF_DEFAULT, NULL);//ִ������������Speek����������������������֡�
	pSpVoice->Release(); //�ͷ���������
	::CoUninitialize();//�ͷ���������
	return 0;
}