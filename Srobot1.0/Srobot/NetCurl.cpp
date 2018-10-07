#define _CRT_SECURE_NO_WARNINGS
#include"stdafx.h"
#include"NetCurl.h"


#pragma region ��libcurl����ͨ�ŵ�NetCurl��
////��̬������ʼ��
bool NetCurl::callbackflag = false;
Json::Value NetCurl::PostResult = NULL;//post�ص�����body�����ص����PostResult

NetCurl::NetCurl()
{
	callbackflag = false;
	stopcallback = false;
}

NetCurl::~NetCurl()
{
	callbackflag = false;
	stopcallback = true;
}

#pragma region ��Щת������ֵʹ�ú���Ҫ�ֶ��ͷ��ڴ�
//UTF8תANSI
char* NetCurl::UTF8toANSI(const char* strUTF8)
{
	//��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�����
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 2];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;

	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;

	strUTF8 = szBuffer;
	//�����ڴ�
	delete[]wszBuffer;
	return szBuffer;
}
std::string NetCurl::UTF8ToUnicode(const char* mbcsStr)//UFT8תUnicode
{
	wchar_t*  wideStr;
	char*   unicodeStr;
	int   charLen;

	charLen = MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, NULL, 0);
	wideStr = (wchar_t*)malloc(sizeof(wchar_t)*charLen);
	MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, wideStr, charLen);

	charLen = WideCharToMultiByte(936, 0, wideStr, -1, NULL, 0, NULL, NULL);
	unicodeStr = (char*)malloc(charLen);
	WideCharToMultiByte(936, 0, wideStr, -1, unicodeStr, charLen, NULL, NULL);
	
	std::string unmbcsStr;
	unmbcsStr = unicodeStr;

	free(wideStr);
	free(unicodeStr);
	return unmbcsStr;
}

//ANSIתUTF8
char* NetCurl::ANSItoUTF8(const char* strAnsi)
{
	//��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�������936Ϊ��������GB2312����ҳ
	UINT nLen = MultiByteToWideChar(936, NULL, strAnsi, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(936, NULL, strAnsi, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	//��ȡתΪUTF8���ֽں���Ҫ�Ļ�������С���������ֽڻ�����
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;

	strAnsi = szBuffer;
	//�ڴ�����
	delete[]wszBuffer;
	return szBuffer;
}

char* NetCurl::EncodeToUTF8(const char* mbcsStr)//UnicodeתUTF8
{
	wchar_t*  wideStr;
	char*   utf8Str;
	int   charLen;

	charLen = MultiByteToWideChar(936, 0, mbcsStr, -1, NULL, 0);    ////////936 ----- gb2312
	wideStr = (wchar_t*)malloc(sizeof(wchar_t)*charLen);
	MultiByteToWideChar(936, 0, mbcsStr, -1, wideStr, charLen);

	charLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);

	utf8Str = (char*)malloc(charLen);

	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, charLen, NULL, NULL);

	free(wideStr);
	return utf8Str;

}
#pragma endregion

std::wstring NetCurl::AsciiToUnicode(const std::string& str)//stringתwstring(asciiתUnicode)
{
	// Ԥ��-�������п��ֽڵĳ���  
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	// ��ָ�򻺳�����ָ����������ڴ�  
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
	// ��ʼ�򻺳���ת���ֽ�  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
	std::wstring ret_str = pUnicode;
	free(pUnicode);
	return ret_str;
}

std::string NetCurl::UnicodeToUtf8(const std::wstring& wstr)
{
	// Ԥ��-�������ж��ֽڵĳ���  
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// ��ָ�򻺳�����ָ����������ڴ�  
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// ��ʼ�򻺳���ת���ֽ�  
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}


std::string NetCurl::AsciiToUtf8(const std::string& str)
{
	return UnicodeToUtf8(AsciiToUnicode(str));
}


std::wstring NetCurl::StringToWstring(const std::string str)//wstring.c_str����Ϊconst wchar_t*
{// stringתwstring
	unsigned len = str.size() * 2;// Ԥ���ֽ���
	setlocale(LC_CTYPE, "");     //������ô˺���
	wchar_t *p = new wchar_t[len];// ����һ���ڴ���ת������ַ���
	mbstowcs(p, str.c_str(), len);// ת��
	std::wstring str1(p);
	delete[] p;// �ͷ�������ڴ�
	return str1;
}

