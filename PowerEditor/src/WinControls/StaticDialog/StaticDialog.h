//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef WINCONTROLS_STATICDIALOG_STATICDIALOG_H
#define WINCONTROLS_STATICDIALOG_STATICDIALOG_H

#ifndef WINCONTROLS_WINDOW_H
#include "WinControls/Window.h"
#endif

typedef HRESULT (WINAPI * ETDTProc) (HWND, DWORD);

enum PosAlign{ALIGNPOS_LEFT, ALIGNPOS_RIGHT, ALIGNPOS_TOP, ALIGNPOS_BOTTOM};

struct DLGTEMPLATEEX {
      WORD   dlgVer;
      WORD   signature;
      DWORD  helpID;
      DWORD  exStyle;
      DWORD  style;
      WORD   cDlgItems;
      short  x;
      short  y;
      short  cx;
      short  cy;
      // The structure has more fields but are variable length
} ;

class StaticDialog : public Window
{
public :
	StaticDialog() : Window() {};
	virtual ~StaticDialog();
	virtual void create(int dialogID, bool isRTL = false);

	void goToCenter();

	void display(bool toShow = true) const;

	POINT getLeftTopPoint(HWND hwnd) const;

    virtual void destroy();

protected :
	RECT _rc;
	static BOOL CALLBACK dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

    void alignWith(HWND handle, HWND handle2Align, PosAlign pos, POINT & point);
	HGLOBAL makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate);
};

#endif //WINCONTROLS_STATICDIALOG_STATICDIALOG_H
