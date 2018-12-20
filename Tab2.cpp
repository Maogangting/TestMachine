// Tab2.cpp : implementation file
//

#include "stdafx.h"
#include "MenuTest.h"
#include "Tab2.h"
#include "Tab1.h"
#include "MainFrm.h"
//#include "_dlgNew.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int fgNew;
extern PARA_SETTING   setting;
extern PARA_SETTING new_setting;
extern int ntc_selected;

CString CodeFileName = "";
/////////////////////////////////////////////////////////////////////////////
// CTab2 dialog


CTab2::CTab2(CWnd* pParent /*=NULL*/)
	: CDialog(CTab2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTab2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTab2)
	DDX_Control(pDX, IDC_EDIT1, m_dzbiancheng);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_START, m_dzstart);
	DDX_Control(pDX, IDC_STOP, m_dzstop);
}


BEGIN_MESSAGE_MAP(CTab2, CDialog)
	//{{AFX_MSG_MAP(CTab2)
	ON_EN_CHANGE(IDC_EDIT1, OnChangedzBiancheng)
	ON_EN_CHANGE(IDC_START, OnChangedzstart)
	ON_EN_CHANGE(IDC_STOP, OnChangedzstop)
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDC_BUTTON1, &CTab2::OnBnFileImport)
ON_BN_CLICKED(IDC_CHECK_FLAG, &CTab2::OnBnClickedCheckFlag)
ON_BN_CLICKED(IDC_CHECK_CURRENT, &CTab2::OnBnClickedCheckCurrent)
ON_BN_CLICKED(IDC_CHECK_SPEED, &CTab2::OnBnClickedCheckSpeed)
ON_BN_CLICKED(IDC_CHECK_LOW_VOL, &CTab2::OnBnClickedCheckLowVol)
ON_BN_CLICKED(IDC_CHECK_LOW_VOL_END, &CTab2::OnBnClickedCheckLowVolEnd)
ON_BN_CLICKED(IDC_CHECK_DIR, &CTab2::OnBnClickedCheckDir)
ON_BN_CLICKED(IDC_CHECK_TIME, &CTab2::OnBnClickedCheckTime)
ON_BN_CLICKED(IDC_CHECK_BRAKE, &CTab2::OnBnClickedCheckBrake)
ON_BN_CLICKED(IDC_CHECK_NTC1, &CTab2::OnBnClickedCheckNtc1)
ON_BN_CLICKED(IDC_CHECK_NTC2, &CTab2::OnBnClickedCheckNtc2)
ON_BN_CLICKED(IDC_CHECK_NTC3, &CTab2::OnBnClickedCheckNtc3)
ON_BN_CLICKED(IDC_CHECK_NTC4, &CTab2::OnBnClickedCheckNtc4)
ON_BN_CLICKED(IDC_BUTTON1, &CTab2::OnBnClickedAllSel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTab2 message handlers
void ClearSetting(PARA_SETTING *set)
{
	set->LimitCurrent = "";
	set->Speed = "";
	set->LowVol = "";
	set->LowVolEnd = "";
	set->fgDirection = "";
	set->StartTime = "";
	set->fgBrake = "";
	set->fgNTC = 0;
	set->TopEnd = "";
	set->TopStart = "";
	set->BotEnd = "";
	set->BotStart = "";

	set->deta_current = "";
	set->deta_speed = "";
	set->deta_low_vol = "";
	set->deta_low_vol_end = "";
	set->deta_start_time = "";
	set->deta_ntc = "";
	set->motor_pole = "";
	set->normal_vol = "";
	set->other = "";

	set->dzOther = "";
	set->dzStart = "";
	set->dzStop = "";
}
BOOL /*CTab2::*/LoadFile(CString CodeFileName, PARA_SETTING *file_setting)
{
	// 读取文件中的代码，转换成CString
	CStdioFile File;
	CString strParameterCode = "";
	int type = 0; int temp = 0;CString strTemp = "";
	if(File.Open(CodeFileName,CStdioFile::modeRead))//创建/打开文件
	{
		ClearSetting(file_setting);
		while(!feof(File.m_pStream))
		{
			File.ReadString(strTemp);strTemp.Remove('\r');
			
			if (-1 != strTemp.Find("<parameter>", 0)) {
				type = 1;
			}
			if (-1 != strTemp.Find("<start>", 0)){
				type = 2;
			}
			else if(-1 != strTemp.Find("<stop>", 0)){
				type = 3;
			}else if (-1 != strTemp.Find("<other>", 0)){
				type = 4;
			}

			switch (type)
			{
			case 1:
				
				if (-1 != strTemp.Find("<parameter>")) break;

				
				strTemp.Replace(":", "：");strTemp.Remove(' ');
				temp = strTemp.Find("：");
				strParameterCode = strTemp.Mid(temp+2);
				//strTemp.Left(temp - 1);
				if (-1 != strTemp.Find("电流：")) {
					file_setting->LimitCurrent = strParameterCode;
				}else if(-1 != strTemp.Find("转速：")) {
					file_setting->Speed = strParameterCode;
				}else if (-1 != strTemp.Find("欠压：")) {
					file_setting->LowVol = strParameterCode;
				}else if (-1 != strTemp.Find("欠压恢复：")) {
					file_setting->LowVolEnd = strParameterCode;
				}else if(-1 != strTemp.Find("转向：")) {
					file_setting->fgDirection = strParameterCode;
				}else if (-1 != strTemp.Find("软起时间：")) {
					file_setting->StartTime = strParameterCode;
				}else if (-1 != strTemp.Find("刹车：")) {
					file_setting->fgBrake = strParameterCode;
				}else if(-1 != strTemp.Find("电池NTC：")) {
					if (-1 != strParameterCode.Find("有")) {
						file_setting->fgNTC = 1;
					}
					else {
						file_setting->fgNTC = 0;
					}
					ntc_selected = file_setting->fgNTC;
				}else if (-1 != strTemp.Find("高温截至：")) {
					file_setting->TopEnd = strParameterCode;
				}else if (-1 != strTemp.Find("高温恢复：")) {
					file_setting->TopStart = strParameterCode;
				}else if (-1 != strTemp.Find("低温截至：")) {
					file_setting->BotEnd = strParameterCode;
				}else if (-1 != strTemp.Find("低温恢复：")) {
					file_setting->BotStart = strParameterCode;
				}/*公差*/
				else if (-1 != strTemp.Find("电流公差：")) {
					file_setting->deta_current = strParameterCode;
				}else if (-1 != strTemp.Find("转速公差：")) {
					file_setting->deta_speed = strParameterCode;
				}else if (-1 != strTemp.Find("欠压公差：")) {
					file_setting->deta_low_vol = strParameterCode;
				}else if (-1 != strTemp.Find("欠压恢复公差：")) {
					file_setting->deta_low_vol_end = strParameterCode;
				}else if (-1 != strTemp.Find("软启时间公差：")) {
					file_setting->deta_start_time = strParameterCode;
				}else if (-1 != strTemp.Find("NTC公差：")) {
					file_setting->deta_ntc = strParameterCode;
				}else if (-1 != strTemp.Find("极对数：")) {
					file_setting->motor_pole = strParameterCode;
				}else if (-1 != strTemp.Find("额定电压：")) {
					file_setting->normal_vol = strParameterCode;
				}else if (-1 != strTemp.Find("其他：")) {
					file_setting->other = strParameterCode;
				}
				break;
			case 2:
				if (-1 != strTemp.Find("<start>")) break;
				file_setting->dzStart += strTemp;
				file_setting->dzStart += "\r\n";
				break;
			case 3:
				if (-1 != strTemp.Find("<stop>", 0)) break;
				file_setting->dzStop += strTemp;
				file_setting->dzStop += "\r\n";
				break;
			case 4:
				if (-1 != strTemp.Find("<other>", 0)) break;
				file_setting->dzOther += strTemp;
				file_setting->dzOther += "\r\n";
				break;
			default: break;
			}		
		}
	}else {return FALSE;}

	return TRUE;
}

unsigned char fg_normal_program = 0;			// 0: 常规    1：定制编程

unsigned char normal_sel_current = 0;
unsigned char normal_sel_speed = 0;
unsigned char normal_sel_low_vol = 0;
unsigned char normal_sel_low_vol_end = 0;
unsigned char normal_sel_dir = 0;
unsigned char normal_sel_time = 0;
unsigned char normal_sel_brake = 0;
unsigned char normal_sel_ntc1 = 0;
unsigned char normal_sel_ntc2 = 0;
unsigned char normal_sel_ntc3 = 0;
unsigned char normal_sel_ntc4 = 0;

static int all_sel = 0;
BOOL CTab2::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();
	//CString cc = "aaaaaaaa";
	//cc.Format("aaaaaaaa%sbbbbbbb", "\r\n");
	//((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(cc);

	if (0 == fgNew) 
	{
		((CEdit*)GetDlgItem(IDC_START))->SetWindowText(setting.dzStart);
		((CEdit*)GetDlgItem(IDC_STOP))->SetWindowText(setting.dzStop);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(setting.dzOther);
	}

	// 常规测试初始化，初始化默认： 定制测试 (有效)
	all_sel = 0;
	((CButton*)GetDlgItem(IDC_CHECK_FLAG))->SetCheck(fg_normal_program);

	((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->SetCheck(normal_sel_current);
	((CButton*)GetDlgItem(IDC_CHECK_SPEED))->SetCheck(normal_sel_speed);
	((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->SetCheck(normal_sel_low_vol);
	((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->SetCheck(normal_sel_low_vol_end);
	((CButton*)GetDlgItem(IDC_CHECK_DIR))->SetCheck(normal_sel_dir);
	((CButton*)GetDlgItem(IDC_CHECK_TIME))->SetCheck(normal_sel_time);
	((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->SetCheck(normal_sel_brake);
	((CButton*)GetDlgItem(IDC_CHECK_NTC1))->SetCheck(normal_sel_ntc1);
	((CButton*)GetDlgItem(IDC_CHECK_NTC2))->SetCheck(normal_sel_ntc2);
	((CButton*)GetDlgItem(IDC_CHECK_NTC3))->SetCheck(normal_sel_ntc3);
	((CButton*)GetDlgItem(IDC_CHECK_NTC4))->SetCheck(normal_sel_ntc4);

	if (fg_normal_program) {
		// 常规测试有效
		((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_SPEED))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_DIR))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_TIME))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_NTC1))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_NTC2))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_NTC3))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_NTC4))->EnableWindow(1);

		// 定制部分无效，消隐
		((CButton*)GetDlgItem(IDC_START))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_STOP))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(0);

		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(1);
	}else {
		// 常规测试有效
		((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_SPEED))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_DIR))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_TIME))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_NTC1))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_NTC2))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_NTC3))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_NTC4))->EnableWindow(0);

		// 定制部分无效，消隐
		((CButton*)GetDlgItem(IDC_START))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_STOP))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(1);

		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(0);
	}

	return ret;
}

CString biancheng_text = "";
CString start_text = "";
CString stop_text = "";
void CTab2::OnChangedzBiancheng() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	
	m_dzbiancheng.GetWindowText(biancheng_text);
}
void CTab2::OnChangedzstart() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	
	m_dzstart.GetWindowText(biancheng_text);
}
void CTab2::OnChangedzstop() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	
	m_dzstop.GetWindowText(biancheng_text);
}

BOOL CTab2::PreTranslateMessage(MSG* pMsg) 
{/*
	// TODO: Add your specialized code here and/or call the base class

	 if(WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)     
      {       
        if(GetFocus()== GetDlgItem(IDC_EDIT2))  //根据不同控件焦点判断是那个在执行  
        {  
            //updateData(true);  
            //dosomething... 
			//m_dzbiancheng.SetSel(-1, -1);      //自动滚屏 
			//m_dzbiancheng.ReplaceSel(sNewString+" "); 
			biancheng_text += "\r\n";
			m_dzbiancheng.SetWindowText(biancheng_text);
			((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(biancheng_text);
        }  
		if(GetFocus()== GetDlgItem(IDC_START))  //根据不同控件焦点判断是那个在执行  
        {  
            //updateData(true);  
            //dosomething... 
			//m_dzbiancheng.SetSel(-1, -1);      //自动滚屏 
			//m_dzbiancheng.ReplaceSel(sNewString+" "); 
			start_text += "\r\n";
			m_dzstart.SetWindowText(start_text);
			((CEdit*)GetDlgItem(IDC_START))->SetWindowText(start_text);
        }  
		if(GetFocus()== GetDlgItem(IDC_STOP))  //根据不同控件焦点判断是那个在执行  
        {  
            //updateData(true);  
            //dosomething... 
			//m_dzbiancheng.SetSel(-1, -1);      //自动滚屏 
			//m_dzbiancheng.ReplaceSel(sNewString+" "); 
			stop_text += "\r\n";
			m_dzstop.SetWindowText(stop_text);
			((CEdit*)GetDlgItem(IDC_STOP))->SetWindowText(stop_text);
        }  
	 }
*/	
	return CDialog::PreTranslateMessage(pMsg);
}
/*
void CTab2::OnBnFileImport()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
*/

void CTab2::OnBnClickedCheckFlag()
{
	// TODO: 在此添加控件通知处理程序代码
	fg_normal_program = ((CButton*)GetDlgItem(IDC_CHECK_FLAG))->GetCheck();
	if (fg_normal_program) {
		// 常规测试有效
		((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_SPEED))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_DIR))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_TIME))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_NTC1))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_NTC2))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_NTC3))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_CHECK_NTC4))->EnableWindow(1);

		// 定制部分无效，消隐
		//((CButton*)GetDlgItem(IDC_START))->EnableWindow(0);
		//((CButton*)GetDlgItem(IDC_STOP))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(0);

		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(1);

	}else {
		// 常规测试有效
		((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_SPEED))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_DIR))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_TIME))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_NTC1))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_NTC2))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_NTC3))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_NTC4))->EnableWindow(0);

		// 定制部分无效，消隐
		((CButton*)GetDlgItem(IDC_START))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_STOP))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(1);

		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(0);
	}
	return ;
}

void CTab2::OnBnClickedCheckCurrent()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_current	= ((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->GetCheck();
}


void CTab2::OnBnClickedCheckSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_speed	= ((CButton*)GetDlgItem(IDC_CHECK_SPEED))->GetCheck();

}


void CTab2::OnBnClickedCheckLowVol()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_low_vol	= ((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->GetCheck();
}


void CTab2::OnBnClickedCheckLowVolEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_low_vol_end	= ((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->GetCheck();
}


void CTab2::OnBnClickedCheckDir()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_dir	= ((CButton*)GetDlgItem(IDC_CHECK_DIR))->GetCheck();
}


void CTab2::OnBnClickedCheckTime()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_time	= ((CButton*)GetDlgItem(IDC_CHECK_TIME))->GetCheck();
}


void CTab2::OnBnClickedCheckBrake()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_brake	= ((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->GetCheck();
}


void CTab2::OnBnClickedCheckNtc1()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_ntc1	= ((CButton*)GetDlgItem(IDC_CHECK_NTC1))->GetCheck();
}


void CTab2::OnBnClickedCheckNtc2()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_ntc2	= ((CButton*)GetDlgItem(IDC_CHECK_NTC2))->GetCheck();
}


void CTab2::OnBnClickedCheckNtc3()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_ntc3	= ((CButton*)GetDlgItem(IDC_CHECK_NTC3))->GetCheck();
}


void CTab2::OnBnClickedCheckNtc4()
{
	// TODO: 在此添加控件通知处理程序代码
	normal_sel_ntc4	= ((CButton*)GetDlgItem(IDC_CHECK_NTC4))->GetCheck();
}

// 全选
void CTab2::OnBnClickedAllSel()
{
	// TODO: 在此添加控件通知处理程序代码
	all_sel ^= 1;
	if (all_sel) {
		((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_SPEED))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_DIR))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_TIME))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_NTC1))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_NTC2))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_NTC3))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_NTC4))->SetCheck(BST_CHECKED);

		normal_sel_current	= ((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->GetCheck();
		normal_sel_speed	= ((CButton*)GetDlgItem(IDC_CHECK_SPEED))->GetCheck();
		normal_sel_low_vol	= ((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->GetCheck();
		normal_sel_low_vol_end = ((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->GetCheck();
		normal_sel_dir		= ((CButton*)GetDlgItem(IDC_CHECK_DIR))->GetCheck();
		normal_sel_time		= ((CButton*)GetDlgItem(IDC_CHECK_TIME))->GetCheck();
		normal_sel_brake	= ((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->GetCheck();
		normal_sel_ntc1		= ((CButton*)GetDlgItem(IDC_CHECK_NTC1))->GetCheck();
		normal_sel_ntc2		= ((CButton*)GetDlgItem(IDC_CHECK_NTC2))->GetCheck();
		normal_sel_ntc3		= ((CButton*)GetDlgItem(IDC_CHECK_NTC3))->GetCheck();
		normal_sel_ntc4		= ((CButton*)GetDlgItem(IDC_CHECK_NTC4))->GetCheck();

		((CButton*)GetDlgItem(IDC_BUTTON1))->SetWindowText("全不选");
	}else {
		((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_SPEED))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_DIR))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_TIME))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_NTC1))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_NTC2))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_NTC3))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHECK_NTC4))->SetCheck(BST_UNCHECKED);

		normal_sel_current	= ((CButton*)GetDlgItem(IDC_CHECK_CURRENT))->GetCheck();
		normal_sel_speed	= ((CButton*)GetDlgItem(IDC_CHECK_SPEED))->GetCheck();
		normal_sel_low_vol	= ((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL))->GetCheck();
		normal_sel_low_vol_end = ((CButton*)GetDlgItem(IDC_CHECK_LOW_VOL_END))->GetCheck();
		normal_sel_dir		= ((CButton*)GetDlgItem(IDC_CHECK_DIR))->GetCheck();
		normal_sel_time		= ((CButton*)GetDlgItem(IDC_CHECK_TIME))->GetCheck();
		normal_sel_brake	= ((CButton*)GetDlgItem(IDC_CHECK_BRAKE))->GetCheck();
		normal_sel_ntc1		= ((CButton*)GetDlgItem(IDC_CHECK_NTC1))->GetCheck();
		normal_sel_ntc2		= ((CButton*)GetDlgItem(IDC_CHECK_NTC2))->GetCheck();
		normal_sel_ntc3		= ((CButton*)GetDlgItem(IDC_CHECK_NTC3))->GetCheck();
		normal_sel_ntc4		= ((CButton*)GetDlgItem(IDC_CHECK_NTC4))->GetCheck();

		((CButton*)GetDlgItem(IDC_BUTTON1))->SetWindowText("全选");
	}
}
