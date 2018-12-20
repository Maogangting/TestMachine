#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ListCtrl_Sortable.h"


// CNG_DLG 对话框

class CNG_DLG : public CDialog
{
	DECLARE_DYNAMIC(CNG_DLG)

public:
	CNG_DLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNG_DLG();

// 对话框数据
	enum { IDD = IDD_NG_RECORD };
public:
	// 在数据库中的列排序
	enum{
		DB_PART_NUM	=	0,
		DB_PART_DATE	=1,
		DB_PART_NAME	=2,
		DB_PART_ID		=3,
		DB_PART_TYPE_START	=4,
		DB_PART_TYPE_START_VALUE =  5,
		//.
		//.
		//.
		DB_PART_NG_TYPE = 44,
		DB_PART_NG_VALUE = 45,
		DB_PART_OPER	=46,
		DB_PART_TEST_NUM = 47,
		DB_PART_FACTRORY = 48,
		DB_PART_OTHER2	=49
	};
public:
	void select_DB_for_list(CString string_name, CString string_ng, int sql_date1, int sql_date2, void *pFile, int fg_isFile);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl_Sortable m_ng_list;
	CComboBox m_start_y;
	CComboBox m_start_m;
	CComboBox m_start_d;
	CComboBox m_end_y;
	CComboBox m_end_m;
	CComboBox m_end_d;
	CEdit m_input_key;
	afx_msg void OnBnClickedSearch();
	afx_msg void OnCbnSelchangeStartY();
	afx_msg void OnCbnSelchangeStartM();
	afx_msg void OnCbnSelchangeStartD();
	afx_msg void OnCbnSelchangeEndY();
	afx_msg void OnCbnSelchangeEndM();
	afx_msg void OnCbnSelchangeEndD();
	CEdit m_input_ng;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedToXls();
};