std::string NetCurl::WstringToString(const std::wstring str)//string.c_str()Ϊconst char*
{// wstringתstring
	unsigned len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char *p = new char[len];
	wcstombs(p, str.c_str(), len);
	std::string str1(p);
	delete[] p;
	return str1;
}

int NetCurl::curlpost(std::string urlofpost,Json::Value rec_result)//ʹ��curl����post,��һ������Ϊpost��url,�ڶ�������Ϊpost��body�˴�ӦΪjson��ʽ��string
{
	CURL *curl;
	CURLcode result_code;
	struct curl_slist *headers = NULL;
	int error_code = 0;
	//rec_result = ANSItoUTF8(rec_result);
	curl = curl_easy_init();//curl��ʼ��
	headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, urlofpost.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POST, 1);//����Ϊ��0��ʾ���β���ΪPOST
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, rec_result);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sizeof(rec_result));//�����ϴ�json������,������ÿ��Ժ���
		std::string access_token_result;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, postcallback);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			return 1;
		}
		curl_easy_cleanup(curl);//����curl�ڴ�
		error_code = 0;
	}
	else 
	{
		fprintf(stderr, "curl_easy_init() failed.");
		error_code = 1;
	}
	return error_code;
}

/*static */size_t NetCurl::postcallback(void *ptr, size_t size, size_t nmemb, void *stream)//curl�ص���������ͨ�Ž���post������Ϣ
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string s((char *)ptr, size * nmemb);
	std::string* access_token_result = static_cast<std::string*>(stream);
	// ��ʼ��ȡjson�е�access token��Ŀ
	Json::Reader reader;
	/*Json::Value root;*/
	// ʹ��boost�����json
	if (reader.parse(s, PostResult))
	{
		//bool aaa;
		//aaa = (reader.parse(s, PostResult));
		//std::cout << aaa << std::endl;

		//Json::FastWriter fast_writer;
		//std::cout << fast_writer.write(root) << std::endl;

		//std::string aaa = root["text"].asString();
		//char* turing_back;
		//turing_back = UTF8ToEncode(aaa.c_str());

		//std::cout << "ͼ��ظ���" << turing_back << std::endl;
		//free(turing_back);
		callbackflag = true;//��־post�ص�����body������������ֵ��PostResult
	}

	return size * nmemb;
}

Json::Value NetCurl::GetPostResult()
{
	Json::Value gettemp;
	while (callbackflag != true)
	{
		if (stopcallback = true)
			break;
		Sleep(1);
	}
	gettemp = PostResult;
	PostResult = NULL;
	callbackflag = false;
	stopcallback = false;
	return gettemp;
}
#pragma endregion


#pragma region ��ͼ������˷���post���󲢽��յ���
TulingRobot::TulingRobot()
{
	key = "";
	userid = "";
	tulingapi = "http://www.tuling123.com/openapi/api";
	//myword = NULL;
	strmyword = "";
}

TulingRobot::~TulingRobot()
{
	OutputDebugStringA("~TulingRobot over\n\n");
}
void TulingRobot::InputText(std::string mytext)
{
	Json::Value myword;//���ڱ�ʾ��tulingapi����post��body
	std::string utf8key = AsciiToUtf8(key);
	myword[AsciiToUtf8("key")] = Json::Value(utf8key);
	std::string utf8info = AsciiToUtf8(mytext);
	myword[AsciiToUtf8("info")] = Json::Value(utf8info);
	std::string utf8userid = AsciiToUtf8(userid);
	myword[AsciiToUtf8("userid")] = Json::Value(utf8userid);
	Json::FastWriter fast_writer;
	strmyword = fast_writer.write(myword);//��Json��ʽ��mywordת��Ϊstring��ʽ
}

void TulingRobot::PostToTuling()
{
	curlpost(tulingapi,strmyword);//��ͼ������˷���post������bodyΪstrmyword
}

std::string TulingRobot::TulingResult()
{
	std::string poresult;
	Json::Value tempresult;
	tempresult = GetPostResult();

	std::string retext;
	std::string textofre;
	textofre = tempresult["text"].asString();
	retext = UTF8ToUnicode(textofre.c_str());
	return retext;

}
#pragma endregion


