/* Copyright 2008, 2010, Oracle and/or its affiliates. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

There are special exceptions to the terms and conditions of the GPL
as it is applied to this software. View the full text of the
exception in file EXCEPTIONS-CONNECTOR-C++ in the directory of this
software distribution.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "DBCon.h"

/*
Include directly the different
headers from cppconn/ and mysql_driver.h + mysql_util.h
(and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

dbconnect::dbconnect(sql::SQLString dburl, sql::SQLString username, sql::SQLString psd, sql::SQLString db, sql::SQLString table) {
	DBURL = dburl;
	USERNAME = username;
	PSD = psd;
	DBNAME = db;
	TABLE = table;

	cout << endl;
	cout << "Start to connect to the database..." << endl;

	try {
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(DBURL, USERNAME, PSD);
		/* Connect to the MySQL test database */
		con->setSchema(DBNAME);

		stmt = con->createStatement();
		//stmt->execute("USE test");
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line » " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

dbconnect::~dbconnect(void) {
	delete stmt;
	delete con;

	cout << endl;
	cout << "Disconnect the database..." << endl;
};

void dbconnect::insertData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	try {
		char query[2000];
		sprintf(query, "INSERT INTO market(\
				TradingDay, InstrumentID, ExchangeID, ExchangeInstID, LastPrice, PreSettlementPrice, PreClosePrice, \
				PreOpenInterest, OpenPrice, HighestPrice, LowestPrice, Volume, Turnover, OpenInterest, ClosePrice, \
				SettlementPrice, UpperLimitPrice, LowerLimitPrice, PreDelta, CurrDelta, UpdateTime, UpdateMillisec, \
				BidPrice1, BidVolume1, AskPrice1, AskVolume1, BidPrice2, BidVolume2, AskPrice2, AskVolume2, \
				BidPrice3, BidVolume3, AskPrice3, AskVolume3, BidPrice4, BidVolume4, AskPrice4, AskVolume4, \
				BidPrice5, BidVolume5, AskPrice5, AskVolume5, \
				AveragePrice, ActionDay\
			) VALUES (\
				'%s', '%s', '%s', '%s', %.2f, %.2f, %.2f, \
				%.2f, %f, %.2f, %.2f, %d, %.2f, %.2f, %.2f, \
				%.2f, %.2f, %.2f, %f, %.2f, '%s', %d, \
				%.2f, %d, %.2f, %d, %.2f, %d, %.2f, %d, \
				%.2f, %d, %.2f, %d, %.2f, %d, %.2f, %d, \
				%.2f, %d, %.2f, %d, \
				%.2f, '%s'\
			)"\
			, dataValidate(pDepthMarketData->TradingDay), dataValidate(pDepthMarketData->InstrumentID), dataValidate(pDepthMarketData->ExchangeID), dataValidate(pDepthMarketData->ExchangeInstID), dataValidate(pDepthMarketData->LastPrice), dataValidate(pDepthMarketData->PreSettlementPrice), dataValidate(pDepthMarketData->PreClosePrice)
			, dataValidate(pDepthMarketData->PreOpenInterest), dataValidate(pDepthMarketData->OpenPrice), dataValidate(pDepthMarketData->HighestPrice), dataValidate(pDepthMarketData->LowestPrice), dataValidate(pDepthMarketData->Volume), dataValidate(pDepthMarketData->Turnover), dataValidate(pDepthMarketData->OpenInterest), dataValidate(pDepthMarketData->ClosePrice)
			, dataValidate(pDepthMarketData->SettlementPrice), dataValidate(pDepthMarketData->UpperLimitPrice), dataValidate(pDepthMarketData->LowerLimitPrice), dataValidate(pDepthMarketData->PreDelta), dataValidate(pDepthMarketData->CurrDelta), dataValidate(pDepthMarketData->UpdateTime), dataValidate(pDepthMarketData->UpdateMillisec)
			, dataValidate(pDepthMarketData->BidPrice1), dataValidate(pDepthMarketData->BidVolume1), dataValidate(pDepthMarketData->AskPrice1), dataValidate(pDepthMarketData->AskVolume1), dataValidate(pDepthMarketData->BidPrice2), dataValidate(pDepthMarketData->BidVolume2), dataValidate(pDepthMarketData->AskPrice2), dataValidate(pDepthMarketData->AskVolume2)
			, dataValidate(pDepthMarketData->BidPrice3), dataValidate(pDepthMarketData->BidVolume3), dataValidate(pDepthMarketData->AskPrice3), dataValidate(pDepthMarketData->AskVolume3), dataValidate(pDepthMarketData->BidPrice4), dataValidate(pDepthMarketData->BidVolume4), dataValidate(pDepthMarketData->AskPrice4), dataValidate(pDepthMarketData->AskVolume4)
			, dataValidate(pDepthMarketData->BidPrice5), dataValidate(pDepthMarketData->BidVolume5), dataValidate(pDepthMarketData->AskPrice5), dataValidate(pDepthMarketData->AskVolume5)
			, dataValidate(pDepthMarketData->AveragePrice), dataValidate(pDepthMarketData->ActionDay)
		);

		res = stmt->executeQuery(query);

		cout << "--->>> " << pDepthMarketData->InstrumentID << ": LastPrice=" << pDepthMarketData->LastPrice << endl;

		delete res;
	}
	catch (sql::SQLException &e) {
		//cout << "# ERR: SQLException in " << __FILE__;
		//cout << "(" << __FUNCTION__ << ") on line » " << __LINE__ << endl;
		//cout << "# ERR: " << e.what();
		//cout << " (MySQL error code: " << e.getErrorCode();
		//cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	cout << "--->>> " << pDepthMarketData->InstrumentID << ": LastPrice=" << pDepthMarketData->LastPrice << endl;
}

double dbconnect::dataValidate(double data) {
	return data > 1000000000000 ? -1 : data;
}

int dbconnect::dataValidate(int data) {
	return data > 1000000000000 ? -1 : data;
}

char* dbconnect::dataValidate(char *data) {
	if (strcmp(data, "") == 0)
		strcpy(data, "NULL");
	return data;
}