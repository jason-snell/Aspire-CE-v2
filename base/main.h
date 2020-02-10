#pragma once

#include <xtl.h>
#include <xboxmath.h>
#include <xbox.h>
#include <stdio.h>
#include <setjmp.h>

#include "Ini.h"
#include "Vectors.h"
#include "kernel.h"
#include "tools.h"
#include "xamNet.h"
#include "sockets.h"
#include "HttpSocket.h"
#include "Detour.h"
#include "structs.h"
#include "dependents.h"
#include "functions.h"
#include "auth.h"
#include "menufunc.h"
#include "drawing.h"
#include "spinbot.h"
#include "aimbot.h"
#include "esp.h"
#include "menu.h"
#include "hooks.h"

void takeDownDetours();
void main();

extern PLDR_DATA_TABLE_ENTRY aspireHandle;