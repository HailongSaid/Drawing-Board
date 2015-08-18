#include "windows.h"
#include "function.h"
#include "resource.h"

HINSTANCE g_hInstance = NULL;
NODE * head = NULL;
/*
isEdit�ж��Ƿ�Ӧ�ñ���ͼ�Σ�
����ӻ���ɾ��ͼ�ζ���ʱ��ΪTRUE��
������SaveFile()����ΪFALSE
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
    static BOOL hasBeenDraw = FALSE;//�ж��Ƿ����ڻ�ͼ, ������Draw()��Ϊ�棬����ͼ��lButtonUpʱΪ��
    HPEN oldPen, hPen;
    HBRUSH oldBrush, hBrush;
    INT mouse_x0,mouse_y0;
    static HR HotArr[MAX_SIZE];
    static INT index = 0;
    static INT CurrentNumberOfImage, LastNumberOfImage = 0;//ѡ���˵ڼ���ͼ�Σ�û���ƶ����κ�ͼ���򷵻�0
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
        //��ȡ����������ʱ������λ��
        TempPoint.x = GET_X_LPARAM(lParam);
        TempPoint.y = GET_Y_LPARAM(lParam);
        mouse_x0= GET_X_LPARAM(lParam);mouse_y0=GET_Y_LPARAM(lParam);
        //�ж�����ʱ�Ĺ���Ƿ����ȵ������ڣ�InHotRegion��������ѡȡ�˵ڼ���ͼ��
        CurrentNumberOfImage = InHotRegion(&TempPoint, HotArr, index);
        //���ȵ������ڵ�������������ѡȡͼ��
        if(CurrentNumberOfImage)
        {
            SelectImage(hWnd, head, CurrentNumberOfImage);
            //CurrentNumberOfImage != LastNumberOfImage���ڷ�ֹѡ��ͼ�κ��ٴε���ѡ�и�ͼɾ���յ�
            if(LastNumberOfImage && CurrentNumberOfImage !=LastNumberOfImage)
                DeleFinishPoint(hWnd, head, LastNumberOfImage);
            LastNumberOfImage = CurrentNumberOfImage;//����ѡȡ�˵ڼ���ͼ�� 
            break;
        }
        //���û�����ȵ������ڵ���������Ѿ�ѡȡ��ͼ�Σ���ɾ����ѡȡͼ�ε��յ�
        else if(LastNumberOfImage)
        {
            DeleFinishPoint(hWnd, head, LastNumberOfImage);
            LastNumberOfImage = 0;//����ɾ����һ��ѡȡ��ͼ�κ�ʹ�øñ���Ϊ0����ʾû���κ���ѡȡ��ͼ��
        }
        //���Ҫ��ͼ���˾ͱ���������굽ͼ�α���
        if(CurrentNode.m_ImageType != ZERO)
        {
            MidPoint.x = CurrentNode.m_SourcePoint.x = GET_X_LPARAM(lParam);
            MidPoint.y = CurrentNode.m_SourcePoint.y = GET_Y_LPARAM(lParam);
        }
        break;
    case WM_LBUTTONUP:
        lButtonDown = FALSE;
        
        //����Ѿ������˾ͱ����յ����굽ͼ�α���
        if(hasBeenDraw)//&&CurrentNode.m_ImageType!=NULL&&CurrentNode.m_ImageType!=GRAPH)
        {
            CurrentNode.m_DestPoint.x = GET_X_LPARAM(lParam);
            CurrentNode.m_DestPoint.y = GET_Y_LPARAM(lParam);
            PushBack(head, &CurrentNode);//��ͼ�α���CurrentNode׷��������
            //����ͼ�ε��ȵ���, �����ȵ������浽�ȵ�������
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
        //�����ǰ�����ȵ������ڣ���ı������״
        if(InHotRegion(&CurrentPoint, HotArr, index))
        {
            SetCursor(LoadCursor(NULL, IDC_SIZEALL));
        }
        //���ѡ����ͼ�β��������������������ͼ��
        if(TRUE == lButtonDown && CurrentNode.m_ImageType != ZERO)
        {
            //�������ʺͻ�ˢ��ѡ���豸����hdc
            hdc = GetDC(hWnd);
            oldPen = NULL;
            hPen = CreatePen(PS_SOLID, CurrentNode.m_PenWidth, CurrentNode.m_PenColor);
            oldPen = (HPEN)SelectObject(hdc, hPen);
            hBrush = CreateSolidBrush(CurrentNode.m_BrushColor);
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            //����ͼ��



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
    1 ��ƴ�����
    2 ע�ᴰ����
    3 �������ڣ�creatwindow)
    4 ��ʾ���ڣ�showwindow)
    5 ���´��ڣ�updatewindow)
    6 ��Ϣѭ��*/


	g_hInstance = hInstance;
    TCHAR WndClassName[] = TEXT("MyFirstWindow");
    TCHAR WndTitle[]     = TEXT("������");
    MSG msg;
	WNDCLASSEX wcex;    // ����һ��������ṹ��
    // �������ṹ��wcex
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
    wcex.hIconSm       = NULL;  // ����Сͼ��
    RegisterClassEx(&wcex); // ����API����ע�ᴰ����

	HWND hWnd;
    // ��������
    hWnd = CreateWindow(WndClassName, WndTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    if(!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);  // ��ʾ����
    UpdateWindow(hWnd);          // ���´���

	// ����Ϣѭ��:
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);    // ת����Ϣ
        DispatchMessage(&msg);     // ������Ϣ
    }
	
    return msg.wParam;
}
