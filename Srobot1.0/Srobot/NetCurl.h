#pragma once
#ifndef _NetCurl_H
#define _NetCurl_H
//#define _CRT_SECURE_NO_WARNINGS
//�ڹ������Ҽ�-������-��c/c++-����������-�����п�  ѡ��MTD
// �һ���Ŀ->����->��������->���棬Ȼ�����ұߵġ���ĿĬ��ֵ���еġ�MFC��ʹ�á�ѡ����ѡ���ھ�̬����ʹ��MFC��
#include "curl/curl.h"
#include <iostream>
#include "json//json.h"    
#pragma comment(lib, "json_vc71_libmtd.lib")    
#pragma comment(lib, "libcurld.lib")

class NetCurl
{
public:
	NetCurl();
	virtual ~NetCurl();
#pragma region ��������ת��

#pragma region �⼸��������Ҫ�ֶ��ͷŷ���ֵ�����ڴ��ַ
	char* ANSItoUTF8(const char* strAnsi);
	char* UTF8toANSI(const char* strUTF8);
	char* EncodeToUTF8(const char* mbcsStr);
#pragma endregion

//////////////////��������ת�����⼸������Ҫ�ֶ��ͷŷ���ֵ�����ڴ��ַ
	std::wstring AsciiToUnicode(const std::string& str);
	std::string UnicodeToUtf8(const std::wstring& wstr);
	std::string AsciiToUtf8(const std::string& str);
	std::string UTF8ToUnicode(const char* mbcsStr);

	std::wstring StringToWstring(const std::string str);
	std::string WstringToString(const std::wstring str);
/////////////////////////////////////////////////////////////
#pragma endregion
	int curlpost(std::string urlofpost,Json::Value rec_result);//ʹ��curl����post,��һ������Ϊpost��url,�ڶ�������Ϊpost��body�˴�ӦΪjson��ʽ��string,�趨post��bodyΪutf8
	Json::Value GetPostResult();//��ȡpost����Json���
	bool stopcallback;//�ص�����ֹͣ�ı�־

private:
	static bool callbackflag;//�ص���ɵı�־����ȡ��PostResult��Ӧ�ֶ���callbackflag��Ϊfalse
	static size_t postcallback(void *ptr, size_t size, size_t nmemb, void *stream);//curl�ص���������ͨ�Ž���post������Ϣ
	static Json::Value PostResult;//post�ص�����body�����ص����PostResult
};

class TulingRobot :public NetCurl//ͼ�����webapi������
{
public:
	//TulingRobot() :key(""), userid(""), tulingapi("http://www.tuling123.com/openapi/api"){}//���캯����ʼ���б�const��ֵ
	TulingRobot();
	~TulingRobot();
	void InputText(std::string mytext);//�����ͼ�������post��body����
	void PostToTuling();//��ͼ������˷���post����
	std::string strmyword;//��Ӧ��myword��string��ʽ
	std::string TulingResult();//��ȡͼ������˻ظ���Ϣ
private:
	/*const */std::string key;//��Ҫ�ύ��key
	/*const */std::string userid;//�ύ���û�id
	/*const */std::string tulingapi;//ͼ��api�����ַ��
};

class Facepp :public NetCurl//face++��webapi������
{
public:
	Facepp();
	~Facepp();

	std::string GetFaPostRe();//��ȡ������������з��ؽ��

	//�����������ϵͳά�����һ��ʹ�ã�����һ�������ļ��� FaceSet�����ڴ洢������ʶ face_token
	bool FaceSetCreat(std::string FaSetLogo); //FaSetLogo���õ�FaceSet�Զ����ʶ

	bool FGetFaceSets();//��ȡĳһ API Key �µ� FaceSet �б��� faceset_token��outer_id��display_name �� tags ����Ϣ��

	//ɾ��һ����������(����ά��ʱʹ��)
	bool FaceSetDelete(std::string DFaceSetToken);//DFaceSetToken��Ҫɾ����FaceSet�ı�ʶ

	//����ͼƬ�����������������������ظ�������facetoken
	bool FaceDetect(std::string FFileName);//FFileName��post���ļ�·�������ļ����ͺ�׺
	std::string FDetectToken();//��ȡ������������facetoken
	static bool ErroFlag;//post����ֵ�ص��Ƿ�ʧ�ܱ�־��ûʧ�ܷ���true

	//Ϊһ���Ѿ������� FaceSet ���������ʶ face_token
	bool FSetAddFace(std::string FtokenToAd, std::string FaceSetId);//FtokenToAd��Ҫ������������ʶ��facetoken,FaceSetIdΪfacetokenҪ�����faceset���趨��id

	//��ȡһ�� FaceSet ��������Ϣ
	bool FSetDetail(std::string FSetTokenToGet);//FSetTokenToGet��Ҫ��ȡ��FaceSet�ı�ʶtoken

	//�Ƴ�һ��FaceSet�е�ȫ��face_token
	bool RemoveFFaSet(std::string RemoveFFSetToken);//RemoveFFSetToken��Ҫ�Ƴ�ȫ��face_token��FaceSet�ı�ʶfaceset_token

	////��һ�����е�FaceSet���ҳ���Ŀ�����������Ƶ�һ����������facetoken
	bool SearchFFSet(std::string FFNaToSe, std::string SeaFaSetLogo);//FFNaToSe�������ͼƬλ�ã������ļ����ͺ�׺��,SeaFaSetLogo;//������FaceSet�����Զ����ʶ,
	std::string GetSearchRe();//��ȡ����Seach�����û��Զ����������ʶ

	//�趨������ʶfacetoken���Զ����ʶ��Ϣ
	bool FaceTokenID(std::string FaceToken,std::string UserId );//FaceToken��Ҫ�������Զ����ʶ��facetoken,UserId��Ҫ�趨������token�е��Զ����ʶ

private:
	std::string FaceppKey;//facepp��key
	std::string FaceppSecret;//facepp��secret
	static std::string FaPostResult;//������������з���ֵ

	std::string FSetCreatUrl;//����FaceSet��Url
	static size_t FSetCreatCallback(void *ptr, size_t size, size_t nmemb, void *stream);//�ص�����������һ�������ļ��� FaceSet�����ڴ洢������ʶ face_token

	std::string FGetFaceSetUrl;//��ȡĳһ API Key �µ� FaceSet �б��� faceset_token��outer_id��display_name �� tags ����Ϣ��Url
	static size_t FGetFSetCallback(void *ptr, size_t size, size_t nmemb, void *stream);

	std::string FSetDeleteUrl;//ɾ��һ�������ϼ�FaceSet��Url;
	static size_t FSetDeleteCallback(void *ptr, size_t size, size_t nmemb, void *stream);

	static std::string FaceToken;//���ε�FaceToken
	std::string FaceppDetectUrl;//����ͼƬ����������������������url
	static size_t FDetectCallback(void *ptr, size_t size, size_t nmemb, void *stream);//Facepp��curl�ص���������ͨ�Ž���post������Ϣ

	std::string FSetAddFaceUrl;//Ϊһ���Ѿ������� FaceSet ���������ʶ face_token��Url
	static size_t FSetAddFaCallback(void *ptr, size_t size, size_t nmemb, void *stream);//ΪΪһ���Ѿ������� FaceSet ���������ʶ face_token�Ļص�����

	std::string FSetDetailUrl;//��ȡһ�� FaceSet ��������Ϣ��Url
	static size_t FSetDetailCallback(void *ptr, size_t size, size_t nmemb, void *stream);//��ȡһ�� FaceSet ��������Ϣ�Ļص�����

	std::string RemoveFFaSetUrl;//�Ƴ�һ��FaceSet�е�ȫ��face_token��Url
	static size_t RemoveFFaSetCallback(void *ptr, size_t size, size_t nmemb, void *stream);//�Ƴ�һ��FaceSet�е�ȫ��face_token�Ļص�����

	static std::string LogoFace;//����search�����Զ����ʶ��Ϣ
	std::string SearchFFSetUrl;//��һ�����е�FaceSet���ҳ���Ŀ�����������Ƶ�һ��������Url
	static size_t SearchFFSetCallback(void *ptr, size_t size, size_t nmemb, void *stream);//��һ�����е�FaceSet���ҳ���Ŀ�����������Ƶ�һ�������Ļص�����

	std::string FaceTokenIDUrl;//�趨������ʶfacetoken���Զ����ʶ��Ϣ��Url
	static size_t FaceTokenIDCallback(void *ptr, size_t size, size_t nmemb, void *stream);//(���趨Ϊ������˴�����ʱ��Ҫ�û���NetCurl��UTF8ToUnicodeת����ʾ)�趨������ʶfacetoken���Զ����ʶ��Ϣ�Ļص�����
};

#endif // !_NetCurl_H

//TulingResult������whileѭ��������