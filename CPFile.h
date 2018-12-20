/*****************************************************************
CPFile.h
******************************************************************/

#include "stdafx.h"

#ifdef DATA_BACE

class CPData  
{
public:
    
    //默认初始化构造函数
    CPData();
    
    //传入连接的构造函数
    CPData(_ConnectionPtr pConnection);
    
    //析构函数
    virtual ~CPData();
public:
    
    //连接数据库
    BOOL Connect(CString strUser,CString strPassword,CString strFileName="ConnectionParam.udl",int nOptions=-1,CString strConStr="Provider=SQLOLEDB;Server=(local);Database=try_db");
    
    //关闭数据库的连接
    void DisConnect();
    
    //数据库查询语句，用来对数据库的各种字段进行查询
    //如果成功返回TRUE,否则返回FALSE.查询的结果存储在类的共有成员变量m_pRecordset中
    //查询结果的行数和列数分别保存在共有成员变量m_nResultRow和m_nResultCol中
    BOOL Select(CString strSql);
    
    //查询语句，负责对仅仅查询一个字段的情况进行处理
    //结果存放在CStringArray类型的变量pResult中
    BOOL Select(CString strSql,CStringArray& Result);
    
    //对多个字段进行查询
    BOOL SelectMulitCol(CString strSql,CStringArray& Result);
    
    //打开表
    BOOL OpenTable(CString strTable);
    
    //打开表
    BOOL OpenTable(CString strTable,CStringArray& Result);
    
    //进行其它的更新操作
    BOOL Execute(CString strSql);
public:
    BOOL ExecuteTrans(CStringArray& aSql);    
    
    //关闭结果集合
    void CloseRecordset();
    
    //得到操作结果的列数
    long GetResultCol();
    
    //得到操作结果的条数
    long GetResultRow();
    
    //得到操作结果
    _RecordsetPtr GetResult();
private:
    
    //数据库操作返回的结果条数
    long m_nResultRow;
    
    //返回的_RecordsetPtr中列数
    long m_nResultCol;
    
    //连接指针
    _ConnectionPtr m_pConnection;
    
    //命令执行指针
    _CommandPtr m_pCommand;
    
    //结果集指针
    _RecordsetPtr m_pRecordset;
};

#endif