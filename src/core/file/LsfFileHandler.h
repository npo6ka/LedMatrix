#pragma once

#ifndef DESP12F
#   include "Lsf12eFileHandler.h"
    using LsfFileHandler = Lsf12eFileHandler;
#else
#   include "Lsf32FileHandler.h"
    using LsfFileHandler = Lsf32FileHandler;
#endif
