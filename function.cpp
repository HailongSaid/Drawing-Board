#include "function.h"
#include "resource.h"

/*
   Function：初始化链表
   Name：InitList
   Parameter：NODE ** aHead
   Return：BOOL
*/
BOOL InitList(NODE ** aHead)
{
    *aHead = new NODE;
    if(NULL == *aHead)
        return FALSE;
    
    (*aHead)->m_pNext = NULL;
    
    return TRUE;
}

/*
   Function：返回最后一个元素的节点
   Name：ReturnLastElementPoint
   Parameter：NODE * aHead
   Return：NODE *
*/
NODE * ReturnLastElementPoint(NODE * aHead)
{
    while(aHead->m_pNext != NULL)
    {
        aHead = aHead->m_pNext;
    }
    
    return aHead;
} 

/*
   Function：将当前节点放到链表的最后面
   Name：PushBack
   Parameter：NODE * aHead, NODE * pNode
   Return：void
*/
void PushBack(NODE * aHead, NODE * pNode)
{
    NODE * pRear = ReturnLastElementPoint(aHead);
    NODE * p = new NODE;
    *p = *pNode;
    pRear->m_pNext = p;
    
    isEdit = TRUE;
}


/*
   Function：返回元素的个数
   Name： ReturnElementCount
   Parameter：NODE * aHead
   Return：int
*/
INT ReturnElementCount(NODE * aHead)
{
    INT ElementCount = 0;
    while(aHead)
    {
        ElementCount++;
        aHead = aHead->m_pNext;
    }
    
    return ElementCount;
}

/*
   Function：销毁链表
   Name： DestoryList
   Parameter：NODE * aHead
   Return：void
*/
void DestoryList(NODE * aHead)
{
    NODE * temp;
    temp = aHead->m_pNext;
    
    while(temp != NULL)
    {
        delete aHead;
        aHead = temp;
        temp = temp->m_pNext;      
    }
    delete aHead;
}

/*
   Function：清空链表
   Name： ClearList
   Parameter：NODE * aHead
   Return：void
*/
void ClearList(NODE * aHead)
{
    DestoryList(aHead);
    InitList(&aHead);
}


/*
   Function：通过点击菜单，来对节点的各种属性进行设置
   Name： EntryShapeElem
   Parameter：HWND hWnd, WPARAM wParam, NODE * Currnode, HR * arr, INT *index
   Return：void
*/
void EntryShapeElem(HWND hWnd, WPARAM wParam, NODE * Currnode, HR * arr, INT *index)
{
	TCHAR message[]=TEXT("该版权归属刘海龙：\n如若想知详细信息，请咨询XXX！");
    TCHAR helptitle[]=TEXT("帮助文档");
    NODE *temp = NULL;
    int i = 0;
    switch(LOWORD(wParam))
    {
    case ID_GRAPH:
		(*Currnode).m_ImageType=GRAPH;
		break;
	case ID_LINE:
		(*Currnode).m_ImageType=LINE;
		break;
	case ID_RECT:
		(*Currnode).m_ImageType=RECTANGLE;
		break;
	case ID_ELLIPSE:
		(*Currnode).m_ImageType=ELLIPSE;
		break;
    case ID_CUBE:
		(*Currnode).m_ImageType=CUBE;
		break;
	case ID_TRIANGLE://三角形
		(*Currnode).m_ImageType=TRIANGLE;
		break;
		
	case ID_SEXANGLE://六边形
		(*Currnode).m_ImageType=SEXANGLE;
		break;
	case ID_NULL:
		(*Currnode).m_ImageType=ZERO;
		break;
	case ID_ONE:
		(*Currnode).m_PenWidth=1;
		break;
	case ID_FIVE:
		(*Currnode).m_PenWidth=5;
		break;
	case ID_TEN:
		(*Currnode).m_PenWidth=10;
		break;
	case ID_PEN_BLUE:
		(*Currnode).m_PenColor=RGB(0,0,255);
		break;
	case ID_PEN_RED:
		(*Currnode).m_PenColor=RGB(255,0,0);
		break;
	case ID_PEN_GREEN:
		(*Currnode).m_PenColor=RGB(0,255,0);
		break;
	case ID_BRUSH_RED:
		(*Currnode).m_BrushColor=RGB(255,0,0);
		break;
	case ID_BRUSH_BLUE:
		(*Currnode).m_BrushColor=RGB(0,0,255);
		break;
	case ID_BRUSH_GREEN:
		(*Currnode).m_BrushColor=RGB(0,255,0);
		break;
	case ID_ERASER:
		(*Currnode).m_ImageType=ERASER;
         (*Currnode).m_BrushColor=RGB(255,255, 255);
        (*Currnode).m_PenColor=RGB(255,255, 255);
		(*Currnode).m_PenWidth=5;
		break;
    case ID_NEW:
        IsSave(hWnd);
        //清空热点数组
        for(i=0; i<*index; i++)
        {
            DeleteObject(arr[i].HotRegionHandle);
        }
        *index = 0;
        New(head, hWnd);
        break;
    case ID_OPEN:
        IsSave(hWnd);
        New(head, hWnd);
        Open(hWnd);
        //清空热点数组
        for(i=0; i<*index; i++)
        {
            DeleteObject(arr[i].HotRegionHandle);
        }
        *index = 0;
        //热点数组重新赋值
        temp = head->m_pNext;
        while(temp)
        {
            arr[*index].SourcePoint = temp->m_SourcePoint;
            arr[*index].DestPoint = temp->m_DestPoint;
            arr[*index].HotRegionHandle =  
            CreateRectRgn(temp->m_SourcePoint.x, temp->m_SourcePoint.y, temp->m_DestPoint.x, temp->m_DestPoint.y);
            (*index)++;
            temp = temp->m_pNext;
        }
        break;
    case ID_SAVE_AS:
        SaveAs(hWnd);
        break;
    case ID_SAVE:
        Save(hWnd);
        break;
    case ID_CLOSE:
        IsSave(hWnd);
        DestoryList(head);
        PostQuitMessage(0);
        break;
	case ID_HELP_D:
		
        MessageBox(
			hWnd,          // handle of owner window
			message,     // address of text in message box
			helptitle,  // address of title of message box
			MB_OK          // style of message box
			);
		break;
    default:
        break;
    }
}


