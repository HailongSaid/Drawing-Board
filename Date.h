#ifndef _DATE_H_
#define _DATE_H_

#define MAX_SIZE 1000
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))

//ͼ����״
typedef enum
{
    ZERO,LINE,RECTANGLE,ELLIPSE,GRAPH,TRIANGLE,SEXANGLE,ERASER,CUBE
}SHAPETYPE;

//ͼ��ĳ�����������
typedef struct Node
{
    SHAPETYPE m_ImageType;
    COLORREF  m_PenColor;
    INT       m_PenWidth;
    COLORREF  m_BrushColor;

    POINT     m_SourcePoint;
    POINT     m_DestPoint;
    BOOL      m_FinishPoint;//��¼ͼ���Ƿ���Ҫ���ƶ˵�
    struct Node * m_pNext;
}NODE;

//�ļ�ͷ
typedef struct tagFILEHEADER 
{
    WORD    bfType; 
    int     bfSize;//�ļ��Ĵ�С���߼�����Ƴɽ����� 
    DWORD   bfOffBits;
}FILEHEADER;

/*HotRegion��һ�������ȵ�������Ϣ�Ľṹ��*/
typedef struct HotRegion
{
    HRGN HotRegionHandle;    //�����ȵ�����ľ��
    POINT SourcePoint;       //�ȵ��������
    POINT DestPoint;         //�ȵ������յ�
}HR;

#endif