#pragma region ��face++�����������
/////////static�������������ʼ��////////
std::string Facepp::FaceToken = "";
bool Facepp::ErroFlag = false;
std::string Facepp::FaPostResult = "";
std::string Facepp::LogoFace = "";
//////////////////////////////////////

Facepp::Facepp()
{
	FaceppKey = "";//��ʽ���Ѱ�
	FaceppSecret = "";//��ʽ���Ѱ�
	//FaceppKey = "";//������ð�
	//FaceppSecret = "";//������ð�
	FaceppDetectUrl = "https://api-cn.faceplusplus.com/facepp/v3/detect";
	FSetCreatUrl = "https://api-cn.faceplusplus.com/facepp/v3/faceset/create";
	FGetFaceSetUrl = "https://api-cn.faceplusplus.com/facepp/v3/faceset/getfacesets";
	FSetDeleteUrl = "https://api-cn.faceplusplus.com/facepp/v3/faceset/delete";
	FSetAddFaceUrl = "https://api-cn.faceplusplus.com/facepp/v3/faceset/addface";
	FSetDetailUrl = "https://api-cn.faceplusplus.com/facepp/v3/faceset/getdetail";
	RemoveFFaSetUrl = "https://api-cn.faceplusplus.com/facepp/v3/faceset/removeface";
	SearchFFSetUrl = "https://api-cn.faceplusplus.com/facepp/v3/search";
	FaceTokenIDUrl = "https://api-cn.faceplusplus.com/facepp/v3/face/setuserid";
}

Facepp::~Facepp()
{
	OutputDebugStringA("~Facepp over\n\n");
}

std::string Facepp::GetFaPostRe()
{
	std::string TeFaPostResult;
	TeFaPostResult = FaPostResult;
	FaPostResult = "";
	return TeFaPostResult;
}

size_t Facepp::FSetCreatCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	////--------��������ֵ---------------------------------
	Json::StyledWriter style_write;
	reader.parse(receive, root);
	/*std::cout << style_write.write(root) << std::endl;*/
	FaPostResult = style_write.write(root);
	////-------------------------------------------------
	if (!root["faceset_token"].isNull())ErroFlag = true;//ֻ�гɹ�����������faceset_token��Ϊtrue
	else ErroFlag = false;

	return size * nmemb;
}

bool Facepp::FaceSetCreat(std::string FaSetLogo)
{
	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��ֻ�гɹ�����������faceset_token��Ϊtrue
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FSetCreatCallback);//post���ؽ���Ļص�����

	/*wsprintfA(buf, "%ls", Filename);*/
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "outer_id",
		CURLFORM_PTRCONTENTS, FaSetLogo.c_str(), CURLFORM_END);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, FSetCreatUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	return ErroFlag;
}

size_t Facepp::FGetFSetCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	////--------------------��ȡ��������ֵ-----------------
	Json::StyledWriter style_write;
	/*reader.parse(receive, root);
	std::cout << style_write.write(root) << std::endl;*/
	////-------------------------------------------------

	if (reader.parse(receive, root))//�����ؽ����Ϊ��
	{
		/*ErroFlag = true;*/
		FaPostResult = style_write.write(root);//��ȡ��������ֵ����ֵ
		Json::Value arrayObj;

		if (!root["facesets"].isNull())
		{
			arrayObj = root["facesets"];
			for (unsigned int i = 0; i < arrayObj.size(); i++)
			{
				if (!arrayObj[i]["faceset_token"].isNull())
				{
					//*access_token_result = arrayObj[i]["face_token"].asString();
					/*FaceToken = arrayObj[i]["face_token"].asString();*/
					ErroFlag = true;//ֻ�гɹ���ѯ��������faceset_token��Ϊtrue
				}
				else ErroFlag = false;
			}
		}
		else ErroFlag = false;
	}
	else ErroFlag = false;
	return size * nmemb;
}

bool Facepp::FGetFaceSets()
{
	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��ֻ�гɹ���ѯ��������faceset_token��Ϊtrue
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FGetFSetCallback);//post���ؽ���Ļص�����

																	 /*wsprintfA(buf, "%ls", Filename);*/
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, FGetFaceSetUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	return ErroFlag;
}