/*
   Function：画出指定的图形类型
   Name： Draw
   Parameter：HDC hdc,const SHAPETYPE aShaptype, const POINT *psource,POINT *pmid, POINT *pcurr, HWND aHwnd
   Return：BOOL
*/

BOOL Draw(HDC hdc,const SHAPETYPE aShaptype, const POINT *psource,POINT *pmid, POINT *pcurr, HWND aHwnd)
//画出指定的图形类型
{

	switch(aShaptype)
	{
	case GRAPH://曲线
		MoveToEx(hdc,pmid->x,pmid->y,NULL);//确定画线的起点
		LineTo(hdc,pcurr->x,pcurr->y);
		pmid->x=pcurr->x;
		pmid->y=pcurr->y;
		return TRUE;
		break;
	case LINE://直线
		MoveToEx(hdc,psource->x,psource->y,NULL);//确定画线的起点
		LineTo(hdc,pmid->x,pmid->y);
		MoveToEx(hdc,psource->x,psource->y,NULL);//确定画线的起点
		LineTo(hdc,pcurr->x,pcurr->y);
		pmid->x=pcurr->x;
		pmid->y=pcurr->y;
		return TRUE;
		break;
	case RECTANGLE://矩形
		Rectangle(hdc,psource->x,psource->y,pmid->x,pmid->y);
		Rectangle(hdc,psource->x,psource->y,pcurr->x,pcurr->y);
		pmid->x=pcurr->x;
		pmid->y=pcurr->y;
		return TRUE;
		break;
	case ELLIPSE://椭圆
		Ellipse(hdc,psource->x,psource->y,pmid->x,pmid->y);
		Ellipse(hdc,psource->x,psource->y,pcurr->x,pcurr->y);
		pmid->x=pcurr->x;
		pmid->y=pcurr->y;
		return TRUE;
		break;
	case CUBE://立方体
		Cube(hdc,psource,pmid);
		Cube(hdc,psource,pcurr);
		pmid->x=pcurr->x;
		pmid->y=pcurr->y;
		return TRUE;
		break;
	case TRIANGLE:// 三角形
		Triangle(hdc,psource,pmid);
		Triangle(hdc,psource,pcurr);
		pmid->x=pcurr->x;
		pmid->y=pcurr->y;
		return TRUE;
		break;
	case SEXANGLE://六角形
		Sexangle(hdc,psource,pmid);
		Sexangle(hdc,psource,pcurr);
		pmid->x=pcurr->x;
		pmid->y=pcurr->y;
		return TRUE;
		break;
	case ERASER://橡皮擦
		Rectangle(hdc,psource->x,psource->y,pmid->x,pmid->y);
		Rectangle(hdc,psource->x,psource->y,pcurr->x,pcurr->y);
		pcurr->x=pmid->x;
		pcurr->y=pmid->y;
		pmid->x=pcurr->x;
		pmid->y=pcurr->y;
		return TRUE;
		break;
	default:
		return FALSE;
		break;
		
		
	}
	
}


/*
   Function：画出虚线
   Name： Xuxian
   Parameter：HDC hdc,int x1,int y1,int x2,int y2
   Return：void
*/
void XuXian(HDC hdc,int x1,int y1,int x2,int y2)
{
	int n=10;
	for (int i=0;i<n;i+=2)
	{
		MoveToEx(hdc,(int)(x1+(x2-x1)/n*i),(int)(y1+(y2-y1)/n*i),NULL);
		LineTo(hdc,(int)(x1+(x2-x1)/n*(i+1)),(int)(y1+(y2-y1)/n*(i+1)));
	}
}


