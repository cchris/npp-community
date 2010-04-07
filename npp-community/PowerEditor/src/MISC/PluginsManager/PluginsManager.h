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

#ifndef MISC_PLUGINSMANAGER_PLUGINSMANAGER_H
#define MISC_PLUGINSMANAGER_PLUGINSMANAGER_H

#ifndef MISC_PLUGINSMANAGER_PLUGININTERFACE_H
#include "MISC/PluginsManager/PluginInterface.h"
#endif

typedef BOOL (__cdecl * PFUNCISUNICODE)();

struct PluginCommand {
	generic_string _pluginName;
	int _funcID;
	PFUNCPLUGINCMD _pFunc;
	PluginCommand(const TCHAR *pluginName, int funcID, PFUNCPLUGINCMD pFunc): _funcID(funcID), _pFunc(pFunc), _pluginName(pluginName){};
};

struct PluginInfo {
	PluginInfo();
	~PluginInfo();

	HINSTANCE _hLib;
	HMENU _pluginMenu;

	PFUNCSETINFO _pFuncSetInfo;
	PFUNCGETNAME _pFuncGetName;
	PBENOTIFIED	_pBeNotified;
	PFUNCGETFUNCSARRAY _pFuncGetFuncsArray;
	PMESSAGEPROC _pMessageProc;
	PFUNCISUNICODE _pFuncIsUnicode;

	FuncItem *_funcItems;
	int _nbFuncItem;
	generic_string _moduleName;
};

class PluginsManager {
public:
	PluginsManager() : _hPluginsMenu(NULL), _isDisabled(false) {};
	~PluginsManager();
	void init(const NppData & nppData) {
		_nppData = nppData;
	};

    int loadPlugin(const generic_string& pluginFilePath, std::vector<generic_string> & dll2Remove);
	bool loadPlugins(const TCHAR *dir = NULL);

    bool unloadPlugin(int index, HWND nppHandle);

	void runPluginCommand(size_t i);
	void runPluginCommand(const TCHAR *pluginName, int commandID);

    void addInMenuFromPMIndex(int i);
	void setMenu(HMENU hMenu, const TCHAR *menuName);
	bool getShortcutByCmdID(int cmdID, ShortcutKey *sk);

	void notify(SCNotification *notification);
	void relayNppMessages(UINT Message, WPARAM wParam, LPARAM lParam);
	bool relayPluginMessages(UINT Message, WPARAM wParam, LPARAM lParam);

	HMENU getMenuHandle() {
		return _hPluginsMenu;
	};

	void disable() {_isDisabled = true;};
	bool hasPlugins(){return (_pluginInfos.size()!= 0);};

private:
	NppData _nppData;
	HMENU _hPluginsMenu;

	std::vector<PluginInfo *> _pluginInfos;
	std::vector<PluginCommand> _pluginsCommands;
	bool _isDisabled;

	void pluginCrashAlert(const TCHAR *pluginName, const TCHAR *funcSignature);
};

#define EXT_LEXER_DECL __stdcall

// External Lexer function definitions...
typedef int (EXT_LEXER_DECL *GetLexerCountFn)();
typedef void (EXT_LEXER_DECL *GetLexerNameFn)(unsigned int Index, char *name, int buflength);
typedef void (EXT_LEXER_DECL *GetLexerStatusTextFn)(unsigned int Index, TCHAR *desc, int buflength);

#endif //MISC_PLUGINSMANAGER_PLUGINSMANAGER_H
