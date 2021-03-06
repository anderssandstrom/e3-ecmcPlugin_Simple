/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcPluginExample.cpp
*
*  Created on: Mar 21, 2020
*      Author: anderssandstrom
*
\*************************************************************************/
#define ECMC_IS_PLUGIN
#define ECMC_EXAMPLE_PLUGIN_VERSION 1

// only allow to load once
#define ECMC_PLUGIN_ALLOW_MULTI_LOAD 0

// Error codes
#define ECMC_PLUGIN_ERROR_ALREADY_LOADED 1

#include <stdio.h>
#include "ecmcPluginDefs.h"

static int    loaded         = 0;

/** Optional. 
 *  Will be called once just before ecmc goes into realtime mode.
 *  Return value other than 0 will be considered error.
 **/
int exampleConstruct(char * configStr)
{
  // Ensure that plugin is only loaded once
  if(loaded && !ECMC_PLUGIN_ALLOW_MULTI_LOAD) {
    printf("%s/%s:%d: Error: Module already loaded (0x%x).\n",__FILE__, __FUNCTION__,
           __LINE__,ECMC_PLUGIN_ERROR_ALREADY_LOADED);
    return ECMC_PLUGIN_ERROR_ALREADY_LOADED;
  }

  printf("exampleConstruct...\n");

  // Prevent loading more than once
  loaded = 1;
  return 0;
}

/** Optional function.
 *  Will be called once at unload.
 **/
void exampleDestruct(void)
{
  printf("exampleDestruct...\n");
}

/** Optional function.
 *  Will be called each realtime cycle if definded
 *  ecmcError: Error code of ecmc. Makes it posible for 
 *  this plugin to react on ecmc errors
 *  Return value other than 0 will be considered error.
 **/
int exampleRealtime(int ecmcError)
{
  //printf("exampleRealtime...\n");
  return 0;
}

/** Optional function.
 *  Will be called once just before going to realtime mode
 *  Return value other than 0 will be considered error.
 *  ecmcRefs will be used to pass ecmc objects to lib
 **/
int exampleEnterRT(void){
  printf("exampleEnterRT...\n");
  return 0;
}

/** Optional function.
 *  Will be called once just before leaving realtime mode
 *  Return value other than 0 will be considered error.
 **/
int exampleExitRT(void){
  printf("exampleExitRT...\n");
  return 0;
}

/** Optional plc function*/
double customPlcFunc1(double arg1, double arg2)
{
  printf("customPlcFunc1 %lf, %lf.\n",arg1,arg2);
  return arg1 * arg2;
}

/** Optional plc function*/
double customPlcFunc2(double arg1, double arg2, double arg3)
{
  printf("customPlcFunc2 %lf, %lf, %lf.\n",arg1,arg2,arg3);
  return arg1 * arg2 * arg3;
}

// Compile data for lib so ecmc now what to use
struct ecmcPluginData pluginDataDef = {
   // Allways ECMC_PLUG_VERSION_MAGIC
  .ifVersion = ECMC_PLUG_VERSION_MAGIC, 
  // Name 
  .name = "ecmcExamplePlugin",
  // Description
  .desc = "Simple plugin demo with 2 plc functions",
    // Option description
  .optionDesc = "No Options defined!",
  // Plugin version
  .version = ECMC_EXAMPLE_PLUGIN_VERSION,
  // Optional construct func, called once at load. NULL if not definded.
  .constructFnc = exampleConstruct,
  // Optional destruct func, called once at unload. NULL if not definded.
  .destructFnc = exampleDestruct,
  // Optional func that will be called each rt cycle. NULL if not definded.
  .realtimeFnc = exampleRealtime,
  // Optional func that will be called once just before enter realtime mode
  .realtimeEnterFnc = exampleEnterRT,
  // Optional func that will be called once just before exit realtime mode
  .realtimeExitFnc = exampleExitRT,

  // Allow max ECMC_PLUGIN_MAX_FUNC_COUNT custom funcs
  .funcs[0] =      
      { /*----customPlcFunc1----*/
        // Function name (this is the name you use in ecmc plc-code)
        .funcName = "ex_plugin_func_1",
        // Function description
        .funcDesc = "double ex_plugin_func_1(arg0, arg1) : multiplies arg0 and arg1. Returns the value",
        /**
        * 7 different prototypes allowed (only doubles since reg in plc).
        * Only funcArg${argCount} func shall be assigned the rest set to NULL.
        **/
        .funcArg0 = NULL,
        .funcArg1 = NULL,
        .funcArg2 = customPlcFunc1, // Func 1 has 2 args
        .funcArg3 = NULL,
        .funcArg4 = NULL,
        .funcArg5 = NULL,
        .funcArg6 = NULL,
        .funcArg7 = NULL,
        .funcArg8 = NULL,
        .funcArg9 = NULL,
        .funcArg10 = NULL,
        .funcGenericObj = NULL,
      },
    .funcs[1] =
      { /*----customPlcFunc2----*/
        // Function name (this is the name you use in ecmc plc-code)
        .funcName = "ex_plugin_func_2",
        // Function description
        .funcDesc = "double ex_plugin_func_2(arg0, arg1, arg2) : multiplies arg0 arg1 and arg2. Returns the value",
        /**
        * 7 different prototypes allowed (only doubles since reg in plc).
        * Only funcArg${argCount} func shall be assigned the rest set to NULL.
        **/
        .funcArg0 = NULL,
        .funcArg1 = NULL,
        .funcArg2 = NULL,
        .funcArg3 = customPlcFunc2, // Func 2 has 3 args
        .funcArg4 = NULL,
        .funcArg5 = NULL,
        .funcArg6 = NULL,
        .funcArg7 = NULL,
        .funcArg8 = NULL,
        .funcArg9 = NULL,
        .funcArg10 = NULL,
        .funcGenericObj = NULL,
      },
      .funcs[2] = {0},  //last element set all to zero..
      .consts[0] = {0}, //last element set all to zero..
};

// Register plugin data
ecmc_plugin_register(pluginDataDef);
