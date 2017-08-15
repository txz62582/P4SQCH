#pragma once

namespace SSDK {
	struct Stack
	{
		Stack *next;
		char  *ret;

		template<typename T> inline T arg(unsigned int i)
		{
			return *(T *)((void **)this + i + 2);
		}
	};

	template <typename T>
	T GetVFunc(void *vTable, int iIndex) {
		return (*(T**)vTable)[iIndex];
	}

	template<typename FuncType>
	inline FuncType CallVFunction(void* ppClass, DWORD index) {
		PDWORD pVTable = *(PDWORD*)ppClass;
		DWORD dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}
}