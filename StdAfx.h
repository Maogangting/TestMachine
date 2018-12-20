// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__39417159_89BB_48B9_AA25_5E44DB301DB2__INCLUDED_)
#define AFX_STDAFX_H__39417159_89BB_48B9_AA25_5E44DB301DB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define MAX_USER_NUM			1024	// ���ͻ����߳����ɵ�5000��ǰ��64K��ջ��
//#define MGT_CLOCK				// ģ��ʱ��
//#define SIMULATE_TOPC			// ģ����λ��MCU�Ϸ����Խ������
#define SYS_SHOW_TEST			// ������ʾģʽ
//#define SYS_SERVER
#ifdef SYS_SERVER
	#define MGT_SOCKET			// ����ú��ʾ֧����̫��ͨ�� ������ˣ�
	#define SYS_THREAD_POOL		// �̳߳ؼ���
#endif

//#define AUTO_TOOL_18V_TEST		// ��� �綯���߲���

#define SYS_CLEINT
#ifdef SYS_CLEINT
	//#define MGT_SOCKET_CLIENT	// ����ú��ʾ֧����̫��ͨ�� ���ͻ��ˣ�
	//#define CLIENT_HTTP			// �����Խ������ͨ��HTTP�����Ϸ�����������̨
#endif

	
#define DATA_BACE				// ����ú��ʾʹ�� SQL SERVER 2008 ���ݿ�
#define WINDOW_MAX				// ���ڽ������
//#define _NO_ACK				// ����ú��ʾ����ͨ�Ų���Ҫ ACK

#ifdef DATA_BACE
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
#endif

#if defined(SYS_CLEINT) && !defined(DATA_BACE)
	#define LOCAL_EXCEL_SAVE
#endif

#define CODE_PART	// ���Ʋ��ֺ�
#ifdef CODE_PART

#define DB_SEGMENT_ITEM_NUMMAX	20		// �������20�������Ϊ�����ݿ��ļ�����
#define UART_BAUD_RATE			9600	// ���ڲ�����
//��1, �����������á�
// �������´������η��ͣ����øı䷢���Ⱥ�˳�򣬲���Ҫ����λ��ͳһ�Ⱥ����
#define BACE_PARAM_NUM	20		// ������������
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
//	18�����������
//  19,���ѹ
//  20,����
// ��... ... ��������ͬʱ�޸ĺ� BACE_PARAM_NUM ��ֵ��

//  ���ݲ���Э�飺 ��������1byte,index:03�� + ���ֽ����� +  ���ֽڹ���    ����4�ֽ�ÿ��������

// ���������ͣ�
#define MAIN_CMD_PARAM			0x00//��������
#define MIAN_CMD_NOMAL			0x01//���Գ����ѡ����			
#define MIAN_CMD_PROGRAM_OTHER	0x02//���Զ�������			
#define MIAN_CMD_PROGRAM_START	0x03//���Ʋ����е���������				
#define MIAN_CMD_PROGRAM_STOP	0x04//���Ʋ����е�ͣ������				
#define MIAN_CMD_STOP			0x05//ֹͣ����				
#define MIAN_CMD_SELFCHECK		0x06//�������԰��Լ�		
#define MIAN_CMD_RESET			0x07//��������				
#define MIAN_CMD_SEND_REV1		0x08//�·���������1			
#define MIAN_CMD_SEND_REV2		0x09//�·���������2			
#define MIAN_CMD_FAULTRET		0x0a//�����ϱ�				
#define MIAN_CMD_CHECK_RET		0x0b//�Լ��ϱ�				
//#define MIAN_CMD_TOPC			0x0c//�ظ�ToPC()			
#define MIAN_CMD_REV1			0x0d//�ϴ���������1			
#define MIAN_CMD_REV2			0x0e//�ϴ���������2			
#define MIAN_CMD_REV3			0x0f//�ϴ���������3			
#define MIAN_CMD_ACK			0x55//ACK	

