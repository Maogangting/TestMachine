#pragma once
 
///////////////////////////////////////////////////////////////////////////////
class CTaskBase
{
public:
    virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped)=0;
public:
	void *pParam;	// mgt 任务参数/线程参数传递
};
 
///////////////////////////////////////////////////////////////////////////////
extern UINT FuncToClient(LPVOID pParam);
void UserTask(void *pParam)
{
	FuncToClient(pParam);
}
class CTask1 : public CTaskBase
{
public:
	CTask1(void *p){pParam = p;}		// mgt
	~CTask1(){}							// mgt
    virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped)
    {
        TRACE("[%d]: CTask1::DoTask(pvParam=%d, pOverlapped=%d)\n",
            ::GetCurrentThreadId(), *(int *)pParam/*(DWORD_PTR)pvParam*/, (DWORD_PTR)pOverlapped);

		UserTask(pParam);		// mgt // 挂接用户线程任务

		//Sleep(3000);	// mgt
		//while(1);
    }
};
 
 
///////////////////////////////////////////////////////////////////////////////
/*
class CTask2 : public CTaskBase
{
public:
	CTask2(void *p){pParam = p;}
	~CTask2(){}
    virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped)
    {
        //TRACE("[%d]: CTask2::DoTask(pvParam=%d, pOverlapped=%d)\n",
        //    ::GetCurrentThreadId(), *(int *)pParam/ *(DWORD_PTR)pvParam* /, (DWORD_PTR)pOverlapped);
		//Sleep(3000);
		//TRACE("sleep over!");
		//while(1);
    }
};
*/
///////////////////////////////////////////////////////////////////////////////
typedef CSimpleArray<CTaskBase*> CTaskArray;
 
///////////////////////////////////////////////////////////////////////////////
CTaskBase* CreateTasks(CTaskArray& tasks, DWORD dwCount, void *pParam)	// mgt --> void *pParam
{
    //bool bOk = false;
	CTaskBase* pTask = NULL;
    if ( /*0 == tasks.GetSize() &&*/ dwCount > 0 ) {	// mgt
        for ( DWORD i = 0; i < dwCount; i++ ) {
            //if ( i % 2 )
                pTask = new CTask1(pParam);
            //else
            //    pTask = new CTask2(pParam);
            if ( !tasks.Add( pTask ))
                ATLASSERT( false );
        }
        //bOk = true;
    }
    return pTask;
}