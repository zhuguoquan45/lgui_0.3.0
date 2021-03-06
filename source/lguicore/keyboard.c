/*
	Copyright (C) 2004-2005 Li Yudong
*/
/*
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "../include/common.h"
#include "../include/blockheap.h"
#include "../include/invalidregion.h"
#include "../include/clipregion.h"
#include "../include/message.h"
#include "../include/hdc.h"
#include "../include/caret.h"
#include "../include/winnc.h"
#include "../include/winbase.h"
#include "../include/window.h"
#include "../include/lguiapp.h"
#include "../include/keyboard.h"



pthread_t thread_kb;

extern PWindowsTree	_lGUI_pWindowsTree;

void 
InitKeyboard()
{
	InstallKBDevice();
	InitLGUIKBDefine();
	OpenKB();
	//create keyboard thread
	pthread_create(&thread_kb,NULL,
				(void*)KeyboardMainLoop,NULL);

}

void 
TerminateKeyboard()
{
	pthread_cancel(thread_kb);
	pthread_join(thread_kb,NULL);

}

//thread cleanup function
void 
cleanup_closekb(
	void* para
)
{
	//close keyboard
	CloseKB();
}

void* 
KeyboardMainLoop(
	void* para
)
{
	int old_cancel_type;
	BYTE btScanCode;
	BYTE btPressed;

	//register thread cleanup function
	pthread_cleanup_push(cleanup_closekb,NULL);
	while(1){
		pthread_testcancel();
		if(ReadKB(&btScanCode,&btPressed)){
			if(btPressed == 1)
				SendKBMessage((int)btScanCode);
			//printf("%d\n", btScanCode);
		}
	}
	
	//cleanup function
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,&old_cancel_type);
	pthread_cleanup_pop(1);
	pthread_setcanceltype(old_cancel_type,NULL);
}