// ��2�����Ʋ��ֱ�̡�
// һ��������
//						����ָ��
//	Px = 1				// x�����ߣ� DA��ֵ     ��ע����A/D , DA ���Ƶ�����ֵ��Ϊ��ѹֵ����λ��mV
//	==					// ��OK�� or ��NG������жϣ������ж����š���ѹ��������ת�١�ĳ��A/Dֵ��
//	delay(t)			// ��ʱ t ms
//
//	LoadTo(L)			// ���ƺ���Դ���ص�L
//	VolTo(x)			// ���Ƶ�Դ������ѹ��x V
//	While(XXX) ��		// �����жϣ��ȴ������� XXX�����㣻���������ǣ����š���ѹ��������ת�١�ĳ��A/Dֵ��
//	Start				// ����      ������ִ��startʱ���⣬������һЩ����׼���������磬�򿪼̵����ȣ�
//	Stop				// ͣ��     ���ر����Զ��ŵ硢�ϱ�PC�����������ϵ硢[�豸��λ]��������β���ˣ�
//	ToPC(x)				// ��x�ļ��ֵ�Ϸ���PC
#define SUB_CMD_SET			0			// =
#define SUB_CMD_JUDGE		1			// ==
#define SUB_CMD_DELAY		2			// delay
#define SUB_CMD_LOADTO		3			// loadto
#define SUB_CMD_VOLTO		4			// volto(2,2000) // 1����ѹ����Դ��2����ѹ��Դ��3����ѹ��Դ
#define SUB_CMD_WAITUNTIL	5			// while(...)
#define SUB_CMD_START		6			// start
#define SUB_CMD_STOP		7			// stop
//#define SUB_CMD_TOPC		8			// to pc
#define SUB_CMD_EXE			9			// exe(id)  ִ��ģ��

/* һע�� ��һ���������ֽڵ���ɣ� ����4bits��:  �����ָ�����ݲ���ֵ�Ĺ���:
					(�ٷֱȣ�0.5%~8%���RPM/AD/DA;��СUnit=0.5%,�磬��4λ����10�����5%)��
					(����ֵ��0.5~8��Ե���/��ѹ;��СUnit=0.5(A or v),�磬��4λ����10�����5)�� // ���������������ִ��
					��̸�ʽΪ����ֵ[����],�磺  while(i==20[1]);		// ����0.5A
                              ����4bits��:  ��������һ���������֧��16��ָ�Ŀǰ8��ָ�
   ��ע���ж� >  <  == �����֣����ƣ�RPM > 2000; or  while(i > 20)��������λ���ݵ������λ�����£�
	#define _equl		0	// ==
	#define _more		1	// >
	#define _less		2	// <
	#define _not_equl	3	// !=
   ��ע��  ������������Ϊ RPM����ʵ��ת��Ϊ���յ�����ֵ�Ŵ�10��
   ��ע��  ʵ��ָ����ʱ��[����]���������ݸ��ں��棬[]�ڵĹ������ݶ��ǰ��վ���ֵ��д�������ڲ�����ת����
*/
#define _equl		0
#define _more		1
#define _less		2
#define _not_equl	3

#define SAOMA_ID_LEN	30	// ɨ��ID���30λ����ʽ�磬	NY161130016-00930


// ����������
// ����(Pn)��ĸ�ߣ�Ĭ�ϣ���ѹ(Vol)���ܵ���(I)��ת��(rpm)��ĳ·A/Dֵ (ADn)�ȡ���������������ע����A/D , DA ���Ƶ�����ֵ��Ϊ��ѹֵ����λ��mV
#define MCU_PIN_MAX				144	// ����MCU���64pin
#define MCU_PIN_START			0	// MCU pin�����������0��ʼ
#define SUB_SUB_CMD_PIN(n)		(MCU_PIN_START + n)	// 0~64����pin����������	
#define SUB_SUB_CMD_VOL			(MCU_PIN_MAX + 1)
#define SUB_SUB_CMD_I			(MCU_PIN_MAX + 2)
#define SUB_SUB_CMD_RPM			(MCU_PIN_MAX + 3)
#define SUB_SUB_CMD_UART_VOL_NG (MCU_PIN_MAX + 4)	// ����ָ�uartvol == 0;  /* �޹���  0: ������1��Ƿѹ��2����ѹ*/
#define SUB_SUB_CMD_POWER		(MCU_PIN_MAX + 5)	// ����ָ�pow == 1000[����%];  /* 1000 ��ʾ��׼���� ����λ��kw  �� �й���*/
#define SUB_SUB_CMD_DIR			(MCU_PIN_MAX + 6)	// ����ָ�dir == 0(Ĭ��) ���� dir == 1����ת��;
// #define ...
// �����Ķ�����������ӵ��˴���SUB_SUB_CMD_PIN_END ֮ǰ������ ֵ < SUB_SUB_CMD_PIN_END �� (MCU_PIN_MAX + 10)
#define SUB_SUB_CMD_PIN_END		(MCU_PIN_MAX + 10)

