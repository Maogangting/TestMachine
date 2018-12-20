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

#define MAX_USER_NUM			1024	// 最大客户端线程数可到5000（前提64K堆栈）
//#define MGT_CLOCK				// 模拟时钟
//#define SIMULATE_TOPC			// 模拟下位机MCU上发测试结果数据
#define SYS_SHOW_TEST			// 定义演示模式
//#define SYS_SERVER
#ifdef SYS_SERVER
	#define MGT_SOCKET			// 定义该宏表示支持以太网通信 （服务端）
	#define SYS_THREAD_POOL		// 线程池技术
#endif

//#define AUTO_TOOL_18V_TEST		// 针对 电动工具测试

#define SYS_CLEINT
#ifdef SYS_CLEINT
	//#define MGT_SOCKET_CLIENT	// 定义该宏表示支持以太网通信 （客户端）
	//#define CLIENT_HTTP			// 将测试结果数据通过HTTP请求上发到服务器后台
#endif

	
#define DATA_BACE				// 定义该宏表示使用 SQL SERVER 2008 数据库
#define WINDOW_MAX				// 窗口界面最大化
//#define _NO_ACK				// 定义该宏表示串口通信不需要 ACK

#ifdef DATA_BACE
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
#endif

#if defined(SYS_CLEINT) && !defined(DATA_BACE)
	#define LOCAL_EXCEL_SAVE
#endif

#define CODE_PART	// 定制部分宏
#ifdef CODE_PART

#define DB_SEGMENT_ITEM_NUMMAX	20		// 定义最多20个测试项，为了数据库表的兼容性
#define UART_BAUD_RATE			9600	// 串口波特率
//【1, 基本参数配置】
// 按照以下次序依次发送，不得改变发送先后顺序，并且要与下位机统一先后次序。
#define BACE_PARAM_NUM	20		// 基本参数个数
//	1，	限流
//	2,	限流公差
//  3,	转速
//	4， 转速公差
//	5,	欠压
//	6，	欠压公差
//	7,	欠压恢复
//	8,	欠压恢复公差
//	9,	转向
//	10,	软启时间
//	11,	软启时间公差
//	12,	刹车
//	13,	NTC高温截至
//	14,	NTC高温恢复
//	15,	NTC低温截至
//	16,	NTC低温恢复
//	17，NTC公差
//	18，电机极对数
//  19,额定电压
//  20,保留
// （... ... 可增补，同时修改宏 BACE_PARAM_NUM 的值）

//  数据部分协议： 种类数（1byte,index:03） + 两字节数据 +  两字节公差    （共4字节每个参数）

// 主命令类型：
#define MAIN_CMD_PARAM			0x00//基本参数
#define MIAN_CMD_NOMAL			0x01//测试常规可选配置			
#define MIAN_CMD_PROGRAM_OTHER	0x02//测试定制配置			
#define MIAN_CMD_PROGRAM_START	0x03//定制测试中的启动定制				
#define MIAN_CMD_PROGRAM_STOP	0x04//定制测试中的停机定制				
#define MIAN_CMD_STOP			0x05//停止测试				
#define MIAN_CMD_SELFCHECK		0x06//启动测试板自检		
#define MIAN_CMD_RESET			0x07//测试重置				
#define MIAN_CMD_SEND_REV1		0x08//下发备用命令1			
#define MIAN_CMD_SEND_REV2		0x09//下发备用命令2			
#define MIAN_CMD_FAULTRET		0x0a//故障上报				
#define MIAN_CMD_CHECK_RET		0x0b//自检上报				
//#define MIAN_CMD_TOPC			0x0c//回复ToPC()			
#define MIAN_CMD_REV1			0x0d//上传备用命令1			
#define MIAN_CMD_REV2			0x0e//上传备用命令2			
#define MIAN_CMD_REV3			0x0f//上传备用命令3			
#define MIAN_CMD_ACK			0x55//ACK	

