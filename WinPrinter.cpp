#include "stdafx.h"
#include "WinPrinter.h"
#include <assert.h>
#include <windows.h>
#include <winspool.h>
#pragma comment(lib, "winspool.lib")

CWinPrinter* CWinPrinter::m_instance = NULL;
CWinPrinter::CWinPrinter()
{
	m_sPrinter.clear();
}


CWinPrinter::~CWinPrinter()
{
}

std::vector<std::string> CWinPrinter::enumPrinter()
{
	std::vector<std::string> vPrinters;
	DWORD dwNeeded;
	DWORD dwReturn;
	DWORD dwFlag = PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL;
	int n = 0;
	EnumPrintersA(dwFlag, NULL, 4, NULL, 0, &dwNeeded, &dwReturn);
	if (dwNeeded > 0 && dwNeeded < 10000)
	{
		PRINTER_INFO_4A* p4;
		p4 = new PRINTER_INFO_4A[dwNeeded];
		EnumPrintersA(dwFlag, NULL, 4, (PBYTE)p4, dwNeeded, &dwNeeded, &dwReturn);

		for (int i = 0; i < (int)dwReturn && i < 50; i++)
			vPrinters.push_back(p4[i].pPrinterName);
		delete[]p4;
	}

	return std::move(vPrinters);
}

CWinPrinter* CWinPrinter::instance()
{
	if (!m_instance)
	{
		m_instance = new CWinPrinter;
		assert(m_instance);
	}

	return m_instance;
}

std::string CWinPrinter::defaultPrinter()
{
	char szDefault[MAX_PATH] = {};
	DWORD szLength = 0;
	::GetDefaultPrinterA(szDefault, &szLength);
	return szDefault;
}

void CWinPrinter::setPrinter(const std::string& sDevName)
{
	m_sPrinter = sDevName;
}

bool CWinPrinter::printText(std::string sText, int nPaperSize)
{
	char szDefault[MAX_PATH] = {};
	DWORD szLength = 0;
	::GetDefaultPrinterA(szDefault, &szLength);
	if (!::SetDefaultPrinterA(m_sPrinter.c_str()))
		return false;

	PRINTDLG dlg;
	ZeroMemory(&dlg, sizeof(dlg));
	dlg.lStructSize = sizeof(dlg);
	dlg.hwndOwner = NULL;
	dlg.Flags = PD_RETURNDEFAULT | PD_RETURNDC;
	PrintDlg(&dlg);
	DWORD rst = CommDlgExtendedError();//看看出错没有   
	if (rst != 0)
	{
		dlg.Flags = 0;
		PrintDlg(&dlg);//弹出对话框自行选择
	}

	DOCINFOA di = { sizeof(DOCINFOA), "printer", NULL };
	float fZoom = (float)(nPaperSize / 58.0);
	//打印字体，可自行设置
	auto font = ::CreateFont((int)(36 * fZoom), (int)(13 * fZoom), 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("微软雅黑"));
	::SelectObject(dlg.hDC, font);
	int x = (int)(20 * fZoom), y = (int)(20 * fZoom), nIndex = 0;
	std::string sTextPrint;
	TEXTMETRICA tm;
	::GetTextMetricsA(dlg.hDC, &tm);
	if (StartDocA(dlg.hDC, &di) > 0)
	{
		StartPage(dlg.hDC);
		SaveDC(dlg.hDC);
		while ((nIndex = sText.find("\n")) > -1)
		{
			sTextPrint = sText.substr(0, nIndex);
			sText = sText.substr(nIndex + 1, sText.length());
			TextOutA(dlg.hDC, x, y, sTextPrint.c_str(), sTextPrint.length());
			y += tm.tmHeight;
		}

		RestoreDC(dlg.hDC, -1);
		EndPage(dlg.hDC);
		EndDoc(dlg.hDC);
	}

	DeleteObject(font);
	DeleteDC(dlg.hDC);
	::SetDefaultPrinterA(szDefault);
	return true;
}