#define SUB_SUB_CMD_AD(n)			(SUB_SUB_CMD_PIN_END + n)		// ��n·AD
#define SUB_SUB_CMD_AD_END			(SUB_SUB_CMD_AD(0) + 30)		// ���30·AD֧��
#define SUB_SUB_CMD_DA(n)			(SUB_SUB_CMD_AD_END + n)		// ��n·DA
// *ע�� ����(n) n = 0,1,2,3 ...  ��0��ʼ  P0����MCU��һ�����ţ�A/D��D/AҲ����ˡ�

// ͨ��Э�飺
// 0xfa		len						cmd	........................checksum	0xf0	
// ��ʼλ	���ݳ��ȣ�Ŀǰ<256��	��������	�����������	У���		����λ

//	* ���У�len�Ǵ�len����ʼ����checksum ǰһ���ֽ�Ϊֹ����������֡������λ0xfa,0xf0 �Լ� checksum֮������ݸ�����
//  * ���У�checksum ��ֵ����len���������ۼ� ^ ����Ľ��  ����������������

// index:   02          03							04
// ����ʱ��(0x02)	ָ����Ŀ(Ŀǰ��֧�ֳ���<256)	ָ��1(һ��������)	ָ��1���ݸ��ֽڣ����������	ָ��1���ݵ��ֽ�(���ݸ�λ)	ָ��1����ĩ�ֽڣ����ݵ�λ��;  ָ��2  ... ...
#define DATA_LENGTH_INDEX	1	// 2018/1/8  ���ݳ��ȸ�Ϊ˫�ֽڣ���ˣ������ؼ���index ȫ��+1

#define MAIN_CMD_INDEX	3
#define CMD_NUM_INDEX	4
#define SUB_CMD_INDEX	5
#define SUB_CMD_BYTES_NUM	4	// һ������������ 4���ֽ�
#define this_cmd_index	(SUB_CMD_INDEX + cmd_num * SUB_CMD_BYTES_NUM)	// ��ǰ���������������������ַ
#define PC_CMD_BYTES_NUM	(10*1024)		// 10K
	
// ��3��������Կ�ѡ���֡�
#define NORMAL_CODE_NUM		11	//  �������ѡ�� �� 11��
// ����Э���еķ��ʹ����ܸ��ģ�ֻ��β����������಻���� DZ_FAULT_START_MACRO 50��
//  ����Э�飺��������1byte ,index:03�� + ������1 byte�� = 0����ѡ�У� ���� 1��ѡ�У��� + ת�� + Ƿѹ + Ƿѹ�ָ� + ת�� + ����ʱ�� + ɲ�� + NTC���½�ֹ + NTC���»ָ� + NTC���½�ֹ + NCT���»ָ� + ... ...����������
#endif		// CODE_PART


