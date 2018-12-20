
#include "stdafx.h"

#include "ListCtrl_Sortable.h"
#include "Resource.h"
#include "CPFile.h"

#include <shlwapi.h>
#ifdef DATA_BACE
extern CPData *p_cp_data;
#endif
int CListCtrl_Sortable::g_columnIndex = 0;
bool CListCtrl_Sortable::g_ascending = false;

BEGIN_MESSAGE_MAP(CListCtrl_Sortable, CListCtrl)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnHeaderClick)	// Column Click
	ON_NOTIFY_REFLECT_EX(NM_RCLICK, OnNMRclkList2)
END_MESSAGE_MAP()

namespace {
	bool IsThemeEnabled()
	{
//		HMODULE hinstDll;
		bool XPStyle = false;
	/*	bool (__stdcall *pIsAppThemed)();
		bool (__stdcall *pIsThemeActive)();

		// Test if operating system has themes enabled
		hinstDll = ::LoadLibrary("UxTheme.dll");
		if (hinstDll)
		{
			(FARPROC&)pIsAppThemed = ::GetProcAddress(hinstDll, "IsAppThemed");
			(FARPROC&)pIsThemeActive = ::GetProcAddress(hinstDll,"IsThemeActive");
			::FreeLibrary(hinstDll);
			if (pIsAppThemed != NULL && pIsThemeActive != NULL)
			{
				if (pIsAppThemed() && pIsThemeActive())
				{
					// Test if application has themes enabled by loading the proper DLL
					hinstDll = LoadLibrary("comctl32.dll");
					if (hinstDll)
					{
						DLLGETVERSIONPROC pDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hinstDll, "DllGetVersion");
						::FreeLibrary(hinstDll);
						if (pDllGetVersion != NULL)
						{
							DLLVERSIONINFO dvi;
							ZeroMemory(&dvi, sizeof(dvi));
							dvi.cbSize = sizeof(dvi);
							HRESULT hRes = pDllGetVersion ((DLLVERSIONINFO *) &dvi);
							if (SUCCEEDED(hRes))
                                XPStyle = dvi.dwMajorVersion >= 6;
						}
					}
				}
			}
		}*/
		return XPStyle;
	}

	LRESULT EnableWindowTheme(HWND hwnd, LPCWSTR app, LPCWSTR idlist)
	{/*
		HMODULE hinstDll;
		HRESULT (__stdcall *pSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
		HANDLE (__stdcall *pOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
		HRESULT (__stdcall *pCloseThemeData)(HANDLE hTheme);

		hinstDll = ::LoadLibrary("UxTheme.dll");
		if (hinstDll)
		{
			(FARPROC&)pOpenThemeData = ::GetProcAddress(hinstDll, TEXT("OpenThemeData"));
			(FARPROC&)pCloseThemeData = ::GetProcAddress(hinstDll, TEXT("CloseThemeData"));
			(FARPROC&)pSetWindowTheme = ::GetProcAddress(hinstDll, TEXT("SetWindowTheme"));
			::FreeLibrary(hinstDll);
			if (pSetWindowTheme && pOpenThemeData && pCloseThemeData)
			{
				HANDLE theme = pOpenThemeData(hwnd,L"ListView");
				if (theme!=NULL)
				{
					VERIFY(pCloseThemeData(theme)==S_OK);
					return pSetWindowTheme(hwnd, app, idlist);
				}
			}
		}*/
		return S_FALSE;
	}
}

BOOL CListCtrl_Sortable::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pLV = reinterpret_cast<NMLISTVIEW*>(pNMHDR);

	SetFocus();	// Ensure other controls gets kill-focus

	int colIndex = pLV->iSubItem;

	if (m_SortCol==colIndex)
	{
		m_Ascending = !m_Ascending;
	}
	else
	{
		m_SortCol = colIndex;
		m_Ascending = true;
	}

	if (SortColumn(m_SortCol, m_Ascending))
		SetSortArrow(m_SortCol, m_Ascending);
	*pResult = 0;
	return FALSE;	// Let parent-dialog get chance
}