// 【2，定制部分编程】
// 一级子命令
//						九条指令
//	Px = 1				// x脚拉高， DA赋值     【注】：A/D , DA 定制的输入值均为电压值，单位：mV
//	==					// ‘OK’ or ‘NG’结果判断，可以判断引脚、电压、电流、转速、某个A/D值等
//	delay(t)			// 延时 t ms
//
//	LoadTo(L)			// 控制恒流源加载到L
//	VolTo(x)			// 控制电源调整电压至x V
//	While(XXX) ；		// 条件判断，等待“条件 XXX”满足；条件可以是：引脚、电压、电流、转速、某个A/D值等
//	Start				// 启动      （除了执行start时序外，还包括一些开机准备工作，如，打开继电器等）
//	Stop				// 停机     （关闭马达、自动放电、上报PC、给控制器断电、[设备复位]、其他结尾事宜）
//	ToPC(x)				// 将x的检测值上发到PC
#define SUB_CMD_SET			0			// =
#define SUB_CMD_JUDGE		1			// ==
#define SUB_CMD_DELAY		2			// delay
#define SUB_CMD_LOADTO		3			// loadto
#define SUB_CMD_VOLTO		4			// volto(2,2000) // 1：低压主电源，2：半压电源，3：高压电源
#define SUB_CMD_WAITUNTIL	5			// while(...)
#define SUB_CMD_START		6			// start
#define SUB_CMD_STOP		7			// stop
//#define SUB_CMD_TOPC		8			// to pc
#define SUB_CMD_EXE			9			// exe(id)  执行模块

/* 一注： 该一级子命令字节的组成： 【高4bits】:  代表该指令数据部分值的公差:
					(百分比：0.5%~8%针对RPM/AD/DA;最小Unit=0.5%,如，高4位若是10则代表5%)，
					(绝对值：0.5~8针对电流/电压;最小Unit=0.5(A or v),如，高4位若是10则代表5)， // 超过按照最大量程执行
					编程格式为：数值[公差],如：  while(i==20[1]);		// 公差0.5A
                              【低4bits】:  代表上述一级子命令，共支持16条指令（目前8条指令）
   二注：判断 >  <  == 的区分（类似：RPM > 2000; or  while(i > 20)），看高位数据的最高两位，如下：
	#define _equl		0	// ==
	#define _more		1	// >
	#define _less		2	// <
	#define _not_equl	3	// !=
   三注：  若二级子命令为 RPM，则实际转速为接收到数据值放大10倍
   四注：  实际指令编程时，[公差]紧挨着数据跟在后面，[]内的公差数据都是按照绝对值填写，程序内部进行转化。
*/
#define _equl		0
#define _more		1
#define _less		2
#define _not_equl	3

#define SAOMA_ID_LEN	30	// 扫码ID最多30位，格式如，	NY161130016-00930


// 二级子命令
// 引脚(Pn)、母线（默认）电压(Vol)、总电流(I)、转速(rpm)、某路A/D值 (ADn)等。。（可增补）【注】：A/D , DA 定制的输入值均为电压值，单位：mV
#define MCU_PIN_MAX				144	// 兼容MCU最大64pin
#define MCU_PIN_START			0	// MCU pin二级子命令从0开始
#define SUB_SUB_CMD_PIN(n)		(MCU_PIN_START + n)	// 0~64代表pin二级子命令	
#define SUB_SUB_CMD_VOL			(MCU_PIN_MAX + 1)
#define SUB_SUB_CMD_I			(MCU_PIN_MAX + 2)
#define SUB_SUB_CMD_RPM			(MCU_PIN_MAX + 3)
#define SUB_SUB_CMD_UART_VOL_NG (MCU_PIN_MAX + 4)	// 代码指令：uartvol == 0;  /* 无公差  0: 正常；1：欠压；2：过压*/
#define SUB_SUB_CMD_POWER		(MCU_PIN_MAX + 5)	// 代码指令：pow == 1000[公差%];  /* 1000 表示标准功率 ，单位：kw  ， 有公差*/
#define SUB_SUB_CMD_DIR			(MCU_PIN_MAX + 6)	// 代码指令：dir == 0(默认) 或者 dir == 1（反转）;
// #define ...
// 增补的二级子命令添加到此处，SUB_SUB_CMD_PIN_END 之前，并且 值 < SUB_SUB_CMD_PIN_END 即 (MCU_PIN_MAX + 10)
#define SUB_SUB_CMD_PIN_END		(MCU_PIN_MAX + 10)

