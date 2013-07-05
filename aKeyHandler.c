/*
 *  gamelib - keyHandler.c - Keyhandling.
 *
 *  This file contains functions to handle key events from the iPod.
 *
 *  Copyright 2005 Benjamin Eriksson & Mattias Pierre.
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

#include <stdlib.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <linux/kd.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h> // For O_NONBLOCK
#include "aKeyHandler.h"
#include "d_event.h"
#include "doomdef.h"
#include "d_event.h"

static int     consoleFd = -1;
static struct  termios old;
static int    keymap[255];

void finishWithMessage(char * msg)
{
	fprintf(stderr, msg);
	closeKeyHandler();
	exit(-1);
}

int startKeyHandler(void)
{
	struct termios new;
	int err;
		
	const char *dev_name = "/dev/tty";
// 	const char *dev_name = "/dev/console";

	if ((consoleFd = open(dev_name, O_NONBLOCK | O_RDONLY)) < 0)
		fprintf(stderr, "Could not open %s\n", dev_name);
	
	if (tcgetattr(consoleFd, &old) < 0)
		fprintf(stderr, "Could not save old termios\n");
	
	new = old;
	
	new.c_lflag    &= ~(ICANON | ECHO  | ISIG);
	new.c_iflag    &= ~(ISTRIP | IGNCR | ICRNL | INLCR | IXOFF | IXON | BRKINT);
	new.c_cc[VMIN]  = 0;
	new.c_cc[VTIME] = 0;
	
	if (tcsetattr(consoleFd, TCSAFLUSH, &new) < 0)
		finishWithMessage( "Could not set new termios\n");

	err = ioctl(consoleFd, KDSKBMODE, K_MEDIUMRAW);
	if( err < 0)
		printf( "Could not set keyboardmode %s (%d)\n", strerror(errno), errno);

	return 0;
}

void closeKeyHandler(void)
{	
	int err = ioctl(consoleFd, KDSKBMODE, K_XLATE);
	if( err < 0)
		printf( "Could not set keyboardmode %s (%d)\n", strerror(errno), errno);

	if (tcsetattr(consoleFd, TCSAFLUSH, &old) < 0)
	fprintf(stderr, "Could not reset old termios\n");
	
	if (consoleFd > 2)
		if (close(consoleFd) < 0)
			fprintf(stderr, "Could not close console\n");
}

int i_getKey(void)
{	
	int c = 0;
	
	int err = read(consoleFd, &c, 1);

	if( err == 0 ){
		c = -1;
	} else if( err < 0 ){
printf("error while reading console %s (%d)\n", strerror(errno), errno);
		c = -1;
	} else {
printf("got key %d\n", c);
	}

	return c;
}

boolean getEventFromButton(int button, event_t * event)
{
	event->data1 = keymap[KEYCODE(button)];
	
	if (event->data1 < 1)
		return false;
	
	if(KEYSTATE(button) == KEY_PRESSED)
		event->type = ev_keydown;
	else
		event->type = ev_keyup;
	
printf("event yes data1 %d\n", event->data1);
	return true;
}

char getKeyEquiv(char * string, boolean real)
{
	if (string == NULL)
		return 0;
	
	if (real)
	{
		// Real keys
		if (strncmp(string, "RIGHTARROW",10) == 0) return KEY_RIGHTARROW;
		if (strncmp(string, "LEFTARROW" , 9) == 0) return KEY_LEFTARROW;
		if (strncmp(string, "UPARROW"   , 7) == 0) return KEY_UPARROW;
		if (strncmp(string, "DOWNARROW" , 9) == 0) return KEY_DOWNARROW;
		if (strncmp(string, "ESCAPE"    , 6) == 0) return KEY_ESCAPE;
		if (strncmp(string, "ENTER"     , 5) == 0) return KEY_ENTER;
		if (strncmp(string, "TAB"       , 3) == 0) return KEY_TAB;
		if (strncmp(string, "F1"        , 2) == 0) return KEY_F1;
		if (strncmp(string, "F2"        , 2) == 0) return KEY_F2;
		if (strncmp(string, "F3"        , 2) == 0) return KEY_F3;
		if (strncmp(string, "F4"        , 2) == 0) return KEY_F4;
		if (strncmp(string, "F5"        , 2) == 0) return KEY_F5;
		if (strncmp(string, "F6"        , 2) == 0) return KEY_F6;
		if (strncmp(string, "F7"        , 2) == 0) return KEY_F7;
		if (strncmp(string, "F8"        , 2) == 0) return KEY_F8;
		if (strncmp(string, "F9"        , 2) == 0) return KEY_F9;
		if (strncmp(string, "F10"       , 3) == 0) return KEY_F10;
		if (strncmp(string, "F11"       , 3) == 0) return KEY_F11;
		if (strncmp(string, "F12"       , 3) == 0) return KEY_F12;
		if (strncmp(string, "BACKSPACE" , 9) == 0) return KEY_BACKSPACE;
		if (strncmp(string, "PAUSE"     , 5) == 0) return KEY_PAUSE;
		if (strncmp(string, "EQUALS"    , 6) == 0) return KEY_EQUALS;
		if (strncmp(string, "MINUS"     , 5) == 0) return KEY_MINUS;
		if (strncmp(string, "RSHIFT"    , 6) == 0) return KEY_RSHIFT;
		if (strncmp(string, "RCTRL"     , 5) == 0) return KEY_RCTRL;
		if (strncmp(string, "RALT"      , 4) == 0) return KEY_RALT;
		if (strncmp(string, "LALT"      , 4) == 0) return KEY_LALT;
		if (strncmp(string, "SPACE"     , 5) == 0) return ' ';
		
		// Bool values
		if (strncmp(string, "YES"       , 3) == 0) return 1;
		if (strncmp(string, "NO"        , 2) == 0) return 0;
		
		// Regular characters
		return string[0];
	}
	else
	{
		// Archos Keys
		if (strncmp(string, "KEY_MENU"    , strlen("KEY_MENU")) == 0) return KEY_MENU;
		if (strncmp(string, "KEY_ATAB"    , strlen("KEY_ATAB")) == 0) return KEY_ATAB;  
		if (strncmp(string, "KEY_LEFT"    , strlen("KEY_LEFT")) == 0) return KEY_LEFT;    
		if (strncmp(string, "KEY_RIGHT"   , strlen("KEY_RIGHT")) == 0) return KEY_RIGHT; 
		if (strncmp(string, "KEY_UP"      , strlen("KEY_UP")) == 0) return KEY_UP;  
		if (strncmp(string, "KEY_DOWN"    , strlen("KEY_DOWN")) == 0) return KEY_DOWN;    
		if (strncmp(string, "KEY_PAGEUP"  , strlen("KEY_PAGEUP")) == 0) return KEY_PAGEUP; 
		if (strncmp(string, "KEY_PAGEDOWN", strlen("KEY_PAGEDOWN")) == 0) return KEY_PAGEDOWN;
		if (strncmp(string, "KEY_OK"      , strlen("KEY_OK")) == 0) return KEY_OK;  
		if (strncmp(string, "KEY_STOP"    , strlen("KEY_STOP")) == 0) return KEY_STOP;    
		if (strncmp(string, "KEY_VOLUP"   , strlen("KEY_VOLUP")) == 0) return KEY_VOLUP; 
		if (strncmp(string, "KEY_VOLDOWN" , strlen("KEY_VOLDOWN")) == 0) return KEY_VOLDOWN;
	}
		
	return 0;
}

void parseKeyMapFile(char * fileName)
{
	char   buffer[128];
	char * delim = " \t";
	char * commentPos;
	int iPodKey, realKey;
	FILE * keyFile;
	
	// Set the default values.
	keymap[KEY_MENU     ] = KEY_ENTER;
	keymap[KEY_ATAB     ] = KEY_ESCAPE;
	keymap[KEY_LEFT     ] = KEY_LEFTARROW;
	keymap[KEY_RIGHT    ] = KEY_RIGHTARROW;
	keymap[KEY_UP       ] = KEY_UPARROW;
	keymap[KEY_DOWN     ] = KEY_DOWNARROW;
	keymap[KEY_PAGEUP   ] = ' ';
	keymap[KEY_PAGEDOWN ] = 'r';
	keymap[KEY_Y        ] = 'y';
	
	if ((keyFile = fopen(fileName, "r")) == 0)
		return; // There is no actual error in not having a key-def file.
	
	while (fgets(buffer, 128, keyFile))
	{
		if ((commentPos = strchr(buffer, '#')) != NULL)
			*commentPos = '\0';
		
		char * tok1 = strtok(buffer, delim);
		char * tok2 = strtok(NULL,   delim);
		
		iPodKey = getKeyEquiv(tok1, false);
		realKey = getKeyEquiv(tok2, true );
		
		if (iPodKey == 0 || realKey == 0)
		    continue;
		
printf("mapping %20s to %20s\n", tok1, tok2);
		keymap[iPodKey] = realKey;
	}
	
	fclose(keyFile);
}

int getKey()
{	
	return i_getKey();
}
