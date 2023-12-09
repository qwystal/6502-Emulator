#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "structs.h"
#include "typedefs.h"
#include "declarations.h"

#include "instructions.c"
#include "LDA.c"
#include "LDX.c"
#include "LDY.c"
#include "STA.c"
#include "STX.c"
#include "STY.c"
#include "execute.c"

#include "../tests/log.c"
#include "../tests/LDA-Test.c"
#include "../tests/LDX-Test.c"
#include "../tests/LDY-Test.c"
#include "../tests/6502-Emulator-Test.c"

#endif