#define SUB_SUB_CMD_AD(n)			(SUB_SUB_CMD_PIN_END + n)		// 第n路AD
#define SUB_SUB_CMD_AD_END			(SUB_SUB_CMD_AD(0) + 30)		// 最多30路AD支持
#define SUB_SUB_CMD_DA(n)			(SUB_SUB_CMD_AD_END + n)		// 第n路DA
// *注： 以上(n) n = 0,1,2,3 ...  从0开始  P0代表MCU第一个引脚，A/D和D/A也是如此。

// 通信协议：
// 0xfa		len						cmd	........................checksum	0xf0	
// 启始位	数据长度（目前<256）	命令类型	子命令或数据	校验和		结束位

//	* 其中，len是从len自身开始，到checksum 前一个字节为止（即，整个帧除了首位0xfa,0xf0 以及 checksum之外的数据个数）
//  * 其中，checksum 的值，是len个数据作累计 ^ 运算的结果  ！！！！！！！！

// index:   02          03							04
// 定制时序(0x02)	指令数目(目前仅支持长度<256)	指令1(一级子命令)	指令1数据高字节（二级子命令）	指令1数据低字节(数据高位)	指令1数据末字节（数据低位）;  指令2  ... ...
#define DATA_LENGTH_INDEX	1	// 2018/1/8  数据长度改为双字节，因此，后续关键字index 全部+1

#define MAIN_CMD_INDEX	3
#define CMD_NUM_INDEX	4
#define SUB_CMD_INDEX	5
#define SUB_CMD_BYTES_NUM	4	// 一条定制子命令 4个字节
#define this_cmd_index	(SUB_CMD_INDEX + cmd_num * SUB_CMD_BYTES_NUM)	// 当前插入子命令的数组索引基址
#define PC_CMD_BYTES_NUM	(10*1024)		// 10K
	
// 【3，常规测试可选部分】
#define NORMAL_CODE_NUM		11	//  常规测试选项 供 11项
// 以下协议中的发送次序不能更改，只能尾部增补，最多不超过 DZ_FAULT_START_MACRO 50项
//  核心协议：种类数（1byte ,index:03） + 电流（1 byte， = 0（不选中） 或者 1（选中）） + 转速 + 欠压 + 欠压恢复 + 转向 + 软启时间 + 刹车 + NTC高温截止 + NTC高温恢复 + NTC低温截止 + NCT低温恢复 + ... ...（可增补）
#endif		// CODE_PART


// 【4，To PC 部分协议】
/*	总协议:			（本文件所列topc部分信息，通用常规测试和定制测试）
**	0xfa   +   字节数(1byte)     +   上报类型(1byte)    +   数据 （n bytes）   + checksum  + 0xf0
**	（其中，上报类型有：  1，测试结果上报（0x00 良；0x01 不良）    2，系统自检上报（0x02）   3，xxxxxxxx （可增补））

**	1，测试结果上报；测试完一个控制器后一次性上发全部测试结果（不管良与不良） 
**	0xfa  +  字节总数   +  0x00/0x01   +  ID（默认0x00000000, 4 bytes）+  分类数目（1byte） + [分类类别1(1 byte) + 分类1数据(2 bytes) + 分类1结果(2 bytes，结果存第31bit,其余保留)] + [分类2结果(2 byte) +  ... ...] +   checksum   +  0xf0
注： 1)，0x00/0x01  0:良；1:不良  
	2)，分类结果:0：单项良；1：单项不良2：不测（保留）
	3)，分类类别：如“ 故障不良类型”宏定义

**	2, 系统自检上报（保留）	// * 注：只上发故障项
**	0xfa  +  字节总数   +  0x02   +   [设备码(1 byte) + 故障码(1 bytes 如，通信故障、精度故障、其他故障等)]   +   checksum   +  0xf0
**	*注：1，[]内是可选项；2，上发的一帧最大字节数 < REV_FLAME_SIZE_MAX (256 bytes)
*/

// 上报类型
#define		TEST_GOOD			0x00
#define		TEST_NG				0x01
#define		SELFCHECK_FAULT		0x02

