#ifndef _DATE_H_
#define _DATE_H_

#define MAX_SIZE 1000
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))

//图的形状
typedef enum
{
    ZERO,LINE,RECTANGLE,ELLIPSE,GRAPH,TRIANGLE,SEXANGLE,ERASER,CUBE
}SHAPETYPE;

//图像的抽象数据类型
typedef struct Node
{
    SHAPETYPE m_ImageType;
    COLORREF  m_PenColor;
    INT       m_PenWidth;
    COLORREF  m_BrushColor;

    POINT     m_SourcePoint;
    POINT     m_DestPoint;
    BOOL      m_FinishPoint;//记录图形是否需要绘制端点
    struct Node * m_pNext;
}NODE;

//文件头
typedef struct tagFILEHEADER 
{
    WORD    bfType; 
    int     bfSize;//文件的大小，逻辑上设计成结点个数 
    DWORD   bfOffBits;
}FILEHEADER;

/*HotRegion是一个保存热点区域信息的结构体*/
typedef struct HotRegion
{
    HRGN HotRegionHandle;    //矩形热点区域的句柄
    POINT SourcePoint;       //热点区的起点
    POINT DestPoint;         //热点区的终点
}HR;

#endif