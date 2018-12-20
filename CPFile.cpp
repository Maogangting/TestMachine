/**************************************************************
CPFile.cpp
**************************************************************/

#include "stdafx.h"
#include "CPFile.h"

#ifdef DATA_BACE

//////////////////////////////////////////////////////////////////////
// 构造函数
//////////////////////////////////////////////////////////////////////
//默认的构造函数
CPData::CPData()
{
    //初始化
    m_nResultRow = 0;
    m_nResultCol=0;
    m_pConnection = NULL;
    //创建对象
    m_pRecordset.CreateInstance(_uuidof(Recordset));
    m_pCommand.CreateInstance(_uuidof(Command)); 
}

//传入参数的构造函数
CPData::CPData(_ConnectionPtr pConnection)
{
    m_pConnection = pConnection; 
    m_nResultRow = 0;
    m_nResultCol=0;
    //创建对象
    m_pRecordset.CreateInstance(_uuidof(Recordset));
    m_pCommand.CreateInstance(_uuidof(Command)); 
    
}
//////////////////////////////////////////////////////////////////////
// 析构函数
//////////////////////////////////////////////////////////////////////
CPData::~CPData()
{ 
    if(m_pRecordset->State!=adStateClosed)
        m_pRecordset->Close();
    m_pRecordset = NULL;

    if(m_pCommand->State!=adStateClosed)
        m_pCommand->Release();
    m_pCommand = NULL;

    if(m_pConnection->State!=adStateClosed) 
        m_pConnection->Close();
    m_pConnection = NULL; 
}

/////////////////////////////////////////////////////////////////////
///简单操作函数
////////////////////////////////////////////////////////////////////

//得到操作结果的行数
long CPData::GetResultRow()
{
    return this->m_nResultRow;
}

//得到操作结果的列数
long CPData::GetResultCol()
{
    return this->m_nResultCol;
}

//得到操作结果
_RecordsetPtr CPData::GetResult()
{
    return this->m_pRecordset;
}

///////////////////////////////////////////////////////////////////////
///连接操作
///////////////////////////////////////////////////////////////////////

//连接到数据库
//1.连接字符串可以自己构造,也可以从文件中读出
BOOL CPData::Connect(CString strUser,CString strPassword,CString strFileName,int nOptions,CString strConStr)
{
    try{ 
        m_pConnection.CreateInstance(__uuidof(Connection));
        HRESULT hr;
        //如果用文件方式配置数据源,进行配置
        if(strFileName.Compare("")!=0/*&&CPFile::IsFileExist(strFileName)*/)
        {
            CString con = "File Name="+strFileName;
            m_pConnection->ConnectionString =(_bstr_t)con;
            hr=m_pConnection->Open("","","",nOptions);    
        }
        else
        {
            //自己配置连接字符串
            m_pConnection->ConnectionString = (_bstr_t)strConStr;
            hr=m_pConnection->Open("",_bstr_t(strUser),_bstr_t(strPassword),nOptions);  
        }
        //进行连接
        //连接失败
        if(FAILED(hr))
        {   
            AfxMessageBox("连接失败!");
            return FALSE;
        }
    }
    catch(_com_error&e)
    {
        AfxMessageBox(e.Description()+"B");
        return FALSE;
    } 
    return TRUE;
}

//断开连接
void CPData::DisConnect()
{
    if(m_pConnection->State!=adStateClosed)
        m_pConnection->Close(); 
}

