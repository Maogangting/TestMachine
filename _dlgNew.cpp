// _dlgNew.cpp : implementation file
//

#include "stdafx.h"
#include "MenuTest.h"
#include "_dlgNew.h"
#include "MainFrm.h"
#include "Tab1.h"
#include "MenuTestView.h"
#include "MyCom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C_dlgNew dialog

CMyCom MyCom("COM4", UART_BAUD_RATE);	// ��������ʵ����Ĭ��COM4

extern PARA_SETTING   setting;
extern int ntc_selected;
extern PARA_SETTING new_setting;

extern BOOL /*CTab2::*/LoadFile(CString CodeFileName, PARA_SETTING *file_setting);
extern void SaveHistroyFile();

C_dlgNew::C_dlgNew(CWnd* pParent /*=NULL*/)
	: CDialog(C_dlgNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(C_dlgNew)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nCurTab = 0;
}


void C_dlgNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C_dlgNew)
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT1, m_save_name);
}


BEGIN_MESSAGE_MAP(C_dlgNew, CDialog)
	//{{AFX_MSG_MAP(C_dlgNew)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDOK, OnNerProjSettingOK)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &C_dlgNew::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &C_dlgNew::OnFileImport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C_dlgNew message handlers


#ifdef CODE_PART

#define DETA_MAX		15		// 4bits
#define DATA_VAL_MAX	16383	// 14bits
unsigned char pc_cmd_bytes_arr[PC_CMD_BYTES_NUM];
// �ú�����Զ���������� ��������ѹ��ת�٣�   ����һ��������� == ����
int save_sub_sub_cmd(unsigned char sub_sub_cmd, CString sub_cmd_string, int sub_sub_cmd_index)
{
	int number_start = 0,number_end = 0;
	unsigned short num_param = 0;
	float  deta = 0;
	unsigned char high_4_bits = 0;

	// v i RPM   ��   pin ad  da������
	if (SUB_SUB_CMD_AD(0) == sub_sub_cmd || SUB_SUB_CMD_DA(0) == sub_sub_cmd || SUB_SUB_CMD_PIN(0) == sub_sub_cmd) {
		number_start = sub_cmd_string.FindOneOf("0123456789");
		if (number_start == -1) {
			ASSERT(0);
			return 0;
		}

		if ((number_end = sub_cmd_string.Find("==")) != -1) {
		}else if ((number_end = sub_cmd_string.Find(">")) != -1) {
		}else if((number_end = sub_cmd_string.Find("<")) != -1) {
		}else if((number_end = sub_cmd_string.Find("!=")) != -1) {
		}else if (number_end == -1) {
			ASSERT(0);
			return 0;
		}
		num_param = atoi(sub_cmd_string.Mid(number_start, number_end - number_start));
	}
	pc_cmd_bytes_arr[sub_sub_cmd_index] = sub_sub_cmd + num_param;
	/////////////////////////////////////////////////////////////////////

	// ���� high_4_bits  "== > < !="
	if ((number_start = sub_cmd_string.Find("==")) != -1) {
		high_4_bits = _equl;
		number_start += 1;		// �������·��ţ�ͳһ��λ��ĩ�˷���
	}else {
		// pin �� uartvol ֻ�� ==
		if (SUB_SUB_CMD_PIN(0) == sub_sub_cmd || SUB_SUB_CMD_UART_VOL_NG == sub_sub_cmd || SUB_SUB_CMD_DIR == sub_sub_cmd) {
			ASSERT(0);
			return 0;
		}

		if ((number_start = sub_cmd_string.Find(">")) != -1) {
			high_4_bits = _more;
		}else if((number_start = sub_cmd_string.Find("<")) != -1) {
			high_4_bits = _less;
		}else if((number_start = sub_cmd_string.Find("!=")) != -1) {
			high_4_bits = _not_equl;
		}else if (number_start == -1) {
			ASSERT(0);
			return 0;
		}
	}

	if (high_4_bits == _equl && SUB_SUB_CMD_PIN(0) != sub_sub_cmd && SUB_SUB_CMD_UART_VOL_NG != sub_sub_cmd && SUB_SUB_CMD_DIR != sub_sub_cmd) { // �й���
		number_end = sub_cmd_string.Find('[');
	}else {															 // �޹���
		// pinû�й���
		if ((number_end = sub_cmd_string.Find('[')) == -1)
			number_end = sub_cmd_string.GetLength();
		else 
			number_end = -1;
	}

	if (number_end == -1) {
		ASSERT(0);
		return 0;
	}
	// ����
	num_param = atoi(sub_cmd_string.Mid(number_start + 1, number_end - (number_start + 1)));
	// ���� �� ֻ�� == �� ��PIN���ͷ�uartvol���� ���й���
	if (high_4_bits == _equl) {
		if (SUB_SUB_CMD_PIN(0) != sub_sub_cmd && SUB_SUB_CMD_UART_VOL_NG != sub_sub_cmd && SUB_SUB_CMD_DIR != sub_sub_cmd) {
			deta = (float)atof(sub_cmd_string.Mid(number_end + 1, sub_cmd_string.GetLength() - 1- (number_end + 1)));
			// �������
			if (SUB_SUB_CMD_I == sub_sub_cmd) {				// unit: 0.5 
				deta *= 2;			// ����������
			}else if (SUB_SUB_CMD_VOL == sub_sub_cmd) {
				deta *= 2;
			}else if (SUB_SUB_CMD_RPM == sub_sub_cmd) {		// unit: 0.5%	
				deta = deta * 100 / num_param * 2;
				num_param /= 10;	// RPM��С10������
			}else if (SUB_SUB_CMD_AD(0) == sub_sub_cmd) {
				deta = deta * 100 / num_param * 2;
			}else if (SUB_SUB_CMD_DA(0) == sub_sub_cmd) {
				deta = deta * 100 / num_param * 2;
			}else if (SUB_SUB_CMD_POWER == sub_sub_cmd) {
				deta = deta * 100 / num_param * 2;
			}else if (SUB_SUB_CMD_PIN(0) == sub_sub_cmd) {	// �޹���
			}else if (SUB_SUB_CMD_UART_VOL_NG == sub_sub_cmd) {// �޹���
			}else if (SUB_SUB_CMD_DIR == sub_sub_cmd) {		// �޹���
			}else {
				ASSERT(0);
				return 0;
			}
		}
	}else if (SUB_SUB_CMD_RPM == sub_sub_cmd) {
		num_param /= 10;	// RPM��С10������
	}

	// �߽�
	if (num_param > DATA_VAL_MAX) num_param = DATA_VAL_MAX;
	if (deta > DETA_MAX) deta = DETA_MAX;

	pc_cmd_bytes_arr[sub_sub_cmd_index - 1] |= ((unsigned char)deta) << 4;	// һ���������ϵ��ӹ���ֵ
	//pc_cmd_bytes_arr[sub_sub_cmd_index] = sub_sub_cmd;
	pc_cmd_bytes_arr[sub_sub_cmd_index + 1] = (high_4_bits << 6) | (num_param >> 8);
	pc_cmd_bytes_arr[sub_sub_cmd_index + 2] = num_param & 0xff;
	return 1;
}
#if 0
// �ú�����Զ���������� MCU����pin �� AD��	����һ��������� == ����
int save_sub_sub_cmdEx(unsigned char sub_sub_cmd, CString sub_cmd_string, int sub_sub_cmd_index)
{
	int number_start = 0, number_end = 0;
	unsigned short num_param = 0;


	number_start = sub_cmd_string.FindOneOf("0123456789");
	if (number_start == -1) {
		ASSERT(0);
		return 0;
	}
	number_end = sub_cmd_string.Find("==");
	if (number_end == -1) {
		ASSERT(0);
		return 0;
	}
	num_param = atoi(sub_cmd_string.Mid(number_start, number_end - number_start));
	pc_cmd_bytes_arr[sub_sub_cmd_index] = sub_sub_cmd + num_param;

	number_start = sub_cmd_string.Find("==");
	if (number_start == -1) {
		ASSERT(0);
		return 0;
	}
	/*number_end = sub_cmd_string.Find(')');
	if (number_end == -1) {
		ASSERT(0);
		return 0;
	}*/
	num_param = atoi(sub_cmd_string.Mid(number_start + 2, sub_cmd_string.GetLength() - number_start - 2));   // -2 ��Ϊ��ʵλ���� ==
	
	pc_cmd_bytes_arr[sub_sub_cmd_index + 1] = num_param >> 8;
	pc_cmd_bytes_arr[sub_sub_cmd_index + 2] = num_param & 0xff;
	return 1;
}
#endif