/*
   Function：画立方体
   Name： Cube
   Parameter：HDC hdc,const POINT *psource,const POINT *pcurr
   Return：void
*/
void Cube(HDC hdc,const POINT *psource,const POINT *pcurr)//画立方体
{
	int a=2*(int)(pcurr->x-psource->x),c=a/2;
	MoveToEx(hdc,psource->x,psource->y,NULL);//2
	LineTo(hdc,pcurr->x+c,psource->y);
	MoveToEx(hdc,psource->x,psource->y,NULL);//1
	LineTo(hdc,psource->x-c,psource->y+c);
	//MoveToEx(hdc,psource->x,psource->y,NULL);//6//
	//LineTo(hdc,psource->x,pcurr->y-c);
	XuXian(hdc,psource->x,pcurr->y-c,psource->x,psource->y);
	MoveToEx(hdc,pcurr->x+c,psource->y,NULL);//8
	LineTo(hdc,pcurr->x+c,pcurr->y-c);
	
	MoveToEx(hdc,psource->x-c,psource->y+c,NULL);//5
	LineTo(hdc,psource->x-c,pcurr->y);
	
/*	MoveToEx(hdc,pcurr->x,pcurr->y-a,NULL);//4
	LineTo(hdc,psource->x-c,psource->y+c);
	MoveToEx(hdc,pcurr->x,pcurr->y-a,NULL);//3
	LineTo(hdc,pcurr->x+c,psource->y);
    MoveToEx(hdc,pcurr->x,pcurr->y-a,NULL);//7
	LineTo(hdc,pcurr->x,pcurr->y);*/	   
	MoveToEx(hdc,psource->x+c, psource->y+c,NULL);//4
	LineTo(hdc,psource->x-c,psource->y+c);
	MoveToEx(hdc,psource->x+c, psource->y+c,NULL);//3
	LineTo(hdc,pcurr->x+c,psource->y);
    MoveToEx(hdc,psource->x+c, psource->y+c,NULL);//7
	LineTo(hdc,pcurr->x,pcurr->y);	
	//MoveToEx(hdc,psource->x-c,pcurr->y,NULL);//9//
	//LineTo(hdc,psource->x,pcurr->y-c);
	XuXian(hdc,psource->x,pcurr->y-c,psource->x-c,pcurr->y);
	MoveToEx(hdc,psource->x-c,pcurr->y,NULL);//10
	LineTo(hdc,pcurr->x,pcurr->y);
	//MoveToEx(hdc,pcurr->x+c,pcurr->y-c,NULL);//12//
	//LineTo(hdc,psource->x,pcurr->y-c);
    XuXian(hdc,psource->x,pcurr->y-c,pcurr->x+c,pcurr->y-c);
	MoveToEx(hdc,pcurr->x+c,pcurr->y-c,NULL);//11
	LineTo(hdc,pcurr->x,pcurr->y);
	


/*	Ellipse(hdc, psource->x+5, psource->y+5, 
		psource->x-5, psource->y-5);
	Ellipse(hdc, pcurr->x+c+5, psource->y+5,
		pcurr->x+c-5, psource->y-5);
	Ellipse(hdc, psource->x+c+5, psource->y+c+5,
		psource->x+c-5, psource->y+c-5);
	Ellipse(hdc, psource->x-c+5, psource->y+c+5,
		psource->x-c-5, psource->y+c-5);
	Ellipse(hdc, psource->x+5, pcurr->y-c+5, 
		psource->x-5, pcurr->y-c-5);
	Ellipse(hdc, pcurr->x+c+5, pcurr->y-c+5,
		pcurr->x+c-5, pcurr->y-c-5);
	Ellipse(hdc, psource->x-c+5, pcurr->y+5, 
		psource->x-c-5, pcurr->y-5);
	Ellipse(hdc, pcurr->x+5, pcurr->y+5,
		pcurr->x-5, pcurr->y-5);*/
}


/*
   Function：画三角形
   Name：Triangle
   Parameter：HDC hdc,const POINT *psource,const POINT *pcurr
   Return：void
*/
void Triangle(HDC hdc,const POINT *psource,const POINT *pcurr)//画 三角形
{
	MoveToEx(hdc,(int)((psource->x+pcurr->x)/2),psource->y,NULL);
	LineTo(hdc,psource->x,pcurr->y);
    MoveToEx(hdc,psource->x,pcurr->y,NULL);
    LineTo(hdc,pcurr->x,pcurr->y);
	MoveToEx(hdc,(int)((psource->x+pcurr->x)/2),psource->y,NULL);
	LineTo(hdc,pcurr->x,pcurr->y);
}