///////////////////////////////////////////////////////////////////////
///更新操作
///////////////////////////////////////////////////////////////////////
BOOL CPData::Execute(CString strSql)
{
    try
    {
        _variant_t vNULL;
        vNULL.vt = VT_ERROR;
        
        ///定义为无参数
        vNULL.scode = DISP_E_PARAMNOTFOUND;
        
        ///非常关键的一句，将建立的连接赋值给它
        m_pCommand->ActiveConnection = m_pConnection;
        
        ///命令字串
        m_pCommand->CommandText = (_bstr_t)strSql;
        
        ///执行命令，取得记录集
        m_pRecordset = m_pCommand->Execute(&vNULL,&vNULL,adCmdText);
        
        //确实，vNULL中的intVal是执行操作所影响的行数
        m_nResultRow = 0;
        m_nResultRow = vNULL.intVal;  
    }
    catch(_com_error&e)
    {
		AfxMessageBox(e.Description());
        m_nResultRow = 0;  
        return FALSE;
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////
///查询操作
///////////////////////////////////////////////////////////////////////
BOOL CPData::Select(CString strSql)
{
    try
    {
        m_nResultCol=0;
        m_nResultRow=0;  
        m_pRecordset->CursorLocation=adUseClient;    //设置游标位置,设置为客户端形式,否则GetRecordCount()返回值不对
        m_pRecordset->Open(_variant_t(strSql),_variant_t((IDispatch *)m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
        m_nResultCol = m_pRecordset->Fields->GetCount();//得到查询结果的列数
        m_nResultRow = m_pRecordset->GetRecordCount();  //得到查询结果的行数
    }
    catch(_com_error&e)
    {  
        AfxMessageBox(e.Description()+"D");
        return FALSE;
    }
    return TRUE;
}

//查询语句，负责对仅仅查询一个字段的情况进行处理
//结果存放在CStringArray类型的变量pResult中
BOOL CPData::Select(CString strSql,CStringArray& Result)
{
    if(Select(strSql)!=0)
    {
        Result.RemoveAll();
        for(int i=0;i<m_nResultRow;i++)
        {
            _variant_t value;
            value=m_pRecordset->Fields->Item[(long)0]->Value;   
            if(value.vt==3||value.vt==14)
            {
                CString strTrans;
                strTrans.Format("%ld",value.intVal);
                Result.Add(strTrans);
            }
            else
                Result.Add(value.bstrVal);//
            m_pRecordset->MoveNext();
        }
        m_pRecordset->Close();
        return TRUE;
    }
    else
    {
        m_pRecordset->Close();
        return FALSE;
    }
}

BOOL CPData::SelectMulitCol(CString strSql,CStringArray& Result)
{
    if(Select(strSql)!=0)
    {
        Result.RemoveAll();
        _variant_t value;
        for(int i=0;i<m_nResultRow;i++)
        {   
            for(int j=0;j<m_nResultCol;j++)
            {
                value=m_pRecordset->Fields->Item[(long)(/*i*m_nResultCol+*/j)]->Value;
                if(value.vt==2 || value.vt==3||value.vt==14)  // 20180303 mgt增加 value.vt==2 ||
                {
                    CString strTrans;
                    strTrans.Format("%ld",value.intVal);
                    Result.Add(strTrans);
                }
                else
                    if(value.vt==7)
                    {
                        COleDateTime time = value.date;
                        CString strTemp;
                        strTemp.Format("%d-%d-%d %s",time.GetYear(),time.GetMonth(),time.GetDay(),time.Format("%H:%M:%S"));
                        Result.Add(strTemp);
                    }else if (value.vt==1)	{	// NULL  20180319 mgt增加
						Result.Add("");
					}
                    else
                        Result.Add(value.bstrVal);//
            }
            m_pRecordset->MoveNext();
        }
        m_pRecordset->Close();
        return TRUE;
    }
    else
    { 
        m_pRecordset->Close();
        return FALSE;
    }
}

//打开整张表
BOOL CPData::OpenTable(CString strTable)
{
    try
    {
        m_nResultCol=0;
        m_nResultRow=0;  
        m_pRecordset->CursorLocation=adUseClient;    //设置游标位置,设置为客户端形式,否则GetRecordCount()返回值不对
        m_pRecordset->Open(_variant_t(strTable),_variant_t((IDispatch *)m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdTable);
        m_nResultCol = m_pRecordset->Fields->GetCount();//得到查询结果的列数
        m_nResultRow = m_pRecordset->GetRecordCount();  //得到查询结果的行数
    }
    catch(_com_error&e)
    {  
        AfxMessageBox(e.Description()+"E");
        return FALSE;
    }
    return TRUE;
}

BOOL CPData::OpenTable(CString strTable,CStringArray& Result)
{
    if(OpenTable(strTable)!=0)
    {
        Result.RemoveAll();
        _variant_t value;
        for(int i=0;i<m_nResultRow;i++)
        {   
            for(int j=0;j<m_nResultCol;j++)
            {
                value=m_pRecordset->Fields->Item[(long)(/*i*m_nResultCol+*/j)]->Value;
                if(value.vt==3||value.vt==14)
                {
                    CString strTrans;
                    strTrans.Format("%ld",value.intVal);
                    Result.Add(strTrans);
                }
                else
                    if(value.vt==7)
                    {
                        COleDateTime time = value.date;
                        CString strTemp;
                        strTemp.Format("%d-%d-%d %s",time.GetYear(),time.GetMonth(),time.GetDay(),time.Format("%H:%M:%S"));
                        Result.Add(strTemp);
                    }
                    else
                        Result.Add(value.bstrVal);//
            }
            m_pRecordset->MoveNext();
        }
        m_pRecordset->Close();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/////////////////////////////////////////////////////////////////////////////
///关闭结果集
/////////////////////////////////////////////////////////////////////////////
void CPData::CloseRecordset()
{
    if(m_pRecordset->State != adStateClosed)
        m_pRecordset->Close();
}
BOOL CPData::ExecuteTrans(CStringArray& aSql)
{
    try{
        int nNum = aSql.GetSize();
        m_pConnection->BeginTrans(); 
        for(int i=0;i<nNum;i++)
        {
            CString strSql = aSql.GetAt(i);  
            m_pConnection->Execute((_bstr_t)aSql.GetAt(i),NULL,adCmdText);
        }
        m_pConnection->CommitTrans(); 
        return TRUE;
    }
    catch(_com_error& e)
    {
        m_pConnection->RollbackTrans();  
        AfxMessageBox(e.Description()+"F");
        return FALSE;
    } 
}

#endif