// ���һ��������� = ����
int save_sub_cmd_set(unsigned char sub_sub_cmd, CString sub_cmd_string, int sub_sub_cmd_index)
{
	int number_start = 0, number_end = 0;
	unsigned short num_param = 0;


	number_start = sub_cmd_string.FindOneOf("0123456789");
	if (number_start == -1) {
		ASSERT(0);
		return 0;
	}
	number_end = sub_cmd_string.Find('=');
	if (number_end == -1) {
		ASSERT(0);
		return 0;
	}
	num_param = atoi(sub_cmd_string.Mid(number_start, number_end - number_start));
	pc_cmd_bytes_arr[sub_sub_cmd_index] = sub_sub_cmd + num_param;

	number_start = sub_cmd_string.Find('=');
	if (number_start == -1) {
		ASSERT(0);
		return 0;
	}
	/*number_end = sub_cmd_string.Find(';');
	if (number_end == -1) {
		ASSERT(0);
		return 0;
	}*/
	num_param = atoi(sub_cmd_string.Mid(number_start + 1, sub_cmd_string.GetLength() - number_start - 1));   // -1 ��Ϊ��ʵλ���� =
	
	pc_cmd_bytes_arr[sub_sub_cmd_index + 1] = num_param >> 8;
	pc_cmd_bytes_arr[sub_sub_cmd_index + 2] = num_param & 0xff;
	return 1;
}
// ���һ��������� /*volto();*/ loadto(); delay();		// ����λ�ڵ�3��4�ֽڣ��ڶ��ֽ�ͳһΪ����ָ��˴�Ϊ�� 0
int save_sub_cmd(CString sub_cmd_string, int sub_sub_cmd_index)
{
	int number_start = 0, number_end = 0;
	unsigned short num_param = 0;

	number_start = sub_cmd_string.FindOneOf("0123456789");
	if (number_start == -1) {
		ASSERT(0);
		return 0;
	}
	/*number_end = sub_cmd_string.Find(')');
	if (number_end == -1) {
		ASSERT(0);
		return 0;
	}*/
	num_param = atoi(sub_cmd_string.Mid(number_start, sub_cmd_string.GetLength() - number_start));
	pc_cmd_bytes_arr[sub_sub_cmd_index + 1] = num_param >> 8;
	pc_cmd_bytes_arr[sub_sub_cmd_index + 2] = num_param & 0xff;
	return 1;
}

// ���һ��������� volto();		// ���ݣ��ڶ�������λ�ڵ�3��4�ֽڣ��ڶ��ֽ�Ϊ��һ����ֵ
int save_sub_cmd_multparam(CString sub_cmd_string, int sub_sub_cmd_index)
{
	int number_start = 0, number_end = 0;
	unsigned short num_param = 0;

	number_start = sub_cmd_string.FindOneOf("0123456789");
	if (number_start == -1) {
		ASSERT(0);
		return 0;
	}
	number_end = sub_cmd_string.Find(',');
	if (number_end == -1) {
		ASSERT(0);
		return 0;
	}
	num_param = atoi(sub_cmd_string.Mid(number_start, number_end - number_start));
	pc_cmd_bytes_arr[sub_sub_cmd_index] = (unsigned char)num_param;

	num_param = atoi(sub_cmd_string.Mid(number_end + 1, sub_cmd_string.GetLength() - number_end - 1));
	pc_cmd_bytes_arr[sub_sub_cmd_index + 1] = num_param >> 8;
	pc_cmd_bytes_arr[sub_sub_cmd_index + 2] = num_param & 0xff;
	return 1;
}
#endif

void C_dlgNew::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	int nNewSel = m_Tab.GetCurSel();  
  
    if (m_nCurTab != nNewSel)    
  
    {    
  
        m_paDlg[m_nCurTab]->ShowWindow(SW_HIDE);     
  
        m_paDlg[nNewSel]->ShowWindow(SW_SHOW);  
  
        m_nCurTab = nNewSel;    
  
    } 
}
extern CString file_selected;
extern int fgNew;
extern CList<HISTROY_FILE ,HISTROY_FILE&> histrory_list;
void C_dlgNew::OnNerProjSettingOK() 
{
	// TODO: Add your control notification handler code here
	if (0 == fgNew || 1 == fgNew) {
		CString name = "";
		m_save_name.GetWindowText(name);
		if (name == "") {
			AfxMessageBox("����д�����ļ�����");
			return;
		}
		// ������Ŀ��Ϣ
		// 1���ļ�����		// ��һ����Ŀ���ã�һ����׼��ʽ�ļ���
		CFileDialog    dlg(FALSE, "*.c", name, OFN_HIDEREADONLY, _T("C Files (*.c)|*.c|TXT Files (*.txt)|*.txt||"), NULL);
		if(dlg.DoModal() == IDOK)
		{
			CString strFile = dlg.GetPathName();//��ȡ����·��
			CStdioFile file;

			if(file.Open(strFile,CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite))//����/���ļ�
			{
				m_dlg1.GetDlgItem(IDC_EDIT1)->GetWindowText(setting.LimitCurrent);
				m_dlg1.GetDlgItem(IDC_EDIT2)->GetWindowText(setting.Speed);
				m_dlg1.GetDlgItem(IDC_EDIT3)->GetWindowText(setting.LowVol);
				m_dlg1.GetDlgItem(IDC_EDIT6)->GetWindowText(setting.LowVolEnd);
				m_dlg1.GetDlgItem(IDC_EDIT4)->GetWindowText(setting.fgDirection);
				m_dlg1.GetDlgItem(IDC_EDIT5)->GetWindowText(setting.StartTime);
				m_dlg1.GetDlgItem(IDC_EDIT7)->GetWindowText(setting.fgBrake);
				m_dlg1.GetDlgItem(IDC_EDIT8)->GetWindowText(setting.TopEnd);
				m_dlg1.GetDlgItem(IDC_EDIT9)->GetWindowText(setting.TopStart);
				m_dlg1.GetDlgItem(IDC_EDIT10)->GetWindowText(setting.BotEnd);
				m_dlg1.GetDlgItem(IDC_EDIT11)->GetWindowText(setting.BotStart);
				//
				m_dlg1.GetDlgItem(IDC_EDIT12)->GetWindowText(setting.deta_current);
				m_dlg1.GetDlgItem(IDC_EDIT13)->GetWindowText(setting.deta_speed);
				m_dlg1.GetDlgItem(IDC_EDIT14)->GetWindowText(setting.deta_low_vol);
				m_dlg1.GetDlgItem(IDC_EDIT15)->GetWindowText(setting.deta_low_vol_end);
				m_dlg1.GetDlgItem(IDC_EDIT16)->GetWindowText(setting.deta_start_time);
				m_dlg1.GetDlgItem(IDC_EDIT17)->GetWindowText(setting.deta_ntc);
				m_dlg1.GetDlgItem(IDC_EDIT18)->GetWindowText(setting.motor_pole);
				m_dlg1.GetDlgItem(IDC_EDIT19)->GetWindowText(setting.normal_vol);
				m_dlg1.GetDlgItem(IDC_EDIT20)->GetWindowText(setting.other);
				// ��������
				CString code = "";	// strTemp = "";
				code.Format(
					"<parameter>\r\n������  %s\r\n��������:  %s\r\nת��:  %s\r\nת�ٹ���:  %s\r\nǷѹ��%s\r\nǷѹ����:  %s\r\nǷѹ�ָ���%s\r\nǷѹ�ָ�����:  %s\r\nת��%s\r\n����ʱ�䣺%s\r\n����ʱ�乫��:  %s\r\nɲ����%s\r\n���NTC��%s\r\n���½�����%s\r\n���»ָ���%s\r\n���½�����%s\r\n���»ָ���%s\r\nNTC���%s\r\n�����������%s\r\n���ѹ��%s\r\n������%s", 
					setting.LimitCurrent,
					setting.deta_current,
					setting.Speed,
					setting.deta_speed,
					setting.LowVol,
					setting.deta_low_vol,
					setting.LowVolEnd,
					setting.deta_low_vol_end,
					setting.fgDirection,
					setting.StartTime,
					setting.deta_start_time,
					setting.fgBrake,
					(1 == ntc_selected)? "��":"��",
					setting.TopEnd,
					setting.TopStart,
					setting.BotEnd,
					setting.BotStart,
					setting.deta_ntc,
					setting.motor_pole,
					setting.normal_vol,
					setting.other);

				// ���Ʋ���
				code += "\r\n<start>\r\n";
				m_dlg2.GetDlgItem(IDC_START)->GetWindowText(setting.dzStart);
				code += setting.dzStart; code += "\r\n<stop>\r\n";
				m_dlg2.GetDlgItem(IDC_STOP)->GetWindowText(setting.dzStop);
				code += setting.dzStop; code += "\r\n<other>\r\n";
				m_dlg2.GetDlgItem(IDC_EDIT1)->GetWindowText(setting.dzOther);
				code += setting.dzOther; code += "\r\n";
				file.WriteString(code);//д������
				file.Close();//�ر�����ļ�
			}
			// 2��������ʷ��¼
			//histrory_list.AddHead(strFile);
			((CMainFrame*)AfxGetMainWnd())->OnCommand(0,(long)(&strFile));
			SaveHistroyFile();

			file_selected = strFile;
			pCurrView->GetDlgItem(IDC_EDIT1)->SetWindowText(file_selected);
		}
		
	}else if (2 == fgNew){		// ��������
		// �������ò�����
		// 1���޸��ļ�������
	}

	fgNew = 0;
	
	OnOK();
}
extern int fgNew;
extern CString file_selected;
 
BOOL C_dlgNew::OnInitDialog()
{


	BOOL ret = CDialog::OnInitDialog();

	if (0 == fgNew) 
	{
		m_save_name.SetWindowText(file_selected);
		m_save_name.EnableWindow(0);
	}else if (2 == fgNew) {
		GetDlgItem(IDOK)->SetWindowText("��������");
	}

	

	m_Tab.InsertItem(0, "��������");
	m_Tab.InsertItem(1, "��������");

	m_dlg1.Create(IDD_DIALOG1,&m_Tab);  
	m_dlg2.Create(IDD_DIALOG2,&m_Tab);  
  
	m_paDlg[0] = &m_dlg1;  
	m_paDlg[1] = &m_dlg2; 

	CRect rc;     
	m_Tab.GetClientRect(&rc);  
	rc.top  += 23;    
	rc.left  += 3;  
	rc.bottom -= 3;     
	rc.right -= 3;  
  
	m_dlg1.MoveWindow(&rc);  
	m_dlg2.MoveWindow(&rc);  
	//��ʾ��һ�δ򿪵�һ���ӶԻ��򴰿�
	if (m_nCurTab == 0) {
		m_dlg1.ShowWindow(SW_SHOW); 
	}else if (m_nCurTab == 1) {
		m_dlg2.ShowWindow(SW_SHOW);
	}
	m_Tab.SetCurSel(m_nCurTab);

	return ret;
}

void C_dlgNew::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	fgNew = 0;
	OnCancel();
}


void C_dlgNew::OnFileImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog    dlgFile(TRUE, NULL, "code.c", OFN_HIDEREADONLY, "C Files (*.c)|*.c|text Files (*.txt)|*.txt||", NULL);
    if (dlgFile.DoModal() == IDOK)
    {
		if (TRUE == LoadFile(dlgFile.GetPathName(), &new_setting)) {
			m_dlg2.GetDlgItem(IDC_START)->SetWindowText(new_setting.dzStart);
			m_dlg2.GetDlgItem(IDC_STOP)->SetWindowText(new_setting.dzStop);
			m_dlg2.GetDlgItem(IDC_EDIT1)->SetWindowText(new_setting.dzOther);

			m_dlg1.GetDlgItem(IDC_EDIT1)->SetWindowText(new_setting.LimitCurrent);
			m_dlg1.GetDlgItem(IDC_EDIT2)->SetWindowText(new_setting.Speed);
			m_dlg1.GetDlgItem(IDC_EDIT3)->SetWindowText(new_setting.LowVol);
			m_dlg1.GetDlgItem(IDC_EDIT6)->SetWindowText(new_setting.LowVolEnd);
			m_dlg1.GetDlgItem(IDC_EDIT4)->SetWindowText(new_setting.fgDirection);
			m_dlg1.GetDlgItem(IDC_EDIT5)->SetWindowText(new_setting.StartTime);
			m_dlg1.GetDlgItem(IDC_EDIT7)->SetWindowText(new_setting.fgBrake);
			m_dlg1.GetDlgItem(IDC_EDIT8)->SetWindowText(new_setting.TopEnd);
			m_dlg1.GetDlgItem(IDC_EDIT9)->SetWindowText(new_setting.TopStart);
			m_dlg1.GetDlgItem(IDC_EDIT10)->SetWindowText(new_setting.BotEnd);
			m_dlg1.GetDlgItem(IDC_EDIT11)->SetWindowText(new_setting.BotStart);
			//
			m_dlg1.GetDlgItem(IDC_EDIT12)->SetWindowText(new_setting.deta_current);
			m_dlg1.GetDlgItem(IDC_EDIT13)->SetWindowText(new_setting.deta_speed);
			m_dlg1.GetDlgItem(IDC_EDIT14)->SetWindowText(new_setting.deta_low_vol);
			m_dlg1.GetDlgItem(IDC_EDIT15)->SetWindowText(new_setting.deta_low_vol_end);
			m_dlg1.GetDlgItem(IDC_EDIT16)->SetWindowText(new_setting.deta_start_time);
			m_dlg1.GetDlgItem(IDC_EDIT17)->SetWindowText(new_setting.deta_ntc);
			m_dlg1.GetDlgItem(IDC_EDIT18)->SetWindowText(new_setting.motor_pole);
			m_dlg1.GetDlgItem(IDC_EDIT19)->SetWindowText(setting.normal_vol);
			m_dlg1.GetDlgItem(IDC_EDIT20)->SetWindowText(setting.other);
			if (1 == new_setting.fgNTC) {
				((CButton*)m_dlg1.GetDlgItem(IDC_CHECK1))->SetCheck(BST_CHECKED);
				m_dlg1.GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
				m_dlg1.GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
				m_dlg1.GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
				m_dlg1.GetDlgItem(IDC_EDIT11)->EnableWindow(TRUE);
			}else {
				((CButton*)m_dlg1.GetDlgItem(IDC_CHECK1))->SetCheck(BST_UNCHECKED);
				m_dlg1.GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
				m_dlg1.GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
				m_dlg1.GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
				m_dlg1.GetDlgItem(IDC_EDIT11)->EnableWindow(FALSE);
			}
			
			GetDlgItem(IDC_EDIT1)->SetWindowText(dlgFile.GetFileTitle());
		}
    }
}
extern CList<HISTROY_FILE ,HISTROY_FILE&> histrory_list;
void SaveHistroyFile()
{
	CString strHis = "";
	CStdioFile file;

	if(file.Open(HIS_FILE,CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite))//����/���ļ�
	{
		POSITION pos = histrory_list.GetTailPosition(); // pos == last 0;

		for (int i = 0; i < histrory_list.GetCount(); i++) {
			strHis += histrory_list.GetPrev(pos).histroy;
			if (i + 1 < histrory_list.GetCount())
				strHis += "\r\n";
		}

		file.WriteString(strHis);

		file.Close();//�ر�����ļ�
	}
}