size_t Facepp::FSetDeleteCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	////------------------��ȡ��������ֵ-----------------
	Json::StyledWriter style_write;
	reader.parse(receive, root);
	//std::cout << style_write.write(root) << std::endl;
	FaPostResult = style_write.write(root);
	////-------------------------------------------------
	if (!root["faceset_token"].isNull())ErroFlag = true;//ֻ�гɹ�����������faceset_token��Ϊtrue
	else ErroFlag = false;

	return size * nmemb;
}

bool Facepp::FaceSetDelete(std::string DFaceSetToken)
{
	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��ֻ�гɹ���ѯ��������faceset_token��Ϊtrue
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FSetDeleteCallback);//post���ؽ���Ļص�����

																	/*wsprintfA(buf, "%ls", Filename);*/
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "faceset_token",
		CURLFORM_PTRCONTENTS, DFaceSetToken.c_str(), CURLFORM_END);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, FSetDeleteUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	return ErroFlag;
}

size_t Facepp::FDetectCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	//long a = GetCurrentThreadId();//������̵߳�id
	//std::cout << a << std::endl;

	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	////--------------------��ȡ��������ֵ-----------------
	Json::StyledWriter style_write;
	//reader.parse(receive, root);
	//std::cout << style_write.write(root) << std::endl;
	////-------------------------------------------------

	if (reader.parse(receive, root))//�����ؽ����Ϊ��
	{
		/*ErroFlag = true;*/
		FaPostResult = style_write.write(root);//��������ֵ��ֵ
		Json::Value arrayObj;

		if (!root["faces"].isNull())
		{
			arrayObj = root["faces"];
			for (unsigned int i = 0; i < arrayObj.size(); i++)
			{
				if (!arrayObj[i]["face_token"].isNull())
				{
					//*access_token_result = arrayObj[i]["face_token"].asString();
					FaceToken = arrayObj[i]["face_token"].asString();
					ErroFlag = true;//ֻ�гɹ���⵽����������face_token��Ϊtrue
				}
				else ErroFlag = false;
			}
		}
		else ErroFlag = false;
	}
	else ErroFlag = false;
	return size * nmemb;
}


bool Facepp::FaceDetect(std::string FFileName)
{
	//long a = GetCurrentThreadId();//������̵߳�id���ĵ������post�Ļص�����һ����
	//std::cout << a << std::endl;

	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��ֻ�гɹ�post��⵽���������ظ�������face_token��Ϊtrue
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;
	/*static char buf[1024] = "Content-Type:multipart/form-data;charset=UTF-8";*/

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	//std::string access_token_result;
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FDetectCallback);//post���ؽ���Ļص�����

	/*wsprintfA(buf, "%ls", Filename);*/

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "image_file",
		CURLFORM_FILE, FFileName.c_str(), CURLFORM_END);

	//curl_formadd(&post,
	//	&last,
	//	CURLFORM_COPYNAME, "return_attributes",
	//	CURLFORM_COPYCONTENTS, "gender,age", CURLFORM_END);


	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, FaceppDetectUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	/*Gender_value = m_CURL.Gender_value;*/
	/*face_token = m_CURL.Face_Token.c_str();*/
	/*OutputDebugStringA("\n�������Ա�");
	OutputDebugStringA(Gender_value.c_str());*/
	//Face_Token = access_token_result;
	//OutputDebugStringA(access_token_result.data());
	//OutputDebugStringA(facetoken);
	/*return m_CURL.face_err;*/
	return ErroFlag;
}

std::string Facepp::FDetectToken()
{
	std::string tfacetoken;
	tfacetoken = FaceToken;
	FaceToken = "";
	return tfacetoken;
}

size_t Facepp::FSetAddFaCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	////------------------��ȡ��������ֵ-----------------
	Json::StyledWriter style_write;
	reader.parse(receive, root);
	//std::cout << style_write.write(root) << std::endl;
	FaPostResult = style_write.write(root);
	////-------------------------------------------------
	if (!root["face_added"].isNull())
	{
		int face_add = root["face_added"].asInt();
		if(face_add!=0)ErroFlag = true;//ֻ�м�������һ��face_token��Ϊtrue
		else ErroFlag = false;
	}
	else ErroFlag = false;

	return size * nmemb;
}

