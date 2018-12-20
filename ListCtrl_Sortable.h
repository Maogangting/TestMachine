#pragma once

class CListCtrl_Sortable : public CListCtrl
{
public:
    bool m_Ascending;
	int  m_SortCol;

	DECLARE_MESSAGE_MAP();

	afx_msg BOOL OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMRclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	void PreSubclassWindow();

public:
	CListCtrl_Sortable()
		:m_Ascending(false)
		,m_SortCol(-1)
	{}
	int GetColumnData(int col) const;
	void SetSortArrow(int col, bool ascending);
	bool IsAscending() const { return m_Ascending; }
	void ResetSortOrder();
	virtual bool SortColumn(int columnIndex, bool ascending);
	static int g_columnIndex;
	static bool g_ascending;
};