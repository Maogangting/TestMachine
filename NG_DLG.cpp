// NG_DLG.cpp : 实现文件
//

#include "stdafx.h"
#include "MenuTest.h"
#include "NG_DLG.h"
#include "MenuTestView.h"
#include "MainFrm.h"
#include "afxdialogex.h"

#ifdef DATA_BACE
#include "CPFile.h"
extern CPData *p_cp_data;
extern int ado_open_ok;
#endif
// CNG_DLG 对话框

const int year_arr[] = {2017,2018,2019,2020};
int start_y,start_m,start_d,end_y,end_m,end_d;


IMPLEMENT_DYNAMIC(CNG_DLG, CDialog)

CNG_DLG::CNG_DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CNG_DLG::IDD, pParent)
{

}

CNG_DLG::~CNG_DLG()
{
}

void CNG_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NG_LIST, m_ng_list);
	DDX_Control(pDX, IDC_START_Y, m_start_y);
	DDX_Control(pDX, IDC_START_M, m_start_m);
	DDX_Control(pDX, IDC_START_D, m_start_d);
	DDX_Control(pDX, IDC_END_Y, m_end_y);
	DDX_Control(pDX, IDC_END_M, m_end_m);
	DDX_Control(pDX, IDC_END_D, m_end_d);
	DDX_Control(pDX, IDC_KEY, m_input_key);
	DDX_Control(pDX, IDC_NG, m_input_ng);
}


BEGIN_MESSAGE_MAP(CNG_DLG, CDialog)
	ON_BN_CLICKED(IDOK, &CNG_DLG::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SEARCH, &CNG_DLG::OnBnClickedSearch)
	ON_CBN_SELCHANGE(IDC_START_Y, &CNG_DLG::OnCbnSelchangeStartY)
	ON_CBN_SELCHANGE(IDC_START_M, &CNG_DLG::OnCbnSelchangeStartM)
	ON_CBN_SELCHANGE(IDC_START_D, &CNG_DLG::OnCbnSelchangeStartD)
	ON_CBN_SELCHANGE(IDC_END_Y, &CNG_DLG::OnCbnSelchangeEndY)
	ON_CBN_SELCHANGE(IDC_END_M, &CNG_DLG::OnCbnSelchangeEndM)
	ON_CBN_SELCHANGE(IDC_END_D, &CNG_DLG::OnCbnSelchangeEndD)
	ON_BN_CLICKED(IDC_BUTTON1, &CNG_DLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_TO_XLS, &CNG_DLG::OnBnClickedToXls)
END_MESSAGE_MAP()
extern int total_item_num;
BOOL CNG_DLG::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();

	m_start_y.SetCurSel(1);
	m_start_m.SetCurSel(0);
	m_start_d.SetCurSel(0);

	m_end_y.SetCurSel(1);
	m_end_m.SetCurSel(0);
	m_end_d.SetCurSel(0);

	start_y = 1;
	end_y = 1;

	DWORD dwStyle = m_ng_list.GetExtendedStyle();     
        dwStyle |= LVS_EX_FULLROWSELECT;
        dwStyle |= LVS_EX_GRIDLINES;
		//dwStyle |= LVS_REPORT;
        m_ng_list.SetExtendedStyle(dwStyle);
		//total_item_num = 6;
		pCurrView->re_build_main_list(total_item_num, NG_LIST);

	return ret;
}

// CNG_DLG 消息处理程序


void CNG_DLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}

CString string_name = "";	// 搜索项目名称
CString string_ng = "";		// 搜索不良类型
int sql_date1 = -1;			// 起始时间
int sql_date2 = -1;			// 截至时间
void CNG_DLG::OnBnClickedSearch()			// 搜索记录
{
	// TODO: 在此添加控件通知处理程序代码
	m_ng_list.DeleteAllItems();				// 刷新列表

	m_input_key.GetWindowText(string_name);
	string_name.Remove(' ');
	m_input_ng.GetWindowText(string_ng);
	string_ng.Remove(' ');

	sql_date1 = year_arr[start_y] * 10000 + (start_m + 1) * 100 + (start_d + 1) * 1;
	sql_date2 = year_arr[end_y] * 10000 + (end_m + 1) * 100 + (end_d +1) * 1;

	// SQL SERVER 2008 数据库查询操作
#ifdef DATA_BACE
	select_DB_for_list(string_name, string_ng, sql_date1, sql_date2, &m_ng_list, 0);
#endif	// DATABACE
}



// CString == "" int == 0表示无条件
void CNG_DLG::select_DB_for_list(CString string_name, CString string_ng, int sql_date1, int sql_date2, void *pFile, int fg_isFile)
{	
#ifdef DATA_BACE
	if (ado_open_ok) {
		CString sql_string = "select * from TianYouShou where ";
		// 产品名搜索
		if (string_name != "") {
			sql_string += "产品名 like '%";
			sql_string += string_name;
			sql_string += "%' and ";
		}
		// 不良类型搜索
		if (string_ng != "") {
			sql_string += "不良类型 like '%";
			sql_string += string_ng;
			sql_string += "%' and ";
		}
		// 日期搜索
		if (sql_date1 != 0 && sql_date2 != 0) {
			CString str_date_condi;
			str_date_condi.Format("日期 >= '%d 00:00:00' AND 日期 <= '%d 23:59:59' AND ", sql_date1, sql_date2);
			sql_string += str_date_condi;
		}
		sql_string += "1=1";
		sql_string += " ORDER BY 日期";
		// ---------------------------------------------------------------------

		CStringArray Result;
		p_cp_data->SelectMulitCol(sql_string, Result);

		int row = p_cp_data->GetResultRow();
		int col = p_cp_data->GetResultCol();

		if (row == 0) 
			AfxMessageBox("无记录！");
		else {
			for(int i = 0; i < row; i++) {
				for (int j = 0,ziduan = 0; j < col+1; j++) {
					// 滤掉数据库中扩展的空字段 , 另，测试条数 不显示
					if ((j >= FIRST_COL_NUM + total_item_num * 2 && j < FIRST_COL_NUM + DB_SEGMENT_ITEM_NUMMAX * 2)
						|| j == DB_PART_TEST_NUM) continue;

					CString str;
					if (ziduan > 0) {
						str = Result.GetAt(i*col +j-1);
						str.Trim();
					}
					//if (j == DATE) {
						//str.Insert(4,"/");
						//str.Insert(7,"/");
					//}
					CString num;
					num.Format("%d", i);
					if (fg_isFile == 0) {
						CListCtrl_Sortable *p_listCtrl = (CListCtrl_Sortable*)pFile;
						if (ziduan == 0)
							p_listCtrl->InsertItem(i, num);
						else
							p_listCtrl->SetItemText(i, ziduan,str);
					}else {
						CStdioFile *p_file = (CStdioFile*)pFile;
						if (ziduan == 0) {
							if (i == 0) {
								//p_file->WriteString("序号\t");
								p_file->WriteString("日期\t");
								p_file->WriteString("产品名\t");
								p_file->WriteString("扫码ID\t");

								for (int k = 0; k < total_item_num; k++) {
									CString strtest_item_n;
									strtest_item_n.Format("测试项%d\t", k+1);
									p_file->WriteString(strtest_item_n);
									p_file->WriteString("测试值\t");
								}

								p_file->WriteString("不良类型\t");
								p_file->WriteString("不良值\t"); 
								p_file->WriteString("操作员\t");
								p_file->WriteString("加工厂\t");
								p_file->WriteString("其他\t");
							}
							p_file->WriteString("\n");//换行
						}else {
							p_file->WriteString(str + "\t");//写入内容,并换单元格
						}
					}
					ziduan++;
				}
			}
		}
	}
#endif
}


void CNG_DLG::OnCbnSelchangeStartY()
{
	// TODO: 在此添加控件通知处理程序代码
	start_y = m_start_y.GetCurSel();
}


void CNG_DLG::OnCbnSelchangeStartM()
{
	// TODO: 在此添加控件通知处理程序代码
	start_m = m_start_m.GetCurSel();
}


void CNG_DLG::OnCbnSelchangeStartD()
{
	// TODO: 在此添加控件通知处理程序代码
	start_d = m_start_d.GetCurSel();
}


void CNG_DLG::OnCbnSelchangeEndY()
{
	// TODO: 在此添加控件通知处理程序代码
	end_y = m_end_y.GetCurSel();
}


void CNG_DLG::OnCbnSelchangeEndM()
{
	// TODO: 在此添加控件通知处理程序代码
	end_m = m_end_m.GetCurSel();
}


void CNG_DLG::OnCbnSelchangeEndD()
{
	// TODO: 在此添加控件通知处理程序代码
	end_d = m_end_d.GetCurSel();
}


void CNG_DLG::OnBnClickedButton1()
{
	//
}


void CNG_DLG::OnBnClickedToXls()
{
	// TODO: 在此添加控件通知处理程序代码
	if (sql_date1 == -1 || sql_date2 == -1) {
		AfxMessageBox("无搜索结果！");
		return;
	}
	//CFileDialog dlg(FALSE,_T("txt"),_T("test.txt"));//FALSE表示为“另存为”对话框，否则为“打开”对话框
	CFileDialog    dlg(FALSE, "*.xls", "test.xls", OFN_HIDEREADONLY, _T("EXCEL Files (*.xls)|*.xls|EXCEL Files (*.xlsx)|*.XLSX|All Files (*.*)|*.*||"), NULL);
	if(dlg.DoModal() == IDOK)
	{
		CString strFile = dlg.GetPathName();//获取完整路径
		CStdioFile file;

		if(file.Open(strFile,CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite))//创建/打开文件
		{
			select_DB_for_list(string_name, string_ng, sql_date1, sql_date2, &file, 1);
			file.Close();//关闭输出文件
		}
	}
}