// ��4��To PC ����Э�顿
/*	��Э��:			�����ļ�����topc������Ϣ��ͨ�ó�����ԺͶ��Ʋ��ԣ�
**	0xfa   +   �ֽ���(1byte)     +   �ϱ�����(1byte)    +   ���� ��n bytes��   + checksum  + 0xf0
**	�����У��ϱ������У�  1�����Խ���ϱ���0x00 ����0x01 ������    2��ϵͳ�Լ��ϱ���0x02��   3��xxxxxxxx ������������

**	1�����Խ���ϱ���������һ����������һ�����Ϸ�ȫ�����Խ�����������벻���� 
**	0xfa  +  �ֽ�����   +  0x00/0x01   +  ID��Ĭ��0x00000000, 4 bytes��+  ������Ŀ��1byte�� + [�������1(1 byte) + ����1����(2 bytes) + ����1���(2 bytes��������31bit,���ౣ��)] + [����2���(2 byte) +  ... ...] +   checksum   +  0xf0
ע�� 1)��0x00/0x01  0:����1:����  
	2)��������:0����������1�������2�����⣨������
	3)����������硰 ���ϲ������͡��궨��

**	2, ϵͳ�Լ��ϱ���������	// * ע��ֻ�Ϸ�������
**	0xfa  +  �ֽ�����   +  0x02   +   [�豸��(1 byte) + ������(1 bytes �磬ͨ�Ź��ϡ����ȹ��ϡ��������ϵ�)]   +   checksum   +  0xf0
**	*ע��1��[]���ǿ�ѡ�2���Ϸ���һ֡����ֽ��� < REV_FLAME_SIZE_MAX (256 bytes)
*/

// �ϱ�����
#define		TEST_GOOD			0x00
#define		TEST_NG				0x01
#define		SELFCHECK_FAULT		0x02

// ���ϲ������� �� NORMAL_CODE_NUM ������� ���⻹�ж��Ʋ��ֲ�������
/////////////////////////////////////////////////////////////////////////////////////
/*���������Լ���Ӧ����������*/
#define FAULT_CURRENT		0		// "��������"
#define FAULT_SPEED			1		// "ת�ٲ���"
#define FAULT_LOW_VOL		2		// "Ƿѹ����"
#define FAULT_LOW_VOL_STOP	3		// "Ƿѹ�ָ�"
#define FAULT_DIR			4		// "ת����"
#define FAULT_START_TIME	5		// "����ʱ�䲻��"
#define FAULT_BRAKE			6		// ɲ������
#define FAULT_NTC_GJ		7		// "NTC���½���"
#define FAULT_NTC_GH		8		// "NTC���»ָ�"
#define FAULT_NTC_DJ		9		// "NTC���½���"
#define FAULT_NTC_DH		10		// "NTC���»ָ�"
#define FAULT_POWER			11		// ��ѹ���ʻ�
// �������ּ��ڴ˴�
// ...
//���Ʋ��֣��������ϲ����⣬���б��磺PIN�ż�����A/D������Ȳ����� (n) n��0��ʼ
#define DZ_FAULT_START_MACRO	50
#define FAULT_DZ_PIN(n)			(DZ_FAULT_START_MACRO + n)					// "��n�����źŲ���"
#define FAULT_DZ_AD(n)			(DZ_FAULT_START_MACRO + MCU_PIN_MAX + n)	// "��nͨ��AD��������"

// �ṹ��ѹ������
#pragma pack(1)
struct ITEM_INFO {					// ÿ�����������Ϣ��5 bytes
	unsigned char item_type;		// �������ͣ��磺��������ѹ��  �������  �����ϲ������͡� ����
	unsigned short item_value;		// ����ʵ�ʲ���ֵ
	unsigned short item_reserv;		// ����2�ֽ� (���bit  0: ��������1�������)��ʣ���ֽڱ���
};

// ֡�ṹ��
struct TEST_INFO{
	unsigned char frame_head;
	unsigned char frame_data_num;
	unsigned char to_pc_type;	// TEST_GOOD/TEST_NG/SELFCHECK_FAULT	
	char saoma_id[SAOMA_ID_LEN];// ��ʽ���磺 NY161130016-00930������ĩβ�����λ��0
	char item_num;				// �����������
	ITEM_INFO item_info[1];		// ��ʵ��n��,ֻ��Ϊ�˻�ȡ������Ϣ����Ļ�ַ
};
#pragma pack()

