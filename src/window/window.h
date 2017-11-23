#pragma once

#include "../base.h"
#include <Windows.h>
#include <windowsx.h>
#include <d2d1.h>

class Window {
public:
	Window();
	virtual ~Window();

	void Create(const stdstring & title, int x, int y, int w, int h);
	void Move(int x, int y, int w, int h);
	void Show(bool visible);
	void Loop();

	RECT GetRect();
	HWND GetHandle();
	SIZE GetContent();
	POINT GetPosition();

protected:
	virtual void OnMouse(u_int msg, int x, int y, u_int key, int wheel) = 0;
	virtual void OnPaint(ID2D1HwndRenderTarget * pHRT) = 0;
	virtual void OnKeyboard(int flag, int key) = 0;
	virtual void OnResize(int w, int h) = 0;

private:
	static LRESULT CALLBACK WindowMessage(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam);
	void Cls_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void Cls_OnMButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void Cls_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void Cls_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys);
	void Cls_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT keyFlags);
	void Cls_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
	void Cls_OnMButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
	void Cls_OnRButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
	void Cls_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
	void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);
	void Cls_OnPaint(HWND hwnd);
	void Cls_OnClose(HWND hwnd);

private:
	HWND _hWnd;
	ID2D1Factory * _pDxFactory;
	ID2D1HwndRenderTarget * _pDxHRT;
};