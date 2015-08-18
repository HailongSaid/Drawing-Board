#include "windows.h"
#include "function.h"
#include "resource.h"

HINSTANCE g_hInstance = NULL;
NODE * head = NULL;
/*
isEdit判定是否应该保存图形，
当添加或者删除图形对象时设为TRUE，
当调用SaveFile()后设为FALSE
*/
BOOL isEdit = FALSE;

INT nCount = 0;

LRESULT CALLBACK WndProc(HWND    hWnd,   UINT   message,
                         WPARAM  wParam, LPARAM lParam
                         )
{
	HMENU       hMenu;
    PAINTSTRUCT ps;
    HDC         hdc;
    static NODE CurrentNode = {ZERO, RGB(0, 0, 0), 1, RGB(255, 255, 255), 0, 0, FALSE, NULL, FALSE};
    static BOOL lButtonDown = FALSE;
    static POINT MidPoint, CurrentPoint;
    POINT TempPoint;
    static BOOL hasBeenDraw = FALSE;//判定是否正在绘图, 当调用Draw()后为真，画完图，lButtonUp时为假
    HPEN oldPen, hPen;
    HBRUSH oldBrush, hBrush;
    INT mouse_x0,mouse_y0;
    static HR HotArr[MAX_SIZE];
    static INT index = 0;
    static INT CurrentNumberOfImage, LastNumberOfImage = 0;//选择了第几个图形，没有移动到任何图形则返回0
    switch(message)
    {
    case WM_CREATE:
        hMenu = LoadMenu(g_hInstance, (LPCTSTR)(IDR_MENU1));
         SetMenu(hWnd, hMenu);
        InitList(&head);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        ReDraw(hWnd, hdc, head);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        EntryShapeElem(hWnd, wParam, &CurrentNode, HotArr, &index);
        if(CurrentNumberOfImage)
            SetImageProperity(head, CurrentNumberOfImage, hWnd, wParam);
        break;
    case WM_LBUTTONDOWN:
        lButtonDown = TRUE;
        //获取单击鼠标左键时，光标的位置
        TempPoint.x = GET_X_LPARAM(lParam);
        TempPoint.y = GET_Y_LPARAM(lParam);
        mouse_x0= GET_X_LPARAM(lParam);mouse_y0=GET_Y_LPARAM(lParam);
        //判定单击时的光标是否在热点区域内，InHotRegion函数返回选取了第几个图形
        CurrentNumberOfImage = InHotRegion(&TempPoint, HotArr, index);
        //在热点区域内单击了鼠标左键则选取图形
        if(CurrentNumberOfImage)
        {
            SelectImage(hWnd, head, CurrentNumberOfImage);
            //CurrentNumberOfImage != LastNumberOfImage用于防止选中图形后，再次单击选中该图删除终点
            if(LastNumberOfImage && CurrentNumberOfImage !=LastNumberOfImage)
                DeleFinishPoint(hWnd, head, LastNumberOfImage);
            LastNumberOfImage = CurrentNumberOfImage;//保存选取了第几个图形 
            break;
        }
        //如果没有在热点区域内单击左键但已经选取了图形，则删除已选取图形的终点
        else if(LastNumberOfImage)
        {
            DeleFinishPoint(hWnd, head, LastNumberOfImage);
            LastNumberOfImage = 0;//！！删除上一次选取的图形后使得该变量为0，表示没有任何已选取的图形
        }
        //如果要画图形了就保存起点坐标到图形变量
        if(CurrentNode.m_ImageType != ZERO)
        {
            MidPoint.x = CurrentNode.m_SourcePoint.x = GET_X_LPARAM(lParam);
            MidPoint.y = CurrentNode.m_SourcePoint.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_LBUTTONUP:
        lButtonDown = FALSE;
        
        //如果已经画完了就保存终点坐标到图形变量
        if(hasBeenDraw)//&&CurrentNode.m_ImageType!=NULL&&CurrentNode.m_ImageType!=GRAPH)
        {
            CurrentNode.m_DestPoint.x = GET_X_LPARAM(lParam);
            CurrentNode.m_DestPoint.y = GET_Y_LPARAM(lParam);
            PushBack(head, &CurrentNode);//将图形变量CurrentNode追加入链表
            //创建图形的热点区, 并将热点区保存到热点数组中
            SaveHotRegion(
                &CurrentNode, 
                HotArr, 
                index++
                );
            hasBeenDraw = FALSE;
        }







        break;
    case WM_MOUSEMOVE:
        CurrentPoint.x = GET_X_LPARAM(lParam);
        CurrentPoint.y = GET_Y_LPARAM(lParam);
        //如果当前点在热点区域内，则改变鼠标形状
        if(InHotRegion(&CurrentPoint, HotArr, index))
        {
            SetCursor(LoadCursor(NULL, IDC_SIZEALL));
        }
        //如果选择了图形并按下了鼠标左键，则绘制图形
        if(TRUE == lButtonDown && CurrentNode.m_ImageType != ZERO)
        {
            //创建画笔和画刷并选入设备环境hdc
            hdc = GetDC(hWnd);
            oldPen = NULL;
            hPen = CreatePen(PS_SOLID, CurrentNode.m_PenWidth, CurrentNode.m_PenColor);
            oldPen = (HPEN)SelectObject(hdc, hPen);
            hBrush = CreateSolidBrush(CurrentNode.m_BrushColor);
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            //绘制图形



			int noldrop=0;
			if (CurrentNode.m_ImageType!=ERASER)
			{
				noldrop=SetROP2(hdc,R2_NOTXORPEN);//R2_NOTXORPEN  R2_NOTCOPYPEN
				
			} 
			else
			{
				noldrop=SetROP2(hdc,R2_WHITE);//R2_NOTXORPEN  R2_NOTCOPYPEN
				
	}



            hasBeenDraw = Draw(hdc, CurrentNode.m_ImageType, &CurrentNode.m_SourcePoint, &MidPoint, &CurrentPoint, hWnd);
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(hPen);
            DeleteObject(hBrush);
            ReleaseDC(hWnd, hdc);
            break;     
        }



   	/*CurrentNumberOfImage = InHotRegion(&TempPoint, HotArr, index);
     if(CurrentNumberOfImage)
	  {
		  MoveNode(head,CurrentNumberOfImage,mouse_x0,mouse_y0,GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)  );
		  ReDraw(hWnd,hdc,head);
	  }
	  */
	 

        break;
    case WM_DESTROY:
        DestoryList(head);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




int WINAPI WinMain(
                   HINSTANCE hInstance,      // handle to current instance
                   HINSTANCE hPrevInstance,  // handle to previous instance
                   LPSTR lpCmdLine,          // pointer to command line
                   int nCmdShow              // show state of window
                   )
{
   
    /*
    1 设计窗口类
    2 注册窗口类
    3 创建窗口（creatwindow)
    4 显示窗口（showwindow)
    5 更新窗口（updatewindow)
    6 消息循环*/


	g_hInstance = hInstance;
    TCHAR WndClassName[] = TEXT("MyFirstWindow");
    TCHAR WndTitle[]     = TEXT("启航队");
    MSG msg;
	WNDCLASSEX wcex;    // 定义一个窗口类结构体
    // 下面填充结构体wcex
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style		   = CS_HREDRAW | CS_VREDRAW; 
    wcex.lpfnWndProc   = WndProc; 
    wcex.cbClsExtra	   = 0;
    wcex.cbWndExtra	   = 0;
    wcex.hInstance	   = hInstance;
    wcex.hIcon		   = NULL;
    wcex.hCursor	   = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName  = NULL;//LPCTSTR(ID_MAIN_MENU);
    wcex.lpszClassName = WndClassName;
    wcex.hIconSm       = NULL;  // 窗口小图标
    RegisterClassEx(&wcex); // 调用API函数注册窗口类

	HWND hWnd;
    // 创建窗口
    hWnd = CreateWindow(WndClassName, WndTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    if(!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);  // 显示窗口
    UpdateWindow(hWnd);          // 更新窗口

	// 主消息循环:
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);    // 转换消息
        DispatchMessage(&msg);     // 发放消息
    }
	
    return msg.wParam;
}