#define TO_PC_FRAME_MAX	256		// ֡�������ֽ������ֽڱ�ʾ����Ҫ��չΪ���ֽڱ�ʾ��
#define MAX_ITEM 49				// == ((TO_PC_FRAME_MAX - 10) / sizeof(ITEM_INFO))

// ACK���ݰ�
//	0xfa + 1(�ֽ���) + 0x55(CMD) + 0x57(checksum) + 0xf0          

// ��������������
// 1, ���������ԡ���2������Ʒ�� ���� ��XXX������Ҫ��xx��ʵ��xxx��

#define pCurrView	((CMenuTestView*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView())
#define CODE_FILE ("E:\\code.txt")
#define HIS_FILE  ("E:\\histroy.txt")
//#define SETTING_FILE	("E:\\setting.txt")
#define LOCAL_EXCEL_FILE ("E:\\test recoder.xls")

#define HIS_FILE_NUM  100
struct HISTROY_FILE{
	int ID;
	CString histroy;
};//histroy_file[HIS_FILE_NUM];
//int top = 0;
#define ID_BACE 317461023

#define FIRST_COL_NUM	4
#define LAST_COL_NUM	5

// list control type
enum {
	MAIN_LIST = 1,
	NG_LIST   = 2,
};

#define beat_string	"abc#@123"	// socket�������ַ���

// �����ݽṹ���� 1��1���[���öԻ��������]
//��Ҫ��������﷨��
/*
**	P12 = 1; && P8 = 0;		// && ͬʱ����˼
**  delay(20);
**  P8 = 1;
**  delay(200);
**  P1 == 0; && P2 == 0; && P3 == 1;
**  delay(1000);
**  P12 = 0; && P8 = 0;
*/

struct PARA_SETTING {
/*
** ������������
*/
	/*int*/CString LimitCurrent;		// ����
	/*int*/CString Speed;				// ת��
	/*int*/CString LowVol;				// Ƿѹֵ
	/*int*/CString LowVolEnd;			// Ƿѹ�ָ�
	/*int*/CString fgDirection;			// ת�� 1��������ʱ��  0������˳ʱ��
	/*int*/CString fgBrake;				// �Ƿ��ɲ��  n>0��ɲ��ʱ��   0����
	/*int*/CString StartTime;			// ����ʱ��

/*
** ���NTC		DA����� ��λ��mV
*/
	int fgNTC;							// �Ƿ���Ҫ��ذ�NTC		
	/*float*/CString TopEnd;			// ���½���
	/*float*/CString TopStart;			// ���»ָ�
	/*float*/CString BotEnd;			// ���½�ֹ
	/*float*/CString BotStart;			// ���»ָ�

/*
**	���������Ĺ���ֵ
*/
	/////////////////////////////////////////////////////
	CString		deta_current;
	CString		deta_speed;
	CString		deta_low_vol;
	CString		deta_low_vol_end;
	CString		deta_start_time;
	// ntc����
	CString		deta_ntc;
	/////////////////////////////////////////////////////

/*
**	��������
*/
	CString		motor_pole;			// ���������
	CString		normal_vol;			// ���ѹ
	CString		other;				// ����
/*
** ��������
*/
	CString  dzStart;		// �������ر���ַ���


/*
** ͣ������
*/

	CString  dzStop;		// ͣ������ַ���

/*
** ��������
*/
	CString  dzOther;		// �������Ʊ���ַ���

};

// ����itemģ�黯����
typedef struct {
	short	fg_half;	// 0:good����  1��good & ng ��Ҫ��
	CString item_content;
}ITEM_CONTENT;
#define ITEM_JUGUE_CONTENT_NUM_MAX	256

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������

/*
����>>

ë>>		 1,NTC����ֵ�ߵ�������  3 ������Դ������ݿ���ֶκ�ֵ���� 4���������ʱ���Զ���last�����ļ�(����ʱ����)���·���MCU
			5����������

1�� ��֮ǰ���ļ�ģʽ��Ϊ���ݿ�ģʽ���½���
// create table T_flow_step_def(
// Step_no    int not null,     --���̲���ID 
// Step_name    varchar(30)    not null, --���̲������� 
// Step_des    varchar(64)    not null,    --���̲�������
// Limit_time    int not null,     --ʱ��
// URL     varchar(64)    not null,     --�����˵����� 
// ��ע    varchar(256)    not null, 
// )
*/