/*
   Function：画六角形
   Name：Sixangle
   Parameter：HDC hdc,const POINT *psource,const POINT *pcurr
   Return：void
*/
void Sexangle(HDC hdc,const POINT *psource,const POINT *pcurr)//画六角形
{
	MoveToEx(hdc,(int)((psource->x+pcurr->x)/2),psource->y,NULL);
	LineTo(hdc,psource->x,(int)((3*psource->y+pcurr->y)/4));
    MoveToEx(hdc,(int)((psource->x+pcurr->x)/2),psource->y,NULL);
	LineTo(hdc,pcurr->x,(int)((3*psource->y+pcurr->y)/4));
	
	MoveToEx(hdc,psource->x,(int)((psource->y+3*pcurr->y)/4),NULL);
	LineTo(hdc,psource->x,(int)((3*psource->y+pcurr->y)/4));
	MoveToEx(hdc,pcurr->x,(int)((psource->y+3*pcurr->y)/4),NULL);
	LineTo(hdc,pcurr->x,(int)((3*psource->y+pcurr->y)/4));
	
	MoveToEx(hdc,pcurr->x,(int)((psource->y+3*pcurr->y)/4),NULL);
	LineTo(hdc,(int)((psource->x+pcurr->x)/2),pcurr->y);
    MoveToEx(hdc,psource->x,(int)((psource->y+3*pcurr->y)/4),NULL);
	LineTo(hdc,(int)((psource->x+pcurr->x)/2),pcurr->y);
}

/*
   Function：重画图形
   Name： ReDraw
   Parameter：HWND hWnd, HDC hdc, NODE * aHead
   Return：void
*/
void ReDraw(HWND hWnd, HDC hdc, NODE * aHead)
{
    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    HBRUSH hBrush = NULL;
    HBRUSH hOldBrush = NULL;
	aHead = aHead->m_pNext;
    while(aHead!=NULL)
    {
        hPen = CreatePen(PS_SOLID, aHead->m_PenWidth, aHead->m_PenColor);
        hBrush = CreateSolidBrush(aHead->m_BrushColor);
        hOldPen = (HPEN)SelectObject(hdc, hPen);
        hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        switch(aHead->m_ImageType)
        {
     	case GRAPH:
			MoveToEx(hdc,aHead->m_SourcePoint.x,aHead->m_SourcePoint.y,NULL);//确定画线的起点
			LineTo(hdc,aHead->m_DestPoint.x,aHead->m_DestPoint.y);
			break;
		case LINE:
			MoveToEx(hdc,aHead->m_SourcePoint.x,aHead->m_SourcePoint.y,NULL);//确定画线的起点
			LineTo(hdc,aHead->m_DestPoint.x,aHead->m_DestPoint.y);
			break;
		case RECTANGLE:
			Rectangle(hdc,aHead->m_SourcePoint.x,aHead->m_SourcePoint.y,aHead->m_DestPoint.x,aHead->m_DestPoint.y);
			break;
		case ELLIPSE:
			Ellipse(hdc,aHead->m_SourcePoint.x,aHead->m_SourcePoint.y,aHead->m_DestPoint.x,aHead->m_DestPoint.y);
			break;
		case CUBE:
			Cube(hdc,&(aHead->m_SourcePoint),&(aHead->m_DestPoint));
			break;
		case TRIANGLE:
			Triangle(hdc,&(aHead->m_SourcePoint),&(aHead->m_DestPoint));
			break;
		case SEXANGLE:
			Sexangle(hdc,&(aHead->m_SourcePoint),&(aHead->m_DestPoint));
		    break;

        }
        //判定图形对象是否要绘制终点
        if(aHead->m_FinishPoint)
            DrawFinishPoint(hWnd, aHead);
        aHead = aHead->m_pNext;
        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);
    }
}

/*
   Function：图像另存为
   Name： SaveAs
   Parameter：HWND hWnd
   Return：BOOL
*/
BOOL SaveAs(HWND aHwnd)
{
    TCHAR szOpenFileNames[80*MAX_PATH];
    TCHAR szOpenFileTitle[MAX_PATH];
    
    OPENFILENAME ofn;
    DWORD numberOfBytesToWrite = 0;
    HANDLE   hf = NULL;//文件句柄
    FILEHEADER fileHeader;//文件头的结点
    NODE *p = head->m_pNext;//向文件写链表时，用于遍历链表
    fileHeader.bfType = MAKEWORD('D','W');
    fileHeader.bfSize = ReturnElementCount(head);
    fileHeader.bfOffBits = sizeof(FILEHEADER);
    
    ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFile = szOpenFileNames;//Pointer to a buffer that contains a filename used to initialize the File Name edit control. 
    ofn.nMaxFile = sizeof(szOpenFileNames);
    ofn.lpstrFile[0] = '\0';
    ofn.hwndOwner = aHwnd;
    ofn.hInstance = g_hInstance;  
    ofn.lpstrFilter = TEXT("图形文件(*.draw)\0 *.draw\0");
    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST; 
    ofn.lpstrDefExt = TEXT("draw"); //Points to a buffer that contains the default extension
    ofn.lpstrFileTitle = szOpenFileTitle;//Pointer to a buffer that receives the filename and extension (without path information) of the selected file. This member can be NULL. 
    ofn.lpstrTitle = TEXT("另存为");
    
    //The GetSaveFileName function creates a Save common dialog box that lets the user specify the drive, directory, and name of a file to save. 
    GetSaveFileName(&ofn);
    hf = CreateFile(ofn.lpstrFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hf == INVALID_HANDLE_VALUE)//创建位图文件失败则返回
    {
        return FALSE; 
    }
    WriteFile(hf, &fileHeader, sizeof(FILEHEADER), &numberOfBytesToWrite,NULL);
    while(p)
    {
        WriteFile(hf, p, sizeof(NODE), &numberOfBytesToWrite, NULL);
        p = p->m_pNext;
    }
    CloseHandle(hf);
    isEdit = FALSE;
    return TRUE;
}

