#ifndef AMEMOTEXTMATRIX_H
#define AMEMOTEXTMATRIX_H

/*
 *  AMemoTextMatrix 文本数据矩阵
 *  文本数据包括：文本、度量值、着色下标、行高、行宽、行间距
 *  主要功能：文本数据的存储及同步
 */
class AUI_API AMemoTextMatrix
{
public:
	class ALineInfo : public AObject
	{
		CLASS_INFO(LineInfo,Object);
	public:
		ALineInfo():base_class()
		{
			m_iWidth = 0;
			m_iHeight = 0;
			m_iIndex = -1;
			m_iTop = 0;
		}
		virtual ~ALineInfo(){}

		AString		m_sText;//行文本
		AArray		m_aColoration;//每个字符的着色索引
		AArray		m_aPos;//每个字符的位置
		int		m_iHeight;//行高
		int		m_iWidth;//行宽
		int		m_iIndex;//在行宽数组中的下标
		int		m_iTop;//顶端位置
	};
	AMemoTextMatrix()
	{
		m_iRowSpace = 6;
		m_szEditor = ASize(0,0);
	}
	~AMemoTextMatrix()
	{

	}
	//=== 辅助函数
	void SplitString(const AString& s,AStringArray& sa)
	{
		ACHAR* sz = (ACHAR *)s.Text();
		ACHAR* start;
		start = sz;
		while(true)
		{
			if( *sz == 0 ) break;
			if( *sz == '\n' )
			{
				AString s(start,sz-start);
				sa.Add( s );
				sz ++;
				start = sz;
			}
			else
				sz ++;
		}
		if( sz > start ) sa.Add( AString(start,sz-start) );
	}
	//====== 整体操作 =====
	void Clear()
	{
		m_aLine.Clear();
		m_aWidth.Clear();
		m_szEditor = ASize(0,0);
	}