// ����֮�� >>
// 1,�����󣬺ͷ������˶�����汾���磬PC��������������
// 2�������ն˲��Ի���λ��ֱ�Ӵ���ҳ���ʷ�����������������ϻ�������һ�����ճ���д�뵽���ݿ�
// 3���ͻ�������Զ���������,��λ������������ϴ��ͻ��˰汾��&���Գ���汾�ŵ���������
// 4���ն������Ϸ�����ʱ�����ر����һ���������������ϴ��Ļ�����Ҫ���롣
//

/*__________________________________________________________________________________
// ��ʱ������ >>
// 1�����Ʊ�̣�����ָ� if(  == ) {} ָ�����.--------����ʱ�����ǡ�
// 2��ģ����� EXE(id);(���� id��Ϊ����������е�����) ����ʱ�����ǣ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������
*/

#endif // !defined(AFX_STDAFX_H__39417159_89BB_48B9_AA25_5E44DB301DB2__INCLUDED_)
#include <afxcontrolbars.h>

// �޸ļ�¼
/*
20180303��	1���޸���ID���ȣ���ʽ�磬	NY161130016-00930
			2��ԭ���ݿ��װ�����У����� .vt == 2 // smallint ����� ��Ȼ���� int/smallint����תCString������⡣
			3, CListCtrol�������е�������ܣ��Զ���һ��LIST�࣬�̳�CListCtroll��
20180304:   1,������ģ��ʼ�ա�2,�޸���Ӧ�ó���ͼ�� 3���������б��Ҽ������˵����ܡ�
20180305��	1��socket��rev()��������ֵ���жϣ�������0 == retVal�Ĵ�����һ���ж���ͨ�š�
			2,�޸���server socket �ļ����߼��Լ�client socket���֣�����������ص����ӶϿ����ԣ�����OK��
20180306	1,�޸��˷�������recv��send�ĳ�ʱ���ã������������������ƣ���ֹ�ͻ���������߶����·����socket����
20180309	������������Ҫ�Ż��ĵط���
			1��������Ŀǰ��ջ�ڵ�2048���ֽڻ�������   OK
			2������˷�����������  ����ǰ�ͻ��˷�����ȫ����ģʽ��XXXXX OK
			3������������Կͻ��˷����ļ�����������һ������    XXXXX OK
			����Ե�ǰģʽ�Ŀͻ��ˣ����ݷ���Ƶ�ʲ��ߣ����ݳ���Զ<1024�������ڸ��������������Ե����ͻ��˸�Ƶ�ʷ���ʱ����ʹ�ý��ն˻��淽ʽ��
			4���̳߳�ʵ��----�ص�   OK
			5,����ռ�õ�Socket���رպ󣬻��Ǳ��������ӡ�   OK
			6������������������listen�Ļ��ƣ��ٿ����£����listen��һ�Ρ�ʵ����Χ�Ŷӹ��ܡ����������ȴ��� ������

		   |||||||*7��socketͨ������ɶ˿ڻ����첽select�ķ�ʽʵ�� ???????????????????????????????????||||||||||
		   *8����������Ҫ�������ͻ��˵����ݸ��źͶ����������⡣�����ݸ��ſ���ͨ���ض�����Э����߿�����ɣ��������ӿ�����Ҫͨ������ǽ������������Э��ı�־λ��ɣ�

20180314	1,�������̳߳أ�2��ԭ���ղ��ֵĻ����ɶ�ջ���ٸ�Ϊ��̬malloc���٣�3������˿���ռ�õ�Socket���رպ󣬻��Ǳ��������ӵ����⡣

			?? ״̬����ʾ���Ż� ??
20180319    1���������˲���1000���ͻ���ͬʱ�������ӷ��͸����ݣ����������⡣2,������ listen()���ŶӸ�����ԭ����1����Ϊ20����3,CPFile���� if (value.vt == 1) // NULL  ����Ȼ���Ի��ϱ���
20180320	1, ��thread pool init����ֵ�������жϣ���ֹ�߳̿���ʧ�ܣ�  2�� ��socket�̵߳Ķ�ջ��С������64K,ʹ�ÿ��Կ���5000���߳����ϣ�
20180321	1���������˲���5000���ͻ���ͬʱ�������ӷ��͸����ݣ���closesocket�������ӣ����Ի��������⡣
20180423    1,�޸���volto������������˿�ѡ��Դ�豸�Ĳ������磺volto(2,2000);
20180427	1���޸���ɨ���ַ���֧�����29���ַ�
20180428	1���޸��˴��ڸ������ںź��շ���������
20180514	1������� SUB_SUB_CMD_UART_VOL_NG �� SUB_SUB_CMD_POWER��������ָ��,�������Ӷ�����������Ҫ�޸ĵĵط���
			1���ο� RPM ��2���������޹���   3�������Ƿ��и�ֵ����   4��������ʾʵ�ʲ���ֵ 5) ����index�������磺P10��AD2
20180606	1) UartSendCmd()�����ڲ����� i ������ UINT8 ��Ϊ int ; // ���͵����ݸ����Ǵ���256�ģ�Ŀǰ�������������ֽڡ�
20180608	1) ����� SUB_SUB_CMD_DIR ����ָ����Դ�����Ա��ʵ��ת����жϣ����Դ���������dirָ���Ĭ�ϼ�����ǰ��������ʱ�룬����dir���յ�ǰ���Դ���ʵ��ת���ж�
20180609	1) ����� AUTO_TOOL_18V_TEST �����ڵ綯���߲����ξߣ���ʱ����Ŀ������λ������������1��ѡ����Դ����ļ���2�����ô��ڣ�3�������������
20180613	1) ����� LOCAL_EXCEL_SAVE ��Ҫ��Ե綯������λ��û��SQL���ݿ�����£����������ݼ�¼��EXCEL ��ʽ�����ڱ���E���ϡ�
20180621	1) ����˲��Խ��ͼ����ʾ��
20180629	1) ��Ե綯����������ÿ�������ʵ��ֵ����ʾ��2���Ż��˲��Խ��ͼ��������ʾ���⡣
20180727	1) HTTP����   ����ͨ����CHttpClient http_client�� �����Ѳ��Խ����Ϣ�Ϸ���̨���嶯�����ϡ�
20180729	1) ʵ���˾���Ĳ�������ͨ��http post������ JSON ��ʽ�Ϸ����������ķ����������γɵ�JSON�ַ�������ͨ����
20180814    1) ʵ���˵綯���߳ɲ��⹦�ܣ�����ֱ��ɨ��ǹ���ڽ�����ԣ���ȡɨ��ID�Ͳ��Խ���ַ�����
20180817    1) �綯���߳ɲ��⹦������BUG�޸���ϡ�
20181116    1��ReadCmd()�����ڲ�if (RdIndex < WrIndex) -->   if (RdIndex +1 < WrIndex) {
20181205	1)DataToRevBuf()���� �������� if (WrIndex + DataNum < COM_REV_BUF_SIZE) ��ֹ����֮���մ������ݲ�������������ڴ����
*/

/* 20181220 ���л�����һ����Ʒ��Ҫ����ͳɲⶼ�������������װ��ʱ���ÿ����Ʒ�Ƿ񶼾������������Թ�������⣺
	������
	1������ͳɲ�ÿ�ζ�д���ݿ⣬ͨ��chengche��־ָ�����ĸ��׶εĲ��ԣ�
	2�����ݿ�Զ�����ӵ������ƣ�Ҫ�������ӵļ���Ժ�������
	3��װ��ʱû�б�Ҫ�ٴ�ɨ�룬ͨ�����ɾѡȷ���Ƿ�����������δȫ�Ĳ�Ʒ���ɡ�
*/