/*
   Function：保存图像
   Name： Save
   Parameter：HWND hWnd
   Return：BOOL
*/
BOOL Save(HWND aHwnd)
{
    static TCHAR szOpenFileNames[80*MAX_PATH];
    static TCHAR szOpenFileTitle[MAX_PATH];
    static OPENFILENAME ofn;
    DWORD numberOfBytesToWrite = 0;
    HANDLE   hf = NULL;//文件句柄
    FILEHEADER fileHeader;//文件头的结点
    NODE *p = head->m_pNext;//向文件写链表时，用于遍历链表 
    
    if(nCount++==0)
    {
        ZeroMemory(&ofn,sizeof(ofn));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.lpstrFile = szOpenFileNames;//Pointer to a buffer that contains a filename used to initialize the File Name edit control. 
        ofn.nMaxFile = sizeof(szOpenFileNames);
        ofn.lpstrFile[0] = '\0';
        ofn.hwndOwner = aHwnd;
        ofn.hInstance = g_hInstance;  
        ofn.lpstrFilter = TEXT("图形文件(*.draw)\0 *.draw\0");
        ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST; 
        ofn.lpstrDefExt = TEXT("draw"); //Points to a buffer that contains the default extension
        ofn.lpstrFileTitle = szOpenFileTitle;//Pointer to a buffer that receives the filename and extension (without path information) of the selected file. This member can be NULL. 
        ofn.lpstrTitle = TEXT("保存");
        
        GetSaveFileName(&ofn);
    }
    hf = CreateFile(
        ofn.lpstrFile, 
        GENERIC_WRITE, 
        FILE_SHARE_READ,
        NULL, 
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, 
        NULL
        );
    if (hf == INVALID_HANDLE_VALUE)//创建位图文件失败则返回
    {
        return FALSE; 
    }
    
    fileHeader.bfType = MAKEWORD('D','W');
    fileHeader.bfSize = ReturnElementCount(head);
    fileHeader.bfOffBits = sizeof(FILEHEADER);
    
    WriteFile(hf, &fileHeader, sizeof(FILEHEADER), &numberOfBytesToWrite,NULL);
    while(p)
    {
        WriteFile(hf, p, sizeof(NODE), &numberOfBytesToWrite, NULL);
        p = p->m_pNext;
    }
    CloseHandle(hf);
    isEdit = FALSE;
    return TRUE;
}

/*
   Function：打开图形文件
   Name：Open
   Parameter：HWND hWnd
   Return：BOOL
*/
BOOL Open(HWND aHwnd)
{
    nCount = 0;
    
    TCHAR szOpenFileNames[80*MAX_PATH];
    TCHAR szOpenFileTitle[MAX_PATH];
    
    OPENFILENAME ofn;
    DWORD numberOfBytesToRead = 0;//存放读了多少字节
    HANDLE   fileHandle = NULL;//文件句柄
    FILEHEADER fileHeader;//存放读出的文件头
    int tag = 0;//存放消息框的返回值
    RECT rect;//存放窗口客户区
    
    ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFile = szOpenFileNames;//Pointer to a buffer that contains a filename used to initialize the File Name edit control. 
    ofn.nMaxFile = sizeof(szOpenFileNames);
    ofn.lpstrFile[0] = '\0';
    ofn.hwndOwner = aHwnd;
    ofn.hInstance = g_hInstance;  
    ofn.lpstrFilter = TEXT("图形文件(*.draw)\0 *.draw\0");
    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST; 
    ofn.lpstrDefExt = TEXT("draw"); //Points to a buffer that contains the default extension
    ofn.lpstrFileTitle = szOpenFileTitle;//Pointer to a buffer that receives the filename and extension (without path information) of the selected file. This member can be NULL. 
    ofn.lpstrTitle = TEXT("打开");
    GetOpenFileName(&ofn);
    fileHandle = CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)//打开位图文件失败则返回
    {
        //MessageBox(aHwnd, "打开文件失败！", "ERROR", IDOK);
        return FALSE; 
    }
    ReadFile(fileHandle, &fileHeader, sizeof(FILEHEADER), &numberOfBytesToRead, NULL);
    if(fileHeader.bfType != MAKEWORD('D', 'W'))//判断打开的文件是否为合法
    {
        MessageBox(aHwnd, "无法打开该类型的文件！", "ERROR", IDOK);
        return FALSE;
    }
    
    //读取文件的数据生成链表
    ReadFileToLink(aHwnd, fileHandle, fileHeader);
    GetClientRect(aHwnd, &rect);
    InvalidateRect(aHwnd, &rect, TRUE);
    UpdateWindow(aHwnd);
    CloseHandle(fileHandle);
    
    isEdit = FALSE;
    
    return TRUE;
}

