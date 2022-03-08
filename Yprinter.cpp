#include "stdafx.h"
#include "WinPrinter.h"

int main(int argc, char* argv[])
{
	auto vPrinters = ENUM_PRINTER();
	while (!vPrinters.empty())
	{
		printf("请选择打印机：\n");
		for (int i = 0; i < vPrinters.size(); ++i)
		{
			printf("%d. %s\n", i + 1, vPrinters[i].c_str());
		}

		int index = 0;
		scanf_s("%d", &index);
		if (index > 0 && index < vPrinters.size())
		{
			SET_PRINTER(vPrinters[index - 1]);
#if 1
			auto bPrint = PRINT("----->printer test<------\n打印测试\n");
#else
			auto bPrint = PRINT1("----->printer test<------\n打印测试\n");
#endif
			if(bPrint) printf("打印完成.\n");
		}
	}

	printf("找不到打印机.");
	getchar();
	return 0;
}