bool Facepp::FSetAddFace(std::string FtokenToAd, std::string FaceSetId)
{
	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��ֻ�б��γɹ����벻Ϊ���face_token��Ϊtrue������facetoken�Ѽ����򷵻��㣩
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FSetAddFaCallback);//post���ؽ���Ļص�����

																	  /*wsprintfA(buf, "%ls", Filename);*/
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "outer_id",
		CURLFORM_PTRCONTENTS, FaceSetId.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "face_tokens",
		CURLFORM_PTRCONTENTS, FtokenToAd.c_str(), CURLFORM_END);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, FSetAddFaceUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	return ErroFlag;
}

size_t Facepp::FSetDetailCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	Json::StyledWriter style_write;
	reader.parse(receive, root);
	//std::cout << style_write.write(root) << std::endl;
	/*FaPostResult = style_write.write(root);*/
	////-------------------------------------------------
	if (reader.parse(receive, root))//�����ؽ����Ϊ��
	{
		/*ErroFlag = true;*/
		FaPostResult = style_write.write(root);//��������ֵ��ֵ
		Json::Value arrayObj;

		if (!root["face_tokens"].isNull())
		{
			arrayObj = root["face_tokens"];
			for (unsigned int i = 0; i < arrayObj.size(); i++)
			{
				if (!arrayObj[i].isNull())
				{
					//*access_token_result = arrayObj[i]["face_token"].asString();
					/*FaceToken = arrayObj[i].asString();*/
					ErroFlag = true;//ֻ�гɹ���ѯ����FaceSet����face_tokens�����ز�Ϊtrue
				}
				else ErroFlag = false;
			}
		}
		else ErroFlag = false;
	}
	else ErroFlag = false;

	return size * nmemb;
}

bool Facepp::FSetDetail(std::string FSetTokenToGet)
{
	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��ֻ�гɹ���ѯ����FaceSet����face_tokens�����ز�Ϊtrue
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FSetDetailCallback);//post���ؽ���Ļص�����

																	 /*wsprintfA(buf, "%ls", Filename);*/
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	//curl_formadd(&post,
	//	&last,
	//	CURLFORM_COPYNAME, "outer_id",
	//	CURLFORM_PTRCONTENTS, FaceSetId.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "faceset_token",
		CURLFORM_PTRCONTENTS, FSetTokenToGet.c_str(), CURLFORM_END);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, FSetDetailUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	return ErroFlag;
}

size_t Facepp::RemoveFFaSetCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	////------------------��ȡ��������ֵ-----------------
	Json::StyledWriter style_write;
	reader.parse(receive, root);
	//std::cout << style_write.write(root) << std::endl;
	FaPostResult = style_write.write(root);
	////-------------------------------------------------
	if (!root["face_removed"].isNull())ErroFlag = true;//ֻ�м�������һ��face_token��Ϊtrue
	else ErroFlag = false;

	return size * nmemb;
}

bool Facepp::RemoveFFaSet(std::string RemoveFFSetToken)
{
	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��ֻ�гɹ��Ƴ�FaceSet�ڵ�face_tokens�����ز�Ϊtrue
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RemoveFFaSetCallback);//post���ؽ���Ļص�����

																	  /*wsprintfA(buf, "%ls", Filename);*/
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	//curl_formadd(&post,
	//	&last,
	//	CURLFORM_COPYNAME, "outer_id",
	//	CURLFORM_PTRCONTENTS, .c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "faceset_token",
		CURLFORM_PTRCONTENTS, RemoveFFSetToken.c_str(), CURLFORM_END);

	std::string RemoFaTokens;
	RemoFaTokens = "RemoveAllFaceTokens";//�趨Ϊ�Ƴ��úϼ�����������facetoken
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "face_tokens",
		CURLFORM_PTRCONTENTS, RemoFaTokens.c_str(), CURLFORM_END);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, RemoveFFaSetUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	return ErroFlag;
}