	void SetText(const AString& s)
	{
		Clear();
		AStringArray sa;
		SplitString(s,sa);
		AStringArrayEnumerator em(&sa);
		while( em.Next() ) AddLine(em.Cur());
	}
	AString GetText(const AString& s=_T("\n"))
	{
		int iTotalSize=0;
		int iSplitLen = s.Length();
		int iCount = m_aLine.GetCount();
		int i;
		for(i=0;i<iCount;i++)
		{
			ALineInfo* pLine = (ALineInfo*)m_aLine[i];
			iTotalSize += pLine->m_sText.Length();
			iTotalSize += iSplitLen;
		}
		AMemory ms;
		ms.SetSize( (iTotalSize + 1 ) * sizeof(ACHAR) );
		ACHAR* sz = (ACHAR *)ms.GetData();
		for(i=0;i<iCount;i++)
		{
			ALineInfo* pLine = (ALineInfo*)m_aLine[i];
			if( i > 0 && iSplitLen ) 
			{
				memcpy(sz,s.Text(),s.Length()*sizeof(ACHAR));
				sz += iSplitLen;
			}
			if( pLine->m_sText.Length() ) memcpy( sz,pLine->m_sText.Text(),pLine->m_sText.Length() * sizeof(ACHAR));
			sz += pLine->m_sText.Length();
			
		}
		*sz = 0;
		return AString((ACHAR *)ms.GetData());
	}
	//============ 行操作 =====================
	void AddLine(const AString& s)
	{
		ALineInfo* pLine = new ALineInfo();
		m_aLine.Add( pLine );
		
		pLine->m_sText = s;
		int iLen = s.Length();
		for(int i=0;i<iLen;i++)
		{
			pLine->m_aColoration.Add( (void *)-1 );
			pLine->m_aPos.Add( (void *)0 );
		}
		m_szEditor.cy += 0 + m_iRowSpace;//行高暂时为0，后面应该会调用SetLineHeight
	}
	void RemoveLine(int iLine)
	{
		ALineInfo* p = (ALineInfo *)m_aLine[iLine];
		m_szEditor.cy -= (p->m_iHeight + m_iRowSpace);
		p->m_sText = _T("");
		p->m_aColoration.Clear();
		p->m_aPos.Clear();
		_updateLineWidth(iLine);//主要为了更新编辑框宽度
		if( p->m_iIndex != -1 ) m_aWidth.Delete(p->m_iIndex);
		m_aLine.Delete(iLine);
		if( iLine > 0 ) _updateLineTop(iLine-1);
	}
	int GetLineCount()
	{
		return m_aLine.GetCount();
	}
	AString GetLine(int iLine)
	{
		return ((ALineInfo*)m_aLine[iLine])->m_sText;
	}
	ALineInfo* InsertLine(int iLine,const AString& sText)
	{
		ALineInfo* pLine = new ALineInfo();
		pLine->m_sText = sText;
		pLine->m_iHeight = 0;
		int iLen = sText.Length();
		for(int i=0;i<iLen;i++)
		{
			pLine->m_aColoration.Add( (void *)-1 );
			pLine->m_aPos.Add( (void *)0 );
		}
		m_aLine.Insert(iLine,pLine);
		m_szEditor.cy += 0 + m_iRowSpace;//行高暂时为0，后面应该会调用SetLineHeight
		return pLine;
	}
	int GetLineHeight(int iLine)
	{
		if( iLine >= m_aLine.GetCount() ) return 0;//返回缺省高度
		ALineInfo* pLine = (ALineInfo *)m_aLine[iLine];
		return pLine->m_iHeight;
	}
	int GetLineTop(int iLine)
	{
		if( iLine >= m_aLine.GetCount() ) return 0;//返回缺省高度
		ALineInfo* pLine = (ALineInfo *)m_aLine[iLine];
		return pLine->m_iTop;
	}
	void SetLineHeight(int iLine,int iHeight)
	{
		_setLineHeight(iLine,iHeight);
	}
	void UpdateLineWidth(int iLine)
	{
		_updateLineWidth(iLine);
	}
	int GetLineWidth(int iLine)
	{
		if( iLine >= m_aLine.GetCount() ) return 0;//返回缺省宽度
		ALineInfo* pLine = (ALineInfo *)m_aLine[iLine];
		return pLine->m_iWidth;
	}
	int GetLineCharCount(int iLine)
	{
		if( iLine >= m_aLine.GetCount() ) return 0;
		ALineInfo* pLine = (ALineInfo *)m_aLine[iLine];
		return pLine->m_aColoration.GetCount();
	}
	//======== 单个字符操作 ===============
	void InsertChar(ACaretSeat seat,ACHAR ch)
	{
		int r,c;
		r = seat.iRow;
		c = seat.iCol;
		if( r >= m_aLine.GetCount() )
		{
			if( c == 0 )
			{
				int iCount = m_aLine.GetCount();
				for(int i=iCount-1;i<r;i++)
					AddLine(_T(""));//补充空行
				InsertChar(seat,ch);
			}
			else
				throw AException(_T("[MemoEdit]Out of bound"));
		}
		else
		{
			ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
			pLine->m_sText = pLine->m_sText.Left(seat.iCol) + AString(ch) + pLine->m_sText.Right(pLine->m_sText.Length()-seat.iCol);
			pLine->m_aColoration.Insert(seat.iCol,(void *)-1);
			pLine->m_aPos.Insert(seat.iCol,0);
		}
		
	}
	void RemoveChar(ACaretSeat seat)
	{
		int r = seat.iRow;
		int c = seat.iCol;
		if( m_aLine.GetCount() < 1 ) return;
		ALineInfo* pLine = (ALineInfo *)m_aLine[r];
		AString s1,s2;
		if( c > 0 ) s1 = pLine->m_sText.Left(c);
		s2 = pLine->m_sText.Right(pLine->m_sText.Length() - c-1);
		pLine->m_sText = s1 + s2;
		pLine->m_aColoration.Delete(seat.iCol);
		pLine->m_aPos.Delete(seat.iCol);
	}
	ACHAR GetChar(ACaretSeat seat)
	{
		ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
		if( seat.iCol >= pLine->m_sText.Length() ) return 0;
		return pLine->m_sText[seat.iCol];
	}
	AString GetChars(ACaretSeat seat,int iCount)
	{
		ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
		return pLine->m_sText.Mid(seat.iCol,iCount);
	}
	//================== 行内块操作 ==============
	void InsertChars(ACaretSeat seat,const AString& s)
	{
		int iLen = s.Length();
		if( iLen < 1 ) return;
		AString sInsert=s;
		int iPos =0;
		iPos = sInsert.Find(_T("\n"));
		if( iPos > -1 ) sInsert = sInsert.Left(iPos);

		ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
		AString s1,s2;
		if( seat.iCol > 0 ) s1 = pLine->m_sText.Left(seat.iCol);
		s2 = pLine->m_sText.Right(pLine->m_sText.Length() - seat.iCol);
		pLine->m_sText = s1 + sInsert + s2;
		for(int i=0;i<iLen;i++)
		{
			pLine->m_aColoration.Insert(seat.iCol,(void *)-1 );
			pLine->m_aPos.Insert(seat.iCol,(void *)0 );
		}
	}
	void RemoveChars(ACaretSeat seat,int iCount)
	{
		if(iCount < 1 ) return;
		ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
		AString s1,s2;
		if( seat.iCol > 0 ) s1 = pLine->m_sText.Left(seat.iCol);
		s2 = pLine->m_sText.Right(pLine->m_sText.Length() - seat.iCol-iCount);
		pLine->m_sText = s1 + s2;
		for(int i=0;i<iCount;i++)
		{
			pLine->m_aColoration.Delete(seat.iCol);
			pLine->m_aPos.Delete(seat.iCol);
		}
	}
	//===================== 着色操作 ===============================
	void Tint(ACaretSeat seat,int iIndex)
	{
		ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
		pLine->m_aColoration.SetItem(seat.iCol,(void *)iIndex);
	}
	int GetColoration(ACaretSeat seat)
	{
		ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
		return (int)pLine->m_aColoration[seat.iCol];
	}
	//=================== 度量操作 ===========================
	void SetMeasure(ACaretSeat seat,int x)
	{
		ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
		pLine->m_aPos.SetItem(seat.iCol,(void *)x);
	}
	int GetMeasure(ACaretSeat seat)
	{
		ALineInfo* pLine = (ALineInfo *)m_aLine[seat.iRow];
		return (int)pLine->m_aPos[seat.iCol];
	}
	ASize GetEditorSize()
	{
		return m_szEditor;
	}
	int GetRowSpace()
	{
		return m_iRowSpace;
	}
	void SetRowSpace(int v)
	{
		m_iRowSpace = v;
	}
	ACaretSeat SeatByPos(APoint pt)
	{
		int iRow=0;
		int iLineCount = m_aLine.GetCount();
		if( iLineCount < 1 ) return ACaretSeat(0,0);
		int cy=0;
		for(iRow=0;iRow<iLineCount;iRow++)
		{
			ALineInfo* pLine = (ALineInfo*)m_aLine[iRow];
			if( pt.y >= cy && pt.y <= cy + pLine->m_iHeight+m_iRowSpace )
				break;
			cy += pLine->m_iHeight + m_iRowSpace;
		}
		if( iRow == iLineCount && iRow > 0 ) iRow --;
		int cx = 0;
		ALineInfo* pLine = (ALineInfo *)m_aLine[iRow];
		int iCol=0;
		int iColCount = pLine->m_aPos.GetCount();
		for(iCol=0;iCol<iColCount;iCol++)
		{
			int cx2 = (int)pLine->m_aPos[iCol];
			if( pt.x >= cx && pt.x <= cx2 ) 
			{
				if( pt.x > cx + (cx2-cx)/2 ) iCol++;
				break;
			}
			cx = cx2;
		}
		return ACaretSeat(iRow,iCol);
	}
private:
	AObjectArray		m_aLine;
	AObjectArray		m_aWidth;//根据宽度倒序排列的LineInfo集合
	ASize			m_szEditor;//编辑框总体大小
	int			m_iRowSpace;

