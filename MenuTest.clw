; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTab2
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MenuTest.h"
LastPage=0

ClassCount=8
Class1=CMenuTestApp
Class2=CMenuTestDoc
Class3=CMenuTestView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDD_DIALOG1
Class5=CAboutDlg
Resource3=IDD_DIALOG_NEW
Resource4=IDD_MENUTEST_FORM
Resource5=IDR_MAINFRAME
Class6=C_dlgNew
Class7=CTab1
Class8=CTab2
Resource6=IDD_DIALOG2

[CLS:CMenuTestApp]
Type=0
HeaderFile=MenuTest.h
ImplementationFile=MenuTest.cpp
Filter=N

[CLS:CMenuTestDoc]
Type=0
HeaderFile=MenuTestDoc.h
ImplementationFile=MenuTestDoc.cpp
Filter=N

[CLS:CMenuTestView]
Type=0
HeaderFile=MenuTestView.h
ImplementationFile=MenuTestView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=IDC_TYPE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_NEW_PROJ
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=MenuTest.cpp
ImplementationFile=MenuTest.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_NEW_PROJ
Command2=ID_NEWMENU
Command3=ID_APP_ABOUT
CommandCount=3

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_MENUTEST_FORM]
Type=1
Class=CMenuTestView
ControlCount=5
Control1=IDC_TYPE,combobox,1344339971
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350633600
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BUTTON2,button,1342242816

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_DIALOG_NEW]
Type=1
Class=C_dlgNew
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TAB1,SysTabControl32,1350566912

[DLG:IDD_DIALOG1]
Type=1
Class=CTab1
ControlCount=25
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_EDIT4,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT5,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT6,edit,1350631552
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT7,edit,1350631552
Control16=IDC_STATIC1,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT8,edit,1350631552
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT9,edit,1350631552
Control22=IDC_EDIT10,edit,1350631552
Control23=IDC_STATIC,static,1342308352
Control24=IDC_EDIT11,edit,1350631552
Control25=IDC_CHECK1,button,1342242819

[DLG:IDD_DIALOG2]
Type=1
Class=CTab2
ControlCount=3
Control1=IDC_STATIC,button,1342177287
Control2=IDC_EDIT1,edit,1353781444
Control3=IDC_STATIC1,button,1342177287

[CLS:C_dlgNew]
Type=0
HeaderFile=_dlgNew.h
ImplementationFile=_dlgNew.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_TAB1
VirtualFilter=dWC

[CLS:CTab1]
Type=0
HeaderFile=Tab1.h
ImplementationFile=Tab1.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CHECK1
VirtualFilter=dWC

[CLS:CTab2]
Type=0
HeaderFile=Tab2.h
ImplementationFile=Tab2.cpp
BaseClass=CDialog
Filter=D
LastObject=CTab2
VirtualFilter=dWC

