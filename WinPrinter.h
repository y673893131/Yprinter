#pragma once

#include <vector>
#include <string>

#define ENUM_PRINTER() CWinPrinter::instance()->enumPrinter()
#define SET_PRINTER(x) CWinPrinter::instance()->setPrinter(x)
#define PRINT(x) CWinPrinter::instance()->printText(x)

class CWinPrinter
{
public:
	static CWinPrinter* instance();
	std::string defaultPrinter();
	std::vector<std::string> enumPrinter();
	void setPrinter(const std::string& sDevName);
	bool printText(std::string sText, int nPaperSize = 58);
private:
	CWinPrinter();
	~CWinPrinter();
	
private:
	std::string m_sPrinter;
	static CWinPrinter* m_instance;
};