	//--- 内部辅助函数 -----------------
	void _setLineHeight(int iLine,int iHeight)
	{
		ALineInfo* pLine = (ALineInfo *)m_aLine[iLine];
		int h = pLine->m_iHeight;
		if( h != iHeight )
		{
			int delta = iHeight - h;
			pLine->m_iHeight = iHeight;
			m_szEditor.cy += delta;
		}
		_updateLineTop(iLine);
	}
	void _updateLineWidth(int iLine)
	{
		int w=0;
		ALineInfo* pLine = (ALineInfo *)m_aLine[iLine];
		if( pLine->m_sText.Length() > 0 )
		{
			w = (int)pLine->m_aPos[ pLine->m_sText.Length() - 1 ];
		}
		int iOldWidth = pLine->m_iWidth;
		if( iOldWidth != w )
		{
			pLine->m_iWidth = w;//设置新的宽度

			//先删除
			if( pLine->m_iIndex != -1 ) m_aWidth.Delete( pLine->m_iIndex );

			//按倒序排序进行插入
			int iCount = m_aWidth.GetCount();
			if( iCount == 0 )
			{
				m_aWidth.Add( pLine );
				pLine->m_iIndex = 0;
			}
			else
			{
				//对半查找法
				int left = 0;//左边下标
				int right = iCount-1;//右边下标
				int middle = (left+right)/2;//中间下标
				int index = middle;//最终要插入的位置
				while( true )
				{
					ALineInfo* pLeft = (ALineInfo *)m_aWidth[left];
					ALineInfo* pRight = (ALineInfo *)m_aWidth[right];
					ALineInfo* pMiddle = (ALineInfo *)m_aWidth[middle];
					if( pMiddle->m_iWidth == w )
					{
						index = middle;
						break;
					}
					else
					{
						//如果左和右相差不超过2，即区间太小，则不必再查找
						if( right - left <= 2 )
						{
							if( pLeft->m_iWidth < w ) index = left;
							else if( pRight->m_iWidth > w ) index = right+1;
							else index = right;
							break;
						}
						else
						{
							//是否在区间内
							if( pLeft->m_iWidth >= w && pRight->m_iWidth <= w )
							{
								//在，则缩小范围
								if( pMiddle->m_iWidth > w ) left = middle;
								else right = middle;
								middle = (right + left)/2;
							}
							else
							{
								//不在
								if( w > pLeft->m_iWidth ) index = left;
								else index = right + 1;
								break;
							}
						}
					}
				}
				m_aWidth.Insert(index,pLine);
				pLine->m_iIndex = index;
			}
		}
		//根据宽度调整整体编辑框大小
		if( w > m_szEditor.cx ) 
			m_szEditor.cx = w;//撑大
		else
		{
			//判断是否有必要缩小编辑框
			int MaxWidth = 0;
			if( m_aWidth.GetCount() > 0 ) MaxWidth = ((ALineInfo *)m_aWidth[0])->m_iWidth;
			if( m_szEditor.cx > MaxWidth ) m_szEditor.cx = MaxWidth;
		}
	}
	void _updateLineTop(int iFromLine)
	{
		int iCount = m_aLine.GetCount();
		if( iFromLine >= iCount ) return;
		
		int y=0;
		if( iFromLine > 0 ) 
		{
			ALineInfo* pFromLine = (ALineInfo *)m_aLine[iFromLine-1];
			y = pFromLine->m_iTop+ pFromLine->m_iHeight+m_iRowSpace;
		}
		for(int i=iFromLine;i<iCount;i++)
		{
			ALineInfo* p = (ALineInfo *)m_aLine[i];
			p->m_iTop = y;
			y += p->m_iHeight+m_iRowSpace;
		}
	}
};

#endif//AMEMOTEXTMATRIX_H