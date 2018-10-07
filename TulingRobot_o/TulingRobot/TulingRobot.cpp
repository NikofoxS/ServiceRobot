#define _CRT_SECURE_NO_WARNINGS
#include "curl/curl.h"
#include <iostream>
#include "json//json.h"    
#pragma comment(lib, "json_vc71_libmtd.lib")    
#pragma comment(lib, "libcurld.lib")

const std::string key = "NULL";
const std::string userid = "NULL";
const char* tulingapi = "http://www.tuling123.com/openapi/api";
char* ANSItoUTF8(const char* strAnsi);
char* UTF8toANSI(const char* strUTF8);
char* UTF8ToEncode(const char* mbcsStr);
char* EncodeToUTF8(const char* mbcsStr);

std::wstring AsciiToUnicode(const std::string& str);
std::string UnicodeToUtf8(const std::wstring& wstr);
std::string AsciiToUtf8(const std::string& str);

int turing(Json::Value rec_result);
static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream);

int main()
{
	Json::Value myword;
	std::string utf8key = AsciiToUtf8(key);
	myword[AsciiToUtf8("key")] = Json::Value(utf8key);
	std::string info = "����Ц��";
	std::string utf8info = AsciiToUtf8(info);
	myword[AsciiToUtf8("info")] = Json::Value(utf8info);
	std::string utf8userid = AsciiToUtf8(userid);
	myword[AsciiToUtf8("userid")] = Json::Value(utf8userid);

	std::string lins;
	Json::FastWriter fast_writer;
	lins = fast_writer.write(myword);
	int errocod = turing(lins);

	//Json::FastWriter fast_writer;
	//std::cout << fast_writer.write(myword) << std::endl;

	std::cin.get();
	std::cin.get();
	return 0;
}

static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string s((char *)ptr, size * nmemb);
	std::string* access_token_result = static_cast<std::string*>(stream);
	// ��ʼ��ȡjson�е�access token��Ŀ
	Json::Reader reader;
	Json::Value root;
	// ʹ��boost�����json
	if (reader.parse(s, root))
	{

		//Json::FastWriter fast_writer;
		//std::cout << fast_writer.write(root) << std::endl;

		std::string aaa = root["text"].asString();
		char* turing_back;
		turing_back = UTF8ToEncode(aaa.c_str());

		std::cout << "ͼ��ظ���" << turing_back << std::endl;
		free(turing_back);

		//std::string bbb = root["code"].asString();
		//char* errcod;
		//errcod = UTF8ToEncode(bbb.c_str());
		//std::cout << "�������" << errcod << std::endl;
	}

	return size * nmemb;
}


int turing(Json::Value rec_result)
{
	CURL *curl;
	CURLcode result_code;
	struct curl_slist *headers = NULL;
	int error_code = 0;
	//rec_result = ANSItoUTF8(rec_result);
	curl = curl_easy_init();
	headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, tulingapi);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POST, 1);//����Ϊ��0��ʾ���β���ΪPOST
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, rec_result);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sizeof(rec_result));//�����ϴ�json������,������ÿ��Ժ���
		std::string access_token_result;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			return 1;
		}
		curl_easy_cleanup(curl);
		error_code = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		error_code = 1;
	}
	return error_code;
}



///////////////////////////////////////////////////////////////////////////////////��Щת������ֵʹ�ú���Ҫ�ֶ��ͷ��ڴ�
//UTF8תANSI
char* UTF8toANSI(const char* strUTF8)
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
char* UTF8ToEncode(const char* mbcsStr)
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

	free(wideStr);
	return unicodeStr;
}

//ANSIתUTF8
char* ANSItoUTF8(const char* strAnsi)
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

char* EncodeToUTF8(const char* mbcsStr)
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
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

std::wstring AsciiToUnicode(const std::string& str)
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

std::string UnicodeToUtf8(const std::wstring& wstr)
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


std::string AsciiToUtf8(const std::string& str)
{
	return UnicodeToUtf8(AsciiToUnicode(str));
}