// 故障不良类型 共 NORMAL_CODE_NUM 个常规项， 另外还有定制部分不良报错
/////////////////////////////////////////////////////////////////////////////////////
/*不良分类以及相应的语音播报*/
#define FAULT_CURRENT		0		// "电流不良"
#define FAULT_SPEED			1		// "转速不良"
#define FAULT_LOW_VOL		2		// "欠压不良"
#define FAULT_LOW_VOL_STOP	3		// "欠压恢复"
#define FAULT_DIR			4		// "转向不良"
#define FAULT_START_TIME	5		// "启动时间不良"
#define FAULT_BRAKE			6		// 刹车不良
#define FAULT_NTC_GJ		7		// "NTC高温截至"
#define FAULT_NTC_GH		8		// "NTC高温恢复"
#define FAULT_NTC_DJ		9		// "NTC低温截至"
#define FAULT_NTC_DH		10		// "NTC低温恢复"
#define FAULT_POWER			11		// 高压功率环
// 增补部分加在此处
// ...
//定制部分，除了以上不良外，还有比如：PIN脚检测错误，A/D检测错误等不良， (n) n从0开始
#define DZ_FAULT_START_MACRO	50
#define FAULT_DZ_PIN(n)			(DZ_FAULT_START_MACRO + n)					// "第n引脚信号不良"
#define FAULT_DZ_AD(n)			(DZ_FAULT_START_MACRO + MCU_PIN_MAX + n)	// "第n通道AD采样不良"

// 结构体压缩定义
#pragma pack(1)
struct ITEM_INFO {					// 每个单项测试信息，5 bytes
	unsigned char item_type;		// 单项类型，如：电流，电压等  详见上述  “故障不良类型” 定义
	unsigned short item_value;		// 单项实际测试值
	unsigned short item_reserv;		// 保留2字节 (最高bit  0: 单项良，1：单项不良)，剩余字节保留
};

// 帧结构体
struct TEST_INFO{
	unsigned char frame_head;
	unsigned char frame_data_num;
	unsigned char to_pc_type;	// TEST_GOOD/TEST_NG/SELFCHECK_FAULT	
	char saoma_id[SAOMA_ID_LEN];// 格式，如： NY161130016-00930，，，末尾多余的位填0
	char item_num;				// 单项测试总数
	ITEM_INFO item_info[1];		// 其实有n个,只是为了获取单项信息数组的基址
};
#pragma pack()

#define TO_PC_FRAME_MAX	256		// 帧数据总字节数单字节表示（需要扩展为两字节表示）
#define MAX_ITEM 49				// == ((TO_PC_FRAME_MAX - 10) / sizeof(ITEM_INFO))

// ACK数据包
//	0xfa + 1(字节数) + 0x55(CMD) + 0x57(checksum) + 0xf0          

// 测试语音播报：
// 1, “启动测试”；2，“良品” 或者 “XXX不良，要求xx，实测xxx”

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

#define beat_string	"abc#@123"	// socket心跳包字符串

// 该数据结构必须 1：1配对[配置对话框的内容]
//需要定义基本语法：
/*
**	P12 = 1; && P8 = 0;		// && 同时的意思
**  delay(20);
**  P8 = 1;
**  delay(200);
**  P1 == 0; && P2 == 0; && P3 == 1;
**  delay(1000);
**  P12 = 0; && P8 = 0;
*/

