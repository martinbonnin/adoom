/*
 *  gamelib - akeyHandler.h - Keyhandling.
 *
 *  Copyright 2005 Martin Bonnin.
 * 
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 */

#ifndef __KEY_HANDLER__
#define __KEY_HANDLER__

#include <sys/time.h>
#include "d_event.h"

#define KEY_MENU             13
#define KEY_ATAB             27
#define KEY_LEFT            107
#define KEY_RIGHT           109
#define KEY_UP              111
#define KEY_DOWN            108
#define KEY_PAGEUP           7
#define KEY_PAGEDOWN         8 
#define KEY_OK               9
#define KEY_STOP            10
#define KEY_VOLUP           11
#define KEY_VOLDOWN         12

#define KEY_RELEASED 0x80
#define KEY_PRESSED     0

#define  KEYCODE(a) (a & 0x7f)  // Use to get keycode of scancode.
#define KEYSTATE(a) (a & 0x80)  // Check if PRESSED or RELEASED.

int  startKeyHandler(void);
int           getKey(void);
void closeKeyHandler(void);
void parseKeyMapFile(char * fileName);
boolean getEventFromButton(int button, event_t * event);

#endif // __KEY_HANDLER__
