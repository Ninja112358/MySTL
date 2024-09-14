#ifndef MEMORYLEAKCHECK_H
#define MEMORYLEAKCHECK_H

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

struct _SetCheckStatus{
public:
	_SetCheckStatus(void)
	{
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	}
	~_SetCheckStatus(void)
	{
		//_CrtDumpMemoryLeaks();�����β���ã���_CRTDBG_LEAK_CHECK_DF��־���Զ����ÿ���ʡ�ԣ���Ϊ��Ҫ������������������м��
	}
}g_SetCheckStatus{};
#endif

#endif