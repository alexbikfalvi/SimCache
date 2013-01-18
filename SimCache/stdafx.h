#pragma once
#define _CRTDBG_MAP_ALLOC
#define _CRT_RAND_S

#include "targetver.h"

#include <crtdbg.h>
#include <stdlib.h>
#include <iostream>
#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <list>
#include <map>
#include <set>
#include <vector>

using namespace std;

#define __bitrate					unsigned __int64
#define __time						double
#define __bits						unsigned __int64
#define TYPE_CAST					static_cast

#define TIME_UNIT					((__time)1)
#define TIME_INVALID				((__time)-1)
#define RATE_UNIT					((__bitrate)1000)

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif