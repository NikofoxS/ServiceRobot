//#include <sphelper.h>//����ͷ�ļ�
//#include <iostream>//C++ͷ�ļ���������ʾ������Ϣ
//
//int main()
//{
//	::CoInitialize(NULL);//��ʼ����������
//	ISpVoice * pSpVoice = NULL;//��ʼ����������
//	if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice)))
//		//���������������������൱�ڴ�������������FAILED�Ǹ��궨�壬�������ж�CoCreateInstance���������û�гɹ��������������������ǲ��ɹ�����ʾ��Ϣ��
//	{
//		std::cout << "Failed to create instance of ISpVoice!" << std::endl;
//		return -1;
//	}
//
//	pSpVoice->Speak(L"��ã����������Ѱ�", SPF_DEFAULT, NULL);//ִ������������Speek����������������������֡�
//	pSpVoice->Release(); //�ͷ���������
//	::CoUninitialize();//�ͷ���������
//	return 0;
//}