// testTraderApi.cpp : 定义控制台应用程序的入口点。
//

#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MdSpi.h"
#include "DBCon.h"

// UserApi对象
CThostFtdcMdApi* pUserApi;

//	config ctp api account
char  FRONT_ADDR[] = "tcp://180.168.146.187:10031";		// 前置地址(std CTP 开发环境)
														//char  FRONT_ADDR[] = "tcp://180.168.146.187:10010";		// 前置地址(std CTP 生产环境)
TThostFtdcBrokerIDType	BROKER_ID = "9999";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "089215";			// 注意这里输入你自己的投资者代码
TThostFtdcPasswordType  PASSWORD = "simnow";			// 注意这里输入你自己的用户密码

														//	connect mysql server
dbconnect _connector("tcp://127.0.0.1:3306", "root", "root", "test", "student");

char *ppInstrumentID[] = { "ni1710", "rb1710" };			// 行情订阅列表，注意，这个合约ID会过时的，注意与时俱进修改
int iInstrumentID = 2;									// 行情订阅数量


														// 请求编号
int iRequestID = 0;

void main(void)
{
	// 初始化UserApi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
	pUserApi->RegisterFront(FRONT_ADDR);					// connect
	pUserApi->Init();

	pUserApi->Join();
	//	pUserApi->Release();
}

//int main(void)
//{
//	string url = "tcp://127.0.0.1:3306";
//	dbconnect _connector(url, "root", "root", "test", "student");
//	_connector.insertData();
//
//	char temp;
//	cin >> temp;
//
//	return EXIT_SUCCESS;
//}