/*
   Function：读出文件进行连接
   Name： * ReadFileToLink
   Parameter：HWND aHwnd, HANDLE aFileHandle, FILEHEADER aFileHeader
   Return：NODE*
*/
NODE * ReadFileToLink(HWND aHwnd, HANDLE aFileHandle, FILEHEADER aFileHeader)
{
    NODE * pRear = NULL;
    NODE * pCurrent = NULL;
    DWORD numberOfBytesToRead = 0;//存放读了多少字节
    DestoryList(head);
    InitList(&head);
    pRear = head;
    int nodeCount = aFileHeader.bfSize-1;//存放结点个数
    while(nodeCount--)
    {
        pCurrent = new NODE;
        pCurrent->m_pNext = NULL;
        ReadFile(aFileHandle, pCurrent, sizeof(NODE), &numberOfBytesToRead, NULL);
        pRear->m_pNext = pCurrent;
        pRear = pCurrent;
    }
    
    return head;
}


/*
   Function：新建文件
   Name： New
   Parameter：NODE * aHead, HWND aHwnd
   Return：BOOL
*/
BOOL New(NODE * aHead, HWND aHwnd)
{
    nCount = 0;
    
    RECT rect;
    ClearList(aHead);
    GetClientRect(aHwnd, &rect);
    InvalidateRect(aHwnd, &rect, TRUE);
    UpdateWindow(aHwnd);  
    isEdit = FALSE;
    
    return TRUE;
}


/*
   Function：判断是否保存成功
   Name： IsSave
   Parameter：HWND aHwnd
   Return：BOOL
*/
BOOL IsSave(HWND aHwnd)
{
    int tag = 0;
    //是否保存已编辑但未保存的文件！
    if(isEdit == TRUE)
    {   
        
        tag = MessageBox(aHwnd, "是否保存已有文件？", "提示1", MB_OKCANCEL|MB_ICONQUESTION);
        if(tag == IDOK)
        {
            Save(aHwnd);
            MessageBox(aHwnd, "保存成功", "提示", MB_OK);
        }
        return TRUE;
    }  
    else
        return FALSE;
}


/*
   Function：保存热区域
   Name： SaveHotRegion
   Parameter：NODE *ap,HR *arr,int index
   Return：BOOL
*/
void SaveHotRegion( NODE *ap,HR *arr,int index)
{
    arr[index].SourcePoint 
        = ap->m_SourcePoint;
    arr[index].DestPoint 
        = ap->m_DestPoint;
    arr[index].HotRegionHandle 
        = CreateRectRgn
        (
        ap->m_SourcePoint.x, 
        ap->m_SourcePoint.y, 
        ap->m_DestPoint.x, 
        ap->m_DestPoint.y
        );
}


/*
   Function：判定制定的点pt是否在一创建的热点区内，如果不是反回0，否则返回序号(序号从1开始)
   Name： SaveHotRegion
   Parameter：POINT *pt,HR *arr,int index
   Return：int
*/
//判定制定的点pt是否在一创建的热点区内，如果不是反回0，否则返回序号(序号从1开始)
INT InHotRegion(POINT *pt,HR *arr,int index)
{
    for(int j=index-1; j>=0; j--)
        if(PtInRegion(arr[j].HotRegionHandle, 
            pt->x, pt->y))
            return j+1;
    return 0;   
}


/*
   Function：指向要选中的图形对象
   Name： SelectImage
   Parameter：HWND hWnd, NODE * aHead, INT aNumberOfImage
   Return：BOOL
*/
BOOL SelectImage(HWND hWnd, NODE * aHead, INT aNumberOfImage)
{
    //指向要选中的图形对象
    while(aNumberOfImage)
    {
        aHead = aHead->m_pNext;
        aNumberOfImage--;
    }
    //选取图形
    aHead->m_FinishPoint = TRUE;
    return DrawFinishPoint(hWnd, aHead);
}