struct PARA_SETTING {
/*
** 基本参数配置
*/
	/*int*/CString LimitCurrent;		// 限流
	/*int*/CString Speed;				// 转速
	/*int*/CString LowVol;				// 欠压值
	/*int*/CString LowVolEnd;			// 欠压恢复
	/*int*/CString fgDirection;			// 转向 1：轴向逆时针  0：轴向顺时针
	/*int*/CString fgBrake;				// 是否带刹车  n>0：刹车时间   0：无
	/*int*/CString StartTime;			// 启动时间

/*
** 电池NTC		DA输出， 单位：mV
*/
	int fgNTC;							// 是否需要电池包NTC		
	/*float*/CString TopEnd;			// 高温截至
	/*float*/CString TopStart;			// 高温恢复
	/*float*/CString BotEnd;			// 低温截止
	/*float*/CString BotStart;			// 低温恢复

/*
**	基本参数的公差值
*/
	/////////////////////////////////////////////////////
	CString		deta_current;
	CString		deta_speed;
	CString		deta_low_vol;
	CString		deta_low_vol_end;
	CString		deta_start_time;
	// ntc部分
	CString		deta_ntc;
	/////////////////////////////////////////////////////

/*
**	其他参数
*/
	CString		motor_pole;			// 电机极对数
	CString		normal_vol;			// 额定电压
	CString		other;				// 保留
/*
** 启动定制
*/
	CString  dzStart;		// 启动开关编程字符串


/*
** 停机定制
*/

	CString  dzStop;		// 停机编程字符串

/*
** 其他定制
*/
	CString  dzOther;		// 其他定制编程字符串

};

// 测试item模块化名称
typedef struct {
	short	fg_half;	// 0:good不报  1：good & ng 都要报
	CString item_content;
}ITEM_CONTENT;
#define ITEM_JUGUE_CONTENT_NUM_MAX	256

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//————————————————————————————————————————————————————————————————————————————————————————————

/*
阳斌>>

毛>>		 1,NTC公差值高低温区别  3 常规测试存入数据库的字段和值问题 4，软件启动时，自动打开last配置文件(导入时保存)并下发到MCU
			5，美化界面

1， 将之前的文件模式改为数据库模式，新建表：
// create table T_flow_step_def(
// Step_no    int not null,     --流程步骤ID 
// Step_name    varchar(30)    not null, --流程步骤名称 
// Step_des    varchar(64)    not null,    --流程步骤描述
// Limit_time    int not null,     --时限
// URL     varchar(64)    not null,     --二级菜单链接 
// 备注    varchar(256)    not null, 
// )
*/

// 完善之处 >>
// 1,开机后，和服务器核对软件版本，如，PC软件，主板软件，
// 2，考虑终端测试机上位机直接打开网页访问服务器，否则服务器上还需运行一个接收程序写入到数据库
// 3，客户端软件自动升级问题,下位机软件开机后，上传客户端版本号&测试程序版本号到服务器。
// 4，终端连不上服务器时，本地保存后，一旦联网，将本地上传的机制需要加入。
//

/*__________________________________________________________________________________
// 暂时不考虑 >>
// 1》定制编程，基本指令， if(  == ) {} 指令添加.--------【暂时不考虑】
// 2》模块调用 EXE(id);(其中 id即为常规测试项中的索引) （暂时不考虑）
//——————————————————————————————————————————————————————————————————————————————————
*/

#endif // !defined(AFX_STDAFX_H__39417159_89BB_48B9_AA25_5E44DB301DB2__INCLUDED_)
#include <afxcontrolbars.h>