void CListCtrl_Sortable::SetSortArrow(int colIndex, bool ascending)
{/*
	if (IsThemeEnabled())
	{
#if (_WIN32_WINNT >= 0x501)
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
		{
			HDITEM hditem = {0};
			hditem.mask = HDI_FORMAT;
			VERIFY( GetHeaderCtrl()->GetItem( i, &hditem ) );
			hditem.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
			if (i == colIndex)
			{
				hditem.fmt |= ascending ? HDF_SORTDOWN : HDF_SORTUP;
			}
			VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
		}
#endif
	}
	else
	{
		UINT bitmapID = m_Ascending ? IDB_DOWNARROW : IDB_UPARROW;			//IDB_DOWNARROW，表示向下箭头IDB位图；IDB_UPARROW，表示向上箭头IDB位图
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
		{
			HDITEM hditem = {0};
			hditem.mask = HDI_BITMAP | HDI_FORMAT;
			VERIFY( GetHeaderCtrl()->GetItem( i, &hditem ) );
			if (hditem.fmt & HDF_BITMAP && hditem.fmt & HDF_BITMAP_ON_RIGHT)
			{
				if (hditem.hbm)
				{
					DeleteObject(hditem.hbm);
					hditem.hbm = NULL;
				}
				hditem.fmt &= ~(HDF_BITMAP|HDF_BITMAP_ON_RIGHT);
				VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
			}
			if (i == colIndex)
			{
				hditem.fmt |= HDF_BITMAP|HDF_BITMAP_ON_RIGHT;
				hditem.hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0,0, LR_LOADMAP3DCOLORS); 
				VERIFY( hditem.hbm!=NULL );
				VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
			}
		}
	}*/
}

void CListCtrl_Sortable::PreSubclassWindow()
{/*
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);

	EnableWindowTheme(GetSafeHwnd(), L"Explorer", NULL);*/
}

void CListCtrl_Sortable::ResetSortOrder()
{
	m_Ascending = true;
	m_SortCol = -1;
	SetSortArrow(m_SortCol, m_Ascending);
}

// The column version of GetItemData(), one can specify an unique
// identifier when using InsertColumn()
int CListCtrl_Sortable::GetColumnData(int col) const
{
	LVCOLUMN lvc = {0};
	lvc.mask = LVCF_SUBITEM;
	VERIFY( GetColumn(col, &lvc) );
	return lvc.iSubItem;
}

// 比较函数
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	int row1 = (int) lParam1;
	int row2 = (int) lParam2;
	CListCtrl* lc = (CListCtrl*)lParamSort;
	CString lp1 = lc->GetItemText(row1,CListCtrl_Sortable::g_columnIndex);
	CString lp2 = lc->GetItemText(row2,CListCtrl_Sortable::g_columnIndex);
	// 文字型比较
	if (CListCtrl_Sortable::g_ascending)
		return lp1.CompareNoCase(lp2);
	else
		return lp2.CompareNoCase(lp1);

	return 0;
}

bool CListCtrl_Sortable::SortColumn(int columnIndex, bool ascending)
{
	//排序
	g_columnIndex = columnIndex;
	g_ascending = ascending;
	int count = GetItemCount();
	for (int i=0;i<count;i++)
	{
		  SetItemData(i,i); // 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他 比较函数的第一二个参数
	}
	SortItems(MyCompareProc, (DWORD_PTR)this);
	return true;
}

// 列表项右键点击跳出菜单
BOOL CListCtrl_Sortable::OnNMRclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString str_date = "";
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    // TODO: 在此添加控件通知处理程序代码
    int index = pNMItemActivate->iItem;
	if( index == -1 ) return FALSE;
	HMENU hMenu = ::CreatePopupMenu();
	AppendMenu( hMenu, MF_STRING, 10001, "删除" );
	AppendMenu(hMenu,MF_STRING,10002,"编辑");
	CPoint pt;
	GetCursorPos(&pt); //获得当前鼠标位置
	UINT Cmd = (UINT)::TrackPopupMenu( hMenu, TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL );//弹出菜单
	switch( Cmd )//响应点击的菜单
	{
	case 10001:			// 删除
		if (IDOK == AfxMessageBox("确定要删除所选记录项吗？",MB_OKCANCEL)) {
			// 删除 index 行，更新数据库
			str_date = GetItemText(index, 1);	// 1:日期所在列
			DeleteItem(index);
#ifdef DATA_BACE
			p_cp_data->Execute("delete from TianYouShou where 日期 like '%" + str_date + "%'");
#endif
		}
		break;
	case  10002:		// 编辑
		// SetItem(m_listItemCount-2,3,LVIF_TEXT,"不应答",0,0,0,NULL);
		break;
	default:
		break;
	}
	*pResult = 0;
	return FALSE;	// Let parent-dialog get chance
}