/*
   Function：画尾节点
   Name： DrawFinishPoint
   Parameter：HWND hWnd, NODE * ptr
   Return：BOOL
*/
BOOL DrawFinishPoint(HWND hWnd, NODE * ptr)
{
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    HDC hdc = GetDC(hWnd);
    hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
    hBrush = CreateSolidBrush(RGB(255, 255, 0));
    hOldPen = (HPEN)SelectObject(hdc, hPen);
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    
    switch(ptr->m_ImageType)
    {
    case LINE:
        Ellipse(hdc, ptr->m_SourcePoint.x+5, ptr->m_SourcePoint.y+5, 
            ptr->m_SourcePoint.x-5, ptr->m_SourcePoint.y-5);
        Ellipse(hdc, ptr->m_DestPoint.x+5, ptr->m_DestPoint.y+5, 
            ptr->m_DestPoint.x-5, ptr->m_DestPoint.y-5);
        ReleaseDC(hWnd, hdc);
        return TRUE;
        break;
    case RECTANGLE:
        Ellipse(hdc, ptr->m_SourcePoint.x+5, ptr->m_SourcePoint.y+5, 
            ptr->m_SourcePoint.x-5, ptr->m_SourcePoint.y-5);
        Ellipse(hdc, ptr->m_DestPoint.x+5, ptr->m_SourcePoint.y+5,
            ptr->m_DestPoint.x-5, ptr->m_SourcePoint.y-5);
        Ellipse(hdc, ptr->m_SourcePoint.x+5, ptr->m_DestPoint.y+5,
            ptr->m_SourcePoint.x-5, ptr->m_DestPoint.y-5);
        Ellipse(hdc, ptr->m_DestPoint.x+5, ptr->m_DestPoint.y+5, 
            ptr->m_DestPoint.x-5, ptr->m_DestPoint.y-5);
        ReleaseDC(hWnd, hdc);
        return TRUE;
        break;

	case ELLIPSE://椭圆
		Ellipse(hdc, ptr->m_SourcePoint.x+5, ptr->m_SourcePoint.y+5, 
            ptr->m_SourcePoint.x-5, ptr->m_SourcePoint.y-5);
        Ellipse(hdc, ptr->m_DestPoint.x+5, ptr->m_SourcePoint.y+5,
            ptr->m_DestPoint.x-5, ptr->m_SourcePoint.y-5);
        Ellipse(hdc, ptr->m_SourcePoint.x+5, ptr->m_DestPoint.y+5,
            ptr->m_SourcePoint.x-5, ptr->m_DestPoint.y-5);
        Ellipse(hdc, ptr->m_DestPoint.x+5, ptr->m_DestPoint.y+5, 
            ptr->m_DestPoint.x-5, ptr->m_DestPoint.y-5);
        ReleaseDC(hWnd, hdc);
        return TRUE;
		
		break;
	case CUBE://立方体
		DrawFinishPointCube(hWnd,hdc,&(ptr->m_SourcePoint),&(ptr->m_DestPoint));
		return TRUE;
		break;
	case TRIANGLE:// 三角形
		Ellipse(hdc, (int)((ptr->m_SourcePoint.x+ptr->m_DestPoint.x)/2)+5, ptr->m_SourcePoint.y+5, 
            (int)((ptr->m_SourcePoint.x+ptr->m_DestPoint.x)/2)-5, ptr->m_SourcePoint.y-5);
        Ellipse(hdc, ptr->m_SourcePoint.x+5, ptr->m_DestPoint.y+5,
            ptr->m_SourcePoint.x-5, ptr->m_DestPoint.y-5);
        Ellipse(hdc, ptr->m_DestPoint.x+5, ptr->m_DestPoint.y+5, 
            ptr->m_DestPoint.x-5, ptr->m_DestPoint.y-5);
        ReleaseDC(hWnd, hdc);
		return TRUE;
		break;
	case SEXANGLE://六角形
	
		return TRUE;
		break;
    default:
        ReleaseDC(hWnd, hdc);
        return FALSE;
        break;
    }
}


/*
   Function：画立方体各个顶点的坐标
   Name：DrawFinishPointCube
   Parameter：HWND hwnd,HDC hdc,const POINT *psource,const POINT *pcurr
   Return：void
*/
void DrawFinishPointCube(HWND hwnd,HDC hdc,const POINT *psource,const POINT *pcurr)
{
  
	int a=2*(int)(pcurr->x-psource->x),c=a/2;
	Ellipse(hdc, psource->x+5, psource->y+5, 
		psource->x-5, psource->y-5);
	Ellipse(hdc, pcurr->x+c+5, psource->y+5,
		pcurr->x+c-5, psource->y-5);
	Ellipse(hdc, psource->x+c+5, psource->y+c+5,
		psource->x+c-5, psource->y+c-5);
	Ellipse(hdc, psource->x-c+5, psource->y+c+5,
		psource->x-c-5, psource->y+c-5);
	Ellipse(hdc, psource->x+5, pcurr->y-c+5, 
		psource->x-5, pcurr->y-c-5);
	Ellipse(hdc, pcurr->x+c+5, pcurr->y-c+5,
		pcurr->x+c-5, pcurr->y-c-5);
	Ellipse(hdc, psource->x-c+5, pcurr->y+5, 
		psource->x-c-5, pcurr->y-5);
	Ellipse(hdc, pcurr->x+5, pcurr->y+5,
		pcurr->x-5, pcurr->y-5);	
        ReleaseDC(hwnd, hdc);

	
}