#ifdef CODE_PART

// �������������á�
int GetNumberVal(CString str)
{
	str.Remove(' ');
	str.Remove('\r');
	str.Remove('\n');
	str.Remove('\t');
	str.MakeLower();
	str.SpanIncluding("0123456789");
	return str == ""? 0: atoi(str);
}
int send_bace_param_code(PARA_SETTING * psetting)
{
// �������´������η��ͣ����øı䷢���Ⱥ�˳�򣬲���Ҫ����λ��ͳһ�Ⱥ����
//	1��	����
//	2,	��������
//  3,	ת��
//	4�� ת�ٹ���
//	5,	Ƿѹ
//	6��	Ƿѹ����
//	7,	Ƿѹ�ָ�
//	8,	Ƿѹ�ָ�����
//	9,	ת��
//	10,	����ʱ��
//	11,	����ʱ�乫��
//	12,	ɲ��
//	13,	NTC���½���
//	14,	NTC���»ָ�
//	15,	NTC���½���
//	16,	NTC���»ָ�
//	17��NTC����
//  18�����������
	ZeroMemory(pc_cmd_bytes_arr, sizeof(unsigned char) * PC_CMD_BYTES_NUM);	// �� 0
	pc_cmd_bytes_arr[MAIN_CMD_INDEX] = MAIN_CMD_PARAM;
	pc_cmd_bytes_arr[CMD_NUM_INDEX] = BACE_PARAM_NUM;
	unsigned short number_val = 0;
	number_val = GetNumberVal(psetting->LimitCurrent);
	pc_cmd_bytes_arr[SUB_CMD_INDEX] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 1] = number_val & 0xff;
	
	number_val = GetNumberVal(psetting->deta_current);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 2] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 3] = number_val & 0xff;

	number_val = GetNumberVal(psetting->Speed);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 4] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 5] = number_val & 0xff;

	number_val = GetNumberVal(psetting->deta_speed);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 6] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 7] = number_val & 0xff;

	number_val = GetNumberVal(psetting->LowVol);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 8] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 9] = number_val & 0xff;

	number_val = GetNumberVal(psetting->deta_low_vol);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 10] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 11] = number_val & 0xff;

	number_val = GetNumberVal(psetting->LowVolEnd);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 12] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 13] = number_val & 0xff;

	number_val = GetNumberVal(psetting->deta_low_vol_end);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 14] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 15] = number_val & 0xff;

	number_val = psetting->fgDirection.Find("��ʱ��") != -1? 1: 0;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 16] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 17] = number_val & 0xff;

	number_val = GetNumberVal(psetting->StartTime);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 18] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 19] = number_val & 0xff;

	number_val = GetNumberVal(psetting->deta_start_time);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 20] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 21] = number_val & 0xff;

	number_val = GetNumberVal(psetting->fgBrake);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 22] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 23] = number_val & 0xff;

	number_val = GetNumberVal(psetting->TopEnd);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 24] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 25] = number_val & 0xff;

	number_val = GetNumberVal(psetting->TopStart);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 26] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 27] = number_val & 0xff;

	number_val = GetNumberVal(psetting->BotEnd);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 28] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 29] = number_val & 0xff;

	number_val = GetNumberVal(psetting->BotStart);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 30] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 31] = number_val & 0xff;

	number_val = GetNumberVal(psetting->deta_ntc);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 32] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 33] = number_val & 0xff;

	number_val = GetNumberVal(psetting->motor_pole);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 34] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 35] = number_val & 0xff;

	number_val = GetNumberVal(psetting->normal_vol);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 36] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 37] = number_val & 0xff;

	number_val = GetNumberVal(psetting->other);
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 38] = number_val >> 8;
	pc_cmd_bytes_arr[SUB_CMD_INDEX + 39] = number_val & 0xff;
	
	// ���ݳ���Ϊ˫�ֽڱ�ʾ
	unsigned short data_num = BACE_PARAM_NUM * 2 + 4;
	pc_cmd_bytes_arr[DATA_LENGTH_INDEX] = data_num >> 8;
	pc_cmd_bytes_arr[DATA_LENGTH_INDEX + 1] = data_num & 0xff;

	return MyCom.UartSendCmd(pc_cmd_bytes_arr, BACE_PARAM_NUM * 2 + 7,0);		// ���ô��ڷ���֡
}