// 修改记录
/*
20180303：	1，修改了ID长度，格式如，	NY161130016-00930
			2，原数据库封装函数中，增加 .vt == 2 // smallint 情况； 不然表中 int/smallint数据转CString会出问题。
			3, CListCtrol增加了列点击排序功能，自定义一个LIST类，继承CListCtroll类
20180304:   1,增加了模拟始终。2,修改了应用程序图标 3，增加了列表右键单击菜单功能。
20180305：	1，socket的rev()函数反馈值的判断，增加了0 == retVal的处理，即一方中断了通信。
			2,修改了server socket 的监听逻辑以及client socket部分，并进行了相关的连接断开测试，基本OK。
20180306	1,修改了服务器端recv和send的超时设置，并加入了心跳包机制，防止客户端意外断线而导致服务端socket死等
20180309	关于网络编程需要优化的地方：
			1，服务器目前堆栈内的2048个字节会有问题   OK
			2，服务端发送阻塞问题  （当前客户端发送完全阻塞模式）XXXXX OK
			3，服务器端针对客户端发来的几条数据连在一起的情况    XXXXX OK
			（针对当前模式的客户端，数据发送频率不高，数据长度远<1024，不存在该情况。若后续针对单个客户端高频率发送时可以使用接收端缓存方式）
			4，线程池实现----重点   OK
			5,空闲占用的Socket，关闭后，还是被它所连接。   OK
			6，服务器超出后，重启listen的机制，再考虑下，最好listen就一次。实现外围排队功能。否则无优先次序。 ？？？

		   |||||||*7，socket通信以完成端口或者异步select的方式实现 ???????????????????????????????????||||||||||
		   *8，服务端软件要解决恶意客户端的数据干扰和恶意连接问题。（数据干扰可以通过特定数据协议或者口令完成，恶意连接可能需要通过防火墙设置三次握手协议的标志位完成）

20180314	1,导入了线程池；2，原接收部分的缓存由堆栈开辟改为动态malloc开辟；3，解决了空闲占用的Socket，关闭后，还是被它所连接的问题。

			?? 状态栏显示待优化 ??
20180319    1，服务器端测试1000个客户端同时并发连接发送该数据，测试无问题。2,将监听 listen()的排队个数由原来的1个改为20个）3,CPFile增加 if (value.vt == 1) // NULL  （不然测试机上报错）
20180320	1, 对thread pool init返回值进行了判断，防止线程开辟失败；  2， 对socket线程的堆栈大小配置了64K,使得可以开辟5000个线程以上；
20180321	1，服务器端测试5000个客户端同时并发连接发送该数据，并closesocket所有连接，测试基本无问题。
20180423    1,修改了volto命令，对其增加了可选电源设备的参数，如：volto(2,2000);
20180427	1，修改了扫码字符串支持最多29个字符
20180428	1，修改了串口更换串口号后收发出错问题
20180514	1，添加了 SUB_SUB_CMD_UART_VOL_NG 和 SUB_SUB_CMD_POWER两条二级指令,关于增加二级自命令需要修改的地方：
			1）参考 RPM ，2）考虑有无公差   3）考虑是否有赋值操作   4）考虑显示实际测量值 5) 有无index索引，如：P10，AD2
20180606	1) UartSendCmd()函数内部变量 i 类型由 UINT8 改为 int ; // 发送的数据个数是大于256的，目前数量总量两个字节。
20180608	1) 添加了 SUB_SUB_CMD_DIR 二级指令，测试代码可以编程实现转向的判断；测试代码中若无dir指令，则默认兼容以前的轴向逆时针，出现dir则按照当前测试代码实际转向判断
20180609	1) 添加了 AUTO_TOOL_18V_TEST 适用于电动工具测试治具（保时得项目），上位机操作三步：1，选择测试代码文件，2，配置串口，3，点击启动测试
20180613	1) 添加了 LOCAL_EXCEL_SAVE 主要针对电动工具上位机没有SQL数据库情况下，将测试数据记录以EXCEL 形式保存在本地E盘上。
20180621	1) 添加了测试结果图标提示。
20180629	1) 针对电动工具增加了每项测试项实际值的显示。2）优化了测试结果图的坐标显示问题。
20180727	1) HTTP请求   测试通过。CHttpClient http_client； 后续把测试结果信息上发后台具体动作补上。
20180729	1) 实现了具体的测试数据通过http post请求，以 JSON 格式上发到服务器的方案。并对形成的JSON字符串调试通过。
20180814    1) 实现了电动工具成测检测功能，即：直接扫码枪串口接入电脑，获取扫码ID和测试结果字符串。
20180817    1) 电动工具成测检测功能所有BUG修改完毕。
20181116    1）ReadCmd()函数内部if (RdIndex < WrIndex) -->   if (RdIndex +1 < WrIndex) {
20181205	1)DataToRevBuf()函数 增加条件 if (WrIndex + DataNum < COM_REV_BUF_SIZE) 防止程序之接收串口数据不消耗情况出现内存错误。
*/

/* 20181220 针对谢斌提出一个产品需要初测和成测都经过，并在最后装箱时检测每个产品是否都经过过两道测试工序的问题：
	方案：
	1，初测和成测每次都写数据库，通过chengche标志指明是哪个阶段的测试；
	2，数据库远程连接到阿里云，要考虑连接的间断性和重连。
	3，装箱时没有必要再次扫码，通过软件删选确认是否有两道工序未全的产品即可。
*/