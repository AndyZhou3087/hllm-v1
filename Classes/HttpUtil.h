#ifndef __HTTPUTIL_H__
#define __HTTPUTIL_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;
typedef enum
{
	GET = 0,
	POST
}HTTPTYPE;

typedef void(*HTTPCOMPLETE_CALLBACK)(std::string retdata, int code, std::string tag);
class HttpUtil
{
public:
	HttpUtil();

	~HttpUtil();
	void doData(std::string url, HTTPCOMPLETE_CALLBACK http_cb = NULL, std::string filename = ""/*����д���ļ�������Ҫд�ļ���""*/, HTTPTYPE method = GET, std::string postdata = "", std::string tag = "");
private:

	static HttpUtil * m_httpInstance;
	HTTPCOMPLETE_CALLBACK m_httpcallback;
	std::string m_filename;
	std::string m_tag;
private:
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *resp);

	class CGarbo // ����Ψһ��������������������ɾ��CSingleton��ʵ��
	{
	public:
		~CGarbo()
		{
			if (HttpUtil::m_httpInstance)
				delete HttpUtil::m_httpInstance;
		}
	};
	static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������

public:
	static HttpUtil * getInstance();

};
#endif