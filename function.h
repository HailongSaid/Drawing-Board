#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <windows.h>

#include "Date.h"

extern HINSTANCE g_hInstance;
extern NODE * head;
extern BOOL   isEdit;
extern INT    nCount;


BOOL   InitList(NODE ** aHead);//��ͼ��������г�ʼ����
NODE * ReturnLastElementPoint(NODE * aHead);
void   PushBack(NODE * aHead, NODE * pNode);//����ǰ�ڵ�ŵ����������棻
INT    ReturnElementCount(NODE * aHead);
void   DestoryList(NODE * aHead);//��ͼ��������г�ʼ����
void   ClearList(NODE * aHead);
BOOL New(NODE * aHead, HWND aHwnd);
BOOL SaveAs(HWND aHwnd);
BOOL Save(HWND aHwnd);//���Ϊ ���洰���е�����ͼ�ε���Ϣ
BOOL Open(HWND aHwnd);
NODE * ReadFileToLink(HWND aHwnd, HANDLE aFileHandle, FILEHEADER aFileHeader);
BOOL IsSave(HWND aHwnd);
void EntryShapeElem(HWND hWnd, WPARAM wParam, NODE * CurrentNode, HR * arr, INT *index);//ͨ������˵������Խڵ�ĸ������Խ�������
BOOL Draw(HDC hdc,const SHAPETYPE aShaptype, const POINT *psource,POINT *pmid, POINT *pcurr, HWND aHwnd);//����ָ����ͼ������
void ReDraw(HWND hWnd, HDC hdc, NODE * aHead);
void SaveHotRegion(NODE *ap, HR *arr, int index);
INT InHotRegion(POINT *pt, HR *arr, int index);
BOOL SelectImage(HWND ahWnd, NODE * aHead, INT aNumberOfImage);
BOOL DrawFinishPoint(HWND hWnd, NODE * ptr);
BOOL DeleFinishPoint(HWND hWnd, NODE * aHead, INT aNumberOfImage);
void SetImageProperity(NODE * aHead, INT aCurrentNumberOfImage, HWND hWnd, WPARAM wParam);

void Cube(HDC hdc,const POINT *psource,const POINT *pcurr);//��������
void DrawFinishPointCube(HWND hwnd,HDC hdc,const POINT *psource,const POINT *pcurr);
void Triangle(HDC hdc,const POINT *psource,const POINT *pcurr);//�� ������
void Sexangle(HDC hdc,const POINT *psource,const POINT *pcurr);//��������
void XuXian(HDC hdc,int x1,int y1,int x2,int y2);


void MoveNode(NODE *aHead,INT aNumberOfImage,INT x0,INT y0,INT x1,INT y1);

#endif
