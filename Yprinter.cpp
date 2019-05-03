#include "stdafx.h"
#include "WinPrinter.h"

int main(int argc, char* argv[])
{
	auto vPrinters = ENUM_PRINTER();
	if (!vPrinters.empty())
	{
		SET_PRINTER(vPrinters[0]);
		PRINT("----->printer test<------\n´òÓ¡²âÊÔ\n");
	}
	return 0;
}

