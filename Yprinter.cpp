#include "stdafx.h"
#include "WinPrinter.h"

int main(int argc, char* argv[])
{
	auto vPrinters = ENUM_PRINTER();
	while (!vPrinters.empty())
	{
		printf("��ѡ���ӡ����\n");
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
			auto bPrint = PRINT("----->printer test<------\n��ӡ����\n");
#else
			auto bPrint = PRINT1("----->printer test<------\n��ӡ����\n");
#endif
			if(bPrint) printf("��ӡ���.\n");
		}
	}

	printf("�Ҳ�����ӡ��.");
	getchar();
	return 0;
}