/*
   Function：删尾节点
   Name：DeleFinishPoint
   Parameter：HWND hWnd, NODE *aHead, INT aNumberOfImage
   Return：BOOL
*/
BOOL DeleFinishPoint(HWND hWnd, NODE *aHead, INT aNumberOfImage)
{
    HDC hdc = GetDC(hWnd);
    RECT rect;
    //指向要选中的图形对象
    while(aNumberOfImage)
    {
        aHead = aHead->m_pNext;
        aNumberOfImage--;
    }
    //删除图形对象的端点
    aHead->m_FinishPoint = FALSE;
    ReDraw(hWnd, hdc, head);
    GetClientRect(hWnd, &rect);
    InvalidateRect(hWnd, &rect, TRUE);
    UpdateWindow(hWnd);
    ReleaseDC(hWnd, hdc);
    return TRUE;
}

/*
   Function：设置图像的属性
   Name：SetImageProperity
   Parameter：NODE * aHead, INT aCurrentNumberOfImage,HWND hWnd, WPARAM wParam
   Return：BOOL
*/
void SetImageProperity(NODE * aHead, INT aCurrentNumberOfImage,HWND hWnd, WPARAM wParam)
{
    
    HDC hdc = GetDC(hWnd);
    RECT rect;
    //指向要设置的图形对象
    while(aCurrentNumberOfImage)
    {
        aHead = aHead->m_pNext;
        aCurrentNumberOfImage--;
    }
    
    
    switch(LOWORD(wParam))
    {
    case ID_GRAPH:
		(*aHead).m_ImageType=GRAPH;
		break;
	case ID_LINE:
		(*aHead).m_ImageType=LINE;
		break;
	case ID_RECT:
		(*aHead).m_ImageType=RECTANGLE;
		break;
	case ID_ELLIPSE:
		(*aHead).m_ImageType=ELLIPSE;
		break;
    case ID_CUBE:
		(*aHead).m_ImageType=CUBE;
		break;
	case ID_TRIANGLE://三角形
		(*aHead).m_ImageType=TRIANGLE;
		break;
		
	case ID_SEXANGLE://六边形
		(*aHead).m_ImageType=SEXANGLE;
		break;
	case ID_NULL:
		(*aHead).m_ImageType=ZERO;
		break;
	case ID_ONE:
		(*aHead).m_PenWidth=1;
		break;
	case ID_FIVE:
		(*aHead).m_PenWidth=5;
		break;
	case ID_TEN:
		(*aHead).m_PenWidth=10;
		break;
	case ID_PEN_BLUE:
		(*aHead).m_PenColor=RGB(0,0,255);
		break;
	case ID_PEN_RED:
		(*aHead).m_PenColor=RGB(255,0,0);
		break;
	case ID_PEN_GREEN:
		(*aHead).m_PenColor=RGB(0,255,0);
		break;
	case ID_BRUSH_RED:
		(*aHead).m_BrushColor=RGB(255,0,0);
		break;
	case ID_BRUSH_BLUE:
		(*aHead).m_BrushColor=RGB(0,0,255);
		break;
	case ID_BRUSH_GREEN:
		(*aHead).m_BrushColor=RGB(0,255,0);
		break;
	case ID_ERASER:
		(*aHead).m_ImageType=ERASER;
		(*aHead).m_PenColor=RGB(255,0, 255);
		(*aHead).m_PenWidth=5;
		break;
	
    }
    GetClientRect(hWnd, &rect);
    InvalidateRect(hWnd, &rect, TRUE);
    UpdateWindow(hWnd);
    ReleaseDC(hWnd, hdc);
    isEdit = TRUE;
}


void MoveNode(NODE *aHead,INT aNumberOfImage,INT x0,INT y0,INT x1,INT y1)
{

	//指向要选中的图形对象
    while(aNumberOfImage)
    {
        aHead = aHead->m_pNext;
        aNumberOfImage--;
    }

	aHead->m_DestPoint.x=aHead->m_DestPoint.x+x1-x0;
	aHead->m_DestPoint.y=aHead->m_DestPoint.y+y1-y0;
	aHead->m_SourcePoint.x=aHead->m_SourcePoint.x+x1-x0;
	aHead->m_SourcePoint.y=aHead->m_SourcePoint.y+y1-y0;

}