size_t Facepp::SearchFFSetCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	////--------------------��ȡ��������ֵ-----------------
	Json::StyledWriter style_write;
	//reader.parse(receive, root);
	//std::cout << style_write.write(root) << std::endl;
	////-------------------------------------------------

	if (reader.parse(receive, root))//�����ؽ����Ϊ��
	{
		/*ErroFlag = true;*/
		FaPostResult = style_write.write(root);//��������ֵ��ֵ
		Json::Value arrayObj;

		if (!root["results"].isNull())
		{
			arrayObj = root["results"];
			for (unsigned int i = 0; i < arrayObj.size(); i++)
			{
				if (!arrayObj[i]["user_id"].isNull())
				{
					if (arrayObj[i]["confidence"].asInt() > 75)//�����FaceSet����ӽ����������Ŷȴ���90����Ϊ�Ǹ�����
					{
						LogoFace = arrayObj[i]["user_id"].asString();//��Search�����û��Զ����ʶ����LogoFace
						if (LogoFace != "")ErroFlag = true;//ֻ�гɹ���⵽����Ϊ�Ѽ�¼�Ĳ������û��Զ����ʶ��������ʶ��Ϊtrue
						else ErroFlag = false;
					}
				}
				else ErroFlag = false;
			}
		}
		else ErroFlag = false;
	}
	else ErroFlag = false;
	return size * nmemb;
}

bool Facepp::SearchFFSet(std::string FFNaToSe, std::string SeaFaSetLogo)
{
	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��ֻ�гɹ���⵽����Ϊ�Ѽ�¼�Ĳ������û��Զ����ʶ��������ʶ��Ϊtrue
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;
	/*static char buf[1024] = "Content-Type:multipart/form-data;charset=UTF-8";*/

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	//std::string access_token_result;
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SearchFFSetCallback);//post���ؽ���Ļص�����

																   /*wsprintfA(buf, "%ls", Filename);*/

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "image_file",
		CURLFORM_FILE, FFNaToSe.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "outer_id",
		CURLFORM_PTRCONTENTS, SeaFaSetLogo.c_str(), CURLFORM_END);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, SearchFFSetUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	return ErroFlag;
}

std::string Facepp::GetSearchRe()
{
	std::string TeLogoFace;
	TeLogoFace = UTF8ToUnicode(LogoFace.c_str());
	LogoFace = "";
	return TeLogoFace;
}

size_t Facepp::FaceTokenIDCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string receive((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	std::string* access_token_result = static_cast<std::string*>(stream);
	Json::Reader reader;
	Json::Value root;

	////------------------��ȡ��������ֵ-----------------
	Json::StyledWriter style_write;
	reader.parse(receive, root);
	//std::cout << style_write.write(root) << std::endl;
	FaPostResult = style_write.write(root);
	////-------------------------------------------------
	if (!root["user_id"].isNull())ErroFlag = true;//ֻ�гɹ��޸�facetoken���Զ����ʶ�����ظ��Զ����ʶ��Ϊtrue
	else ErroFlag = false;

	return size * nmemb;
}

bool Facepp::FaceTokenID(std::string FaceToken, std::string UserId)
{
	ErroFlag = false;//����post֮ǰ��ʹ����ֵʧ�ܱ�־Ϊfalse��//ֻ�гɹ��޸�facetoken���Զ����ʶ�����ظ��Զ����ʶ��Ϊtrue
	CURL *curl;
	CURLcode res;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;
	/*static char buf[1024] = "Content-Type:multipart/form-data;charset=UTF-8";*/

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data;charset=UTF-8");

	//std::string access_token_result;
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FaceTokenIDCallback);//post���ؽ���Ļص�����

																	   /*wsprintfA(buf, "%ls", Filename);*/

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_PTRCONTENTS, FaceppKey.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_PTRCONTENTS, FaceppSecret.c_str(), CURLFORM_END);

	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "face_token",
		CURLFORM_PTRCONTENTS, FaceToken.c_str(), CURLFORM_END);

	std::string UTF8UserId = AsciiToUtf8(UserId);
	curl_formadd(&post,
		&last,
		CURLFORM_COPYNAME, "user_id",
		CURLFORM_PTRCONTENTS, UTF8UserId.c_str(), CURLFORM_END);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, FaceTokenIDUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all(headerlist);
	}
	return ErroFlag;
}
#pragma endregion