extern int modle_item_content(const CString& code);
// �����Ʋ��֡�
//						����ָ��
//	Px = 1				// x������  , DA��ֵ����ע����A/D , DA ���Ƶ�����ֵ��Ϊ��ѹֵ����λ��mV
//	==					// ��OK�� or ��NG������жϣ������ж����š���ѹ��������ת�١��¶ȡ�ĳ��A/Dֵ��
//	delay(t)			// ��ʱ t ms
//
//	LoadTo(L)			// ���ƺ���Դ���ص�L
//	VolTo(x)			// ���Ƶ�Դ������ѹ��x V
//	While(XXX) ��		// �����жϣ��ȴ������� XXX�����㣻���������ǣ����š���ѹ��������ת�١��¶ȡ�ĳ��A/Dֵ�� ��ע����A/D , DA ���Ƶ�����ֵ��Ϊ��ѹֵ����λ��mV
//	Start				// ����      ������ִ��startʱ���⣬������һЩ����׼���������磬�򿪼̵����ȣ�
//	Stop				// ͣ��     ���ر����Զ��ŵ硢�ϱ�PC�����������ϵ硢[�豸��λ]��������β���ˣ�
//	ToPC(x)				// ��x�ļ��ֵ�Ϸ���PC
int send_program_code(int program_type)
{
	int cmd_num = 0;	//ָ����Ŀ
	ZeroMemory(pc_cmd_bytes_arr, sizeof(unsigned char) * PC_CMD_BYTES_NUM);	// �� 0

	pc_cmd_bytes_arr[MAIN_CMD_INDEX] = program_type;

	CString program_code;
	if (MIAN_CMD_PROGRAM_START == program_type) {
		program_code = setting.dzStart;
	}else if (MIAN_CMD_PROGRAM_STOP == program_type) {
		program_code = setting.dzStop;
	}else if (MIAN_CMD_PROGRAM_OTHER == program_type) {
		program_code = setting.dzOther;
	}

//	program_code = "RPM > 2000[9];";		// debug

	// �ַ���Ԥ����
	int xiegang_index = 0,huiche_index = 0;
	// 1��ȥ����ע��		"/* -- */"
	while(1) {
		if ((xiegang_index = program_code.Find("/*", 0)) != -1) {
			if ((huiche_index = program_code.Find("*/", xiegang_index + 2)) != -1) {
				program_code.Delete(xiegang_index, huiche_index + 2 - xiegang_index);
			}else {
				ASSERT(0);
				TRACE("error!!!");
				return 0;
			}
			
		}else {
			break;
		}
	}
// -----------------�������������------------------------
	if (MIAN_CMD_PROGRAM_OTHER == program_type) {
		if (0 == modle_item_content(program_code))
			return 0;
	}
// ----------------------------------------------------
	
	// 2��ȥ"����������"ע��		"//"
	while(1) {
		if ((xiegang_index = program_code.Find("//", 0)) != -1) {
			if ((huiche_index = program_code.Find("\r\n", xiegang_index)) == -1) {
				huiche_index = program_code.GetLength();
			}
			program_code.Delete(xiegang_index, huiche_index - xiegang_index);
		}else {
			break;
		}
	}
	
	// 3��ȥ�����ַ�
	program_code.Remove(' ');
	program_code.Remove('\r');
	program_code.Remove('\n');
	program_code.Remove('\t');
	program_code.MakeLower();

#ifdef SIMULATE_TOPC
	while(0) {
#else
	while (program_code.GetLength() > 0) {
#endif
		unsigned char temp_sub_sub_cmd = 0;
		CString cmd = program_code.SpanExcluding(";");
		//cmd += ";";
		cmd.TrimRight(")");		// �ѵ�����������������ȥ����ȡ����ʱ����ֱ�Ӱ��ַ����ȼ���
		int fenhao_number = program_code.Find(";");
		if(fenhao_number != -1) { 
			program_code.Delete(0,fenhao_number +1);
		}else {
			ASSERT(0);
			TRACE("error!!!");
			return 0;
		}


		// ���µ��жϴ����ܱ䶯, �����¼���ָ��Ҳ��Ҫ����˳������
		if (cmd.Find("while") != -1) {
			//TRACE("while      ");TRACE(cmd);TRACE("\r\n");
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_WAITUNTIL;
			
			// �ȴ������� XXX�����㣻
			// ���������ǣ�����(Pn)��ĸ�ߣ�Ĭ�ϣ���ѹ(Vol)���ܵ���(I)��ת��(rpm)��ĳ·A/Dֵ (ADn)�ȡ���������������ע����A/D , DA ���Ƶ�����ֵ��Ϊ��ѹֵ����λ��mV
			if (cmd.Find("rpm",5) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_RPM;
			}else if (cmd.Find("uartvol",5) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_UART_VOL_NG;
			}else if (cmd.Find("vol",5) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_VOL;
			}else if (cmd.Find("pow",5) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_POWER;
			}else if (cmd.Find("dir",5) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_DIR;
			}else if (cmd.Find("p",5) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_PIN(0);
			}else if (cmd.Find("i",5) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_I;
			}else if (cmd.Find("ad",5) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_AD(0);
			}else {
				ASSERT(0);
				TRACE("error!!!");
				return 0;
			}
			if (0 == save_sub_sub_cmd(temp_sub_sub_cmd, cmd, this_cmd_index + 1)) {
				return 0;
			}
		}else if (cmd.Find("if") != -1) { // �ݲ�����
		}else if (cmd.Find("==") != -1 || cmd.Find(">") != -1 || cmd.Find("<") != -1 || cmd.Find("!=") != -1) {
			//TRACE("==      ");TRACE(cmd);TRACE("\r\n");
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_JUDGE;

			// �ȴ������� XXX�����㣻
			// ���������ǣ�����(Pn)��ĸ�ߣ�Ĭ�ϣ���ѹ(Vol)���ܵ���(I)��ת��(rpm)��ĳ·A/Dֵ (ADn)�ȡ������������� ��ע����A/D , DA ���Ƶ�����ֵ��Ϊ��ѹֵ����λ��mV
			if (cmd.Find("rpm",0) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_RPM;
			}else if (cmd.Find("uartvol",0) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_UART_VOL_NG;
			}else if (cmd.Find("vol",0) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_VOL;
			}else if (cmd.Find("pow",0) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_POWER;
			}else if (cmd.Find("dir",0) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_DIR;
			}else if (cmd.Find("p",0) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_PIN(0);
			}else if (cmd.Find("i",0) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_I;
			}else if (cmd.Find("ad",0) != -1) {
				temp_sub_sub_cmd = SUB_SUB_CMD_AD(0);
			}else {
				ASSERT(0);
				TRACE("error!!!");
				return 0;
			}
			if (0 == save_sub_sub_cmd(temp_sub_sub_cmd, cmd, this_cmd_index + 1)) {
				return 0;
			}
		}else if (cmd.Find("=") != -1) {		// ��ǰ Set������֧�� pin���ţ���������չ�� DA������
			// ���� �磬P12
			//TRACE("=      ");TRACE(cmd);TRACE("\r\n");
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_SET;
			if (cmd.Find("da",0) != -1) {					// DA��ֵ����λ��mV   DA1 = 2000 mV
				temp_sub_sub_cmd = SUB_SUB_CMD_DA(0);
			}else if (cmd.Find("p",0) != -1 && cmd.Find("pow",0) == -1) { // (��pow����) MCU���� Pn = 1;
				temp_sub_sub_cmd = SUB_SUB_CMD_PIN(0);
			}else {
				ASSERT(0);
				TRACE("error!!!");
				return 0;
			}
			if (0 == save_sub_cmd_set(temp_sub_sub_cmd, cmd, this_cmd_index + 1)) {
				return 0;
			}
		}else if (cmd.Find("volto") != -1) {
			//TRACE("volto      ");TRACE(cmd);TRACE("\r\n");
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_VOLTO;
			if (0 == save_sub_cmd_multparam(cmd, this_cmd_index + 1)) {
				return 0;
			}
		}else if (cmd.Find("loadto") != -1) {
			//TRACE("loadto      ");TRACE(cmd);TRACE("\r\n");
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_LOADTO;
			if (0 == save_sub_cmd(cmd, this_cmd_index + 1)) {
				return 0;
			}
		}else if (cmd.Find("delay") != -1) {
			//TRACE("delay      ");TRACE(cmd);TRACE("\r\n");
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_DELAY;
			if (0 == save_sub_cmd(cmd, this_cmd_index + 1)) {
				return 0;
			}
		}else if (cmd.Find("start") != -1) {
			//TRACE("start      ");TRACE(cmd);TRACE("\r\n");
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_START;
		}else if (cmd.Find("stop") != -1) {
			//TRACE("stop      ");TRACE(cmd);TRACE("\r\n");
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_STOP;
		}else if (cmd.Find("topc") != -1) {			// �Ϸ���ָ��˴����� X
		}else if (cmd.Find("exe") != -1) {			// ģ��ִ��ָ����� 20180116��
			pc_cmd_bytes_arr[this_cmd_index] = SUB_CMD_EXE;
			if (0 == save_sub_cmd(cmd, this_cmd_index + 1)) {
				return 0;
			}
		}else {
			ASSERT(0);
			TRACE("error!!!");
			return 0;
		}
		cmd_num++;
	}
	pc_cmd_bytes_arr[CMD_NUM_INDEX] = cmd_num;
	// ���ݳ���Ϊ˫�ֽڱ�ʾ
	unsigned short data_num = cmd_num * 4 + 4;
	pc_cmd_bytes_arr[DATA_LENGTH_INDEX] = data_num >> 8;
	pc_cmd_bytes_arr[DATA_LENGTH_INDEX + 1] = data_num & 0xff;

	return MyCom.UartSendCmd(pc_cmd_bytes_arr, cmd_num * 4 + 7, 0);			// ���ô��ڷ���֡
}
extern unsigned char fg_normal_program ;			// 0: ����    1�����Ʊ��

extern unsigned char normal_sel_current ;
extern unsigned char normal_sel_speed ;
extern unsigned char normal_sel_low_vol ;
extern unsigned char normal_sel_low_vol_end ;
extern unsigned char normal_sel_dir ;
extern unsigned char normal_sel_time ;
extern unsigned char normal_sel_brake ;
extern unsigned char normal_sel_ntc1 ;
extern unsigned char normal_sel_ntc2 ;
extern unsigned char normal_sel_ntc3 ;
extern unsigned char normal_sel_ntc4 ;
// ������Կ�ѡ����
int send_normal_code()
{
	ZeroMemory(pc_cmd_bytes_arr, sizeof(unsigned char) * PC_CMD_BYTES_NUM);	// �� 0

	// ���ݳ���Ϊ˫�ֽڱ�ʾ
	unsigned short data_num = NORMAL_CODE_NUM + 4;
	pc_cmd_bytes_arr[DATA_LENGTH_INDEX] = data_num >> 8;
	pc_cmd_bytes_arr[DATA_LENGTH_INDEX + 1] = data_num & 0xff;

	pc_cmd_bytes_arr[MAIN_CMD_INDEX] = MIAN_CMD_NOMAL;
	pc_cmd_bytes_arr[CMD_NUM_INDEX] = NORMAL_CODE_NUM;
	
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 1] = normal_sel_current;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 2] = normal_sel_speed;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 3] = normal_sel_low_vol;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 4] = normal_sel_low_vol_end;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 5] = normal_sel_dir;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 6] = normal_sel_time;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 7] = normal_sel_brake;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 8] = normal_sel_ntc1;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 9] = normal_sel_ntc2;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 10] = normal_sel_ntc3;
	pc_cmd_bytes_arr[CMD_NUM_INDEX + 11] = normal_sel_ntc4;

	return MyCom.UartSendCmd(pc_cmd_bytes_arr, NORMAL_CODE_NUM + 7, 0);		// ���ô��ڷ���֡
}

//---------------------------------------------------------------------
#define NOTE_STRING_LENGTH_MAX	64   // ע��Ϊ���6�����ֵ��ֶ�
ITEM_CONTENT item_content[ITEM_JUGUE_CONTENT_NUM_MAX];
int Jugue_num = 0;
int total_item_num = 0;
extern int line_string_deal(CString str_line);

int modle_item_content(const CString& code)
{
	CString line_str = "";
	int huiche_index = -2;
	int str_len = code.GetLength();

	Jugue_num = 0;
	total_item_num  = 0;

	while(1) {
		int start = huiche_index + 2;
		if (start < str_len) {
			if ((huiche_index = code.Find("\r\n", start)) != -1) {

				line_str = code.Mid(start, huiche_index + 2 - start);
			}else {
				line_str = code.Mid(start, code.GetLength() - start);
			}
			//TRACE(line_str);TRACE(">>");
			// �������ַ���
			if (line_string_deal(line_str) == 0) 
				return 0;
			if (huiche_index == -1) break;
		}else {
			break;
		}
	}
	// �ع��б�
	pCurrView->re_build_main_list(total_item_num, MAIN_LIST);
	return 1;
}

extern int GetDiandongToolTestItem(CString& str_comment, int num);
int line_string_deal(CString str_line)
{
	CString cmd = "";
	CString item_temp = "";
	int xiegang_index = 0,huiche_index = 0;

	// 1��ȥ"����������"ע��		"//"
	if ((xiegang_index = str_line.Find("//", 0)) != -1) {
		if ((huiche_index = str_line.Find("\r\n", xiegang_index)) == -1) {
			huiche_index = str_line.GetLength();
		}
		if(xiegang_index + 2 < str_line.GetLength() && huiche_index - xiegang_index - 2 > 0) {
			if (huiche_index - xiegang_index - 2 > NOTE_STRING_LENGTH_MAX) {
				item_temp = str_line.Mid(xiegang_index + 2, NOTE_STRING_LENGTH_MAX);
			}else {
				item_temp = str_line.Mid(xiegang_index + 2, huiche_index - xiegang_index - 2);
			}
			item_temp.Remove(' ');
			//TRACE(item_temp);TRACE("\r\n");
#ifdef AUTO_TOOL_18V_TEST
			if (0 == GetDiandongToolTestItem(item_temp, Jugue_num))
				return 0;
#endif
		}

		str_line.Delete(xiegang_index, huiche_index - xiegang_index);
	}
	// 1��ȥ����ע��		"/* -- */"
	while(1) {
		if ((xiegang_index = str_line.Find("/*", 0)) != -1) {
			if ((huiche_index = str_line.Find("*/", xiegang_index + 2)) != -1) {
				str_line.Delete(xiegang_index, huiche_index + 2 - xiegang_index);
			}else {
				ASSERT(0);
				TRACE("error!!!");
				return 0;
			}
			
		}else {
			break;
		}
	}
	
	// 2��ȥ�����ַ�
	str_line.Remove(' ');
	str_line.Remove('\r');
	str_line.Remove('\n');
	str_line.Remove('\t');
	str_line.MakeLower();

	int num_temp = 0;
	while (str_line.GetLength() > 0) {
		cmd = str_line.SpanExcluding(";");
		int fenhao_number = str_line.Find(";");
		if(fenhao_number != -1) { 
			str_line.Delete(0,fenhao_number +1);
		}else {
			ASSERT(0);
			TRACE("error!!!");
			return 0;
		}

		// ���µ��жϴ����ܱ䶯, �����¼���ָ��Ҳ��Ҫ����˳������
		if (cmd.Find("while") != -1) {
		}else if (cmd.Find("==") != -1 || cmd.Find(">") != -1 || cmd.Find("<") != -1 || cmd.Find("!=") != -1) {
			// ��������JUGUEΪhalf״̬������ٽ����һ��JUGUE��fg = 1;
			item_content[Jugue_num].fg_half = 0;
			item_content[Jugue_num].item_content = item_temp;
			num_temp++;
			if (++Jugue_num >= ITEM_JUGUE_CONTENT_NUM_MAX) break;
		}
	}
	// �ж��﷨
	if (num_temp == 0 && item_temp != "") {		// ע��д���˷��ж������
		ASSERT(0);
		TRACE("error!!!");
		return 0;
	}
	// ����е����һ��JUGUEָ����good & ng��Ҫ�����ġ�Ϊһ����������item
	if (num_temp > 0 && item_temp != "")	{
		item_content[Jugue_num - 1].fg_half = 1;
		total_item_num++;
	}
	//����֮ǰ��half
	if (item_temp != "") {
		int i = Jugue_num - 2;
		while (i >= 0) {
			if (item_content[i].item_content == "") {
				item_content[i].item_content = item_temp;
			}else {
				break;
			}
			i--;
		}
	}
	return 1;
}
#endif
