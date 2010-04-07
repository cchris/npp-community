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

#include "precompiled_headers.h"
#include "WinControls/ImageListSet/ImageListSet.h"

void IconList::create(HINSTANCE hInst, int iconSize)
{
	InitCommonControls();
	_hInst = hInst;
	_iconSize = iconSize;
	_hImglst = ImageList_Create(iconSize, iconSize, ILC_COLOR32 | ILC_MASK, 0, NB_MAX_COMMANDS);
	if (!_hImglst)
		throw int(25);
};

void IconList::create(int iconSize, HINSTANCE hInst, int *iconIDArray, int iconIDArraySize)
{
	create(hInst, iconSize);
	_pIconIDArray = iconIDArray;
	_iconIDArraySize = iconIDArraySize;

	for (int i = 0 ; i < iconIDArraySize ; i++)
		addIcon(iconIDArray[i]);
};

void IconList::addIcon(int iconID) const
{
	HICON hIcon = ::LoadIcon(_hInst, MAKEINTRESOURCE(iconID));
	if (!hIcon)
		throw int(26);
	ImageList_AddIcon(_hImglst, hIcon);
	::DestroyIcon(hIcon);
};

bool IconList::changeIcon(int index, const TCHAR *iconLocation) const
{
	HBITMAP hBmp = (HBITMAP)::LoadImage(_hInst, iconLocation, IMAGE_ICON, _iconSize, _iconSize, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);
	if (!hBmp)
		return false;
	int i = ImageList_ReplaceIcon(_hImglst, index, (HICON)hBmp);
	ImageList_AddMasked(_hImglst, (HBITMAP)hBmp, RGB(255,0,255));
	::DeleteObject(hBmp);
	return (i == index);
}

void IconList::setIconSize(int size) const
{
	ImageList_SetIconSize(_hImglst, size, size);
	for (int i = 0 ; i < _iconIDArraySize ; i++)
		addIcon(_pIconIDArray[i]);
}

void ToolBarIcons::init(ToolBarButtonUnit *buttonUnitArray, int arraySize)
{
	for (int i = 0 ; i < arraySize ; i++)
		_tbiis.push_back(buttonUnitArray[i]);
	_nbCmd = arraySize;
}

void ToolBarIcons::reInit(int size)
{
	ImageList_SetIconSize(getDefaultLst(), size, size);
	ImageList_SetIconSize(getHotLst(), size, size);
	ImageList_SetIconSize(getDisableLst(), size, size);

	for (int i = 0 ; i < int(_tbiis.size()) ; i++)
	{
		if (_tbiis[i]._defaultIcon != -1)
		{
			_iconListVector[HLIST_DEFAULT].addIcon(_tbiis[i]._defaultIcon);
			_iconListVector[HLIST_HOT].addIcon(_tbiis[i]._hotIcon);
			_iconListVector[HLIST_DISABLE].addIcon(_tbiis[i]._grayIcon);
		}
	}
}

void ToolBarIcons::create(HINSTANCE hInst, int iconSize)
{
	_iconListVector.push_back(IconList());
	_iconListVector.push_back(IconList());
	_iconListVector.push_back(IconList());

	_iconListVector[HLIST_DEFAULT].create(hInst, iconSize);
	_iconListVector[HLIST_HOT].create(hInst, iconSize);
	_iconListVector[HLIST_DISABLE].create(hInst, iconSize);

	reInit(iconSize);
}

ToolBarIcons::~ToolBarIcons()
{
	ToolBarIcons::destroy();
}

void ToolBarIcons::destroy()
{
	_iconListVector[HLIST_DEFAULT].destroy();
	_iconListVector[HLIST_HOT].destroy();
	_iconListVector[HLIST_DISABLE].destroy();
	//_iconListVector[HLIST_UGLY].destroy();
}

bool ToolBarIcons::replaceIcon( int witchList, int iconIndex, const TCHAR *iconLocation ) const
{
	if ((witchList != HLIST_DEFAULT) && (witchList != HLIST_HOT) && (witchList != HLIST_DISABLE))
		return false;
	return _iconListVector[witchList].changeIcon(iconIndex, iconLocation);
}
