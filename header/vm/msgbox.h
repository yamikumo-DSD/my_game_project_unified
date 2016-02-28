//msgbox.h

#ifndef __MSG_BOX_H
#define __MSG_BOX_H

#include <windows.h>

#define msgbox_ok(msg) MessageBox(NULL, TEXT(msg), TEXT("MessageBox"),MB_OK);

#endif