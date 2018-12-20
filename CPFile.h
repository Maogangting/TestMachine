/*****************************************************************
CPFile.h
******************************************************************/

#include "stdafx.h"

#ifdef DATA_BACE

class CPData  
{
public:
    
    //Ĭ�ϳ�ʼ�����캯��
    CPData();
    
    //�������ӵĹ��캯��
    CPData(_ConnectionPtr pConnection);
    
    //��������
    virtual ~CPData();
public:
    
    //�������ݿ�
    BOOL Connect(CString strUser,CString strPassword,CString strFileName="ConnectionParam.udl",int nOptions=-1,CString strConStr="Provider=SQLOLEDB;Server=(local);Database=try_db");
    
    //�ر����ݿ������
    void DisConnect();
    
    //���ݿ��ѯ��䣬���������ݿ�ĸ����ֶν��в�ѯ
    //����ɹ�����TRUE,���򷵻�FALSE.��ѯ�Ľ���洢����Ĺ��г�Ա����m_pRecordset��
    //��ѯ����������������ֱ𱣴��ڹ��г�Ա����m_nResultRow��m_nResultCol��
    BOOL Select(CString strSql);
    
    //��ѯ��䣬����Խ�����ѯһ���ֶε�������д���
    //��������CStringArray���͵ı���pResult��
    BOOL Select(CString strSql,CStringArray& Result);
    
    //�Զ���ֶν��в�ѯ
    BOOL SelectMulitCol(CString strSql,CStringArray& Result);
    
    //�򿪱�
    BOOL OpenTable(CString strTable);
    
    //�򿪱�
    BOOL OpenTable(CString strTable,CStringArray& Result);
    
    //���������ĸ��²���
    BOOL Execute(CString strSql);
public:
    BOOL ExecuteTrans(CStringArray& aSql);    
    
    //�رս������
    void CloseRecordset();
    
    //�õ��������������
    long GetResultCol();
    
    //�õ��������������
    long GetResultRow();
    
    //�õ��������
    _RecordsetPtr GetResult();
private:
    
    //���ݿ�������صĽ������
    long m_nResultRow;
    
    //���ص�_RecordsetPtr������
    long m_nResultCol;
    
    //����ָ��
    _ConnectionPtr m_pConnection;
    
    //����ִ��ָ��
    _CommandPtr m_pCommand;
    
    //�����ָ��
    _RecordsetPtr m_pRecordset;
};

#endif