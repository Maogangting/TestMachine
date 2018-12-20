// Tab1.cpp : implementation file
//

#include "stdafx.h"
#include "MenuTest.h"
#include "Tab1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern int fgNew;
PARA_SETTING   setting;
PARA_SETTING   new_setting;
/////////////////////////////////////////////////////////////////////////////
// CTab1 dialog


CTab1::CTab1(CWnd* pParent /*=NULL*/)
	: CDialog(CTab1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTab1)
	//}}AFX_DATA_INIT
}


void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTab1)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTab1, CDialog)
	//{{AFX_MSG_MAP(CTab1)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTab1 message handlers

int ntc_selected = 0;
void CTab1::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	int k = 0;
	ntc_selected = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	if (0 == ntc_selected)
	{
		((CEdit*)GetDlgItem(IDC_EDIT8))->EnableWindow(0);
		((CEdit*)GetDlgItem(IDC_EDIT9))->EnableWindow(0);
		((CEdit*)GetDlgItem(IDC_EDIT10))->EnableWindow(0);
		((CEdit*)GetDlgItem(IDC_EDIT11))->EnableWindow(0);
	}else {
		((CEdit*)GetDlgItem(IDC_EDIT8))->EnableWindow(1);
		((CEdit*)GetDlgItem(IDC_EDIT9))->EnableWindow(1);
		((CEdit*)GetDlgItem(IDC_EDIT10))->EnableWindow(1);
		((CEdit*)GetDlgItem(IDC_EDIT11))->EnableWindow(1);
	}
}

BOOL CTab1::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();

	if (0 == fgNew) 
	{
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(setting.LimitCurrent);
		((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(setting.Speed);
		((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowText(setting.LowVol);
		((CEdit*)GetDlgItem(IDC_EDIT6))->SetWindowText(setting.LowVolEnd);
		((CEdit*)GetDlgItem(IDC_EDIT4))->SetWindowText(setting.fgDirection);
		((CEdit*)GetDlgItem(IDC_EDIT5))->SetWindowText(setting.StartTime);
		((CEdit*)GetDlgItem(IDC_EDIT7))->SetWindowText(setting.fgBrake);
		// ntc
		//ntc_selected = atoi(setting.fgBatNtcIsNeeded);
		((CEdit*)GetDlgItem(IDC_EDIT8))->SetWindowText(setting.TopEnd);
		((CEdit*)GetDlgItem(IDC_EDIT9))->SetWindowText(setting.TopStart);
		((CEdit*)GetDlgItem(IDC_EDIT10))->SetWindowText(setting.BotEnd);
		((CEdit*)GetDlgItem(IDC_EDIT11))->SetWindowText(setting.BotStart);
		//公差
		((CEdit*)GetDlgItem(IDC_EDIT12))->SetWindowText(setting.deta_current);
		((CEdit*)GetDlgItem(IDC_EDIT13))->SetWindowText(setting.deta_speed);
		((CEdit*)GetDlgItem(IDC_EDIT14))->SetWindowText(setting.deta_low_vol);
		((CEdit*)GetDlgItem(IDC_EDIT15))->SetWindowText(setting.deta_low_vol_end);
		((CEdit*)GetDlgItem(IDC_EDIT16))->SetWindowText(setting.deta_start_time);
		((CEdit*)GetDlgItem(IDC_EDIT17))->SetWindowText(setting.deta_ntc);
		((CEdit*)GetDlgItem(IDC_EDIT18))->SetWindowText(setting.motor_pole);
		((CEdit*)GetDlgItem(IDC_EDIT19))->SetWindowText(setting.normal_vol);
		((CEdit*)GetDlgItem(IDC_EDIT20))->SetWindowText(setting.other);
		if (0 == ntc_selected)
		{
			((CEdit*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
			((CEdit*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
			((CEdit*)GetDlgItem(IDC_EDIT10))->EnableWindow(FALSE);
			((CEdit*)GetDlgItem(IDC_EDIT11))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_UNCHECKED);
		}else {
			((CEdit*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
			((CEdit*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
			((CEdit*)GetDlgItem(IDC_EDIT10))->EnableWindow(TRUE);
			((CEdit*)GetDlgItem(IDC_EDIT11))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_CHECKED);
		}

	}else {
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_CHECKED); // 新添加配置时，默认NTC开启
	}

	   
	return ret;
}
