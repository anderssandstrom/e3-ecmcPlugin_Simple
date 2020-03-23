
e3-ecmcPlugin_Simple  
======
ESS Site-specific EPICS module : ecmcPlugin_Simple

Simple Example illustrating a plugin for use with ecmc. Shows both use of callbacks and implementation of custom ecmc plc-functions.

# Interface
The interface is defined in the structure ecmcPluginData in ecmcPluginDefs.h:
```
struct ecmcPluginData {
  // Name 
  const char *name;
  // Plugin version
  int version;
  // ECMC_PLUG_VERSION_MAGIC
  int ifVersion;
  // Optional construct func, called once at load
  int (*constructFnc)(void);
  // Optional destruct func, called once at unload
  void (*destructFnc)(void);
  // Optional func that will be called once just before enter realtime mode
  int (*realtimeEnterFnc)(void*);
  // Optional func that will be called once just before exit realtime mode
  int (*realtimeExitFnc)(void);
  // Optional func that will be called each realtime cycle
  int (*realtimeFnc)(int);
  // Allow max ECMC_PLUGIN_MAX_FUNC_COUNT custom plc functions
  struct ecmcOnePlcFunc funcs[ECMC_PLUGIN_MAX_PLC_FUNC_COUNT];
};
```
## Callbacks:

### constructFnc (optional)
This callback is called once when the plugin is loaded into ecmc. This is a good place to put code for any initialization needed in the plugin module.
If not used then set "ecmcPluginData.constructFnc=NULL".

### destructFnc (optional)
This callback is called once when the plugin is unloaded. This is a good place to put cleanup code needed by the plugin module.
If not used then set "ecmcPluginData.destructFnc=NULL".

### realtimeFnc (optional)
This callback is called once in each realtime loop (sync to ecmc). This is a good place to put any cyclic processing needed by the plugin module.
If not used then set "ecmcPluginData.realtimeFnc=NULL".

### realtimeEnterFnc (optional)
This callback is called once just before ecmc enters realtime mode (starts rt-thread). This is a good place to make any prepartions needed before cyclic processing starts.
If not used then set "ecmcPluginData.enterRealTimeFnc=NULL".

### realtimeExitFnc (optional)
This callback is called once just before ecmc exits realtime mode (exits rt-thread).
If not used then set "ecmcPluginData.exitRealTimeFnc=NULL".

### Example:
```
struct ecmcPluginData pluginDataDef = {
  // Name 
  .name = "ecmcExamplePlugin",
  // Plugin version
  .version = ECMC_EXAMPLE_PLUGIN_VERSION,
  // ECMC_PLUG_VERSION_MAGIC
  .ifVersion = ECMC_PLUG_VERSION_MAGIC, 
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
...
...

```
## PLC functions:
Custom ecmc PLC-functions can be implemnted in plugins. Currentlly the interface supports implementation of up to 32 plc functions. Each plc function needs to be defined by the struct "ecmcOnePlcFunc":
```
struct ecmcOnePlcFunc {
  // Function name (this is the name you use in ecmc plc-code)
  const char *funcName;
  // Number of arguments in the function prototytpe
  int argCount;
  /**
   * 7 different prototypes allowed (only doubles since reg in plc).
   * Only funcArg${argCount} func shall be assigned the rest set to NULL
   * funcArg${argCount}. These need to match. 
   **/
  double (*funcArg0)();
  double (*funcArg1)(double);
  double (*funcArg2)(double,double);
  double (*funcArg3)(double,double,double);
  double (*funcArg4)(double,double,double,double);
  double (*funcArg5)(double,double,double,double,double);
  double (*funcArg6)(double,double,double,double,double,double);
};

```
Example:
```
.funcs[0] =      
      { /*----customPlcFunc1----*/
        // Function name (this is the name you use in ecmc plc-code)
        .funcName = "ex_plugin_func_1",
        // Number of arguments in the function prototytpe
        .argCount = 2,
        /**
        * 7 different prototypes allowed (only doubles since reg in plc).
        * Only funcArg${argCount} func shall be assigned the rest set to NULL.
        * funcArg${argCount}. These need to match. 
        **/
        .funcArg0 = NULL,
        .funcArg1 = NULL,
        .funcArg2 = customPlcFunc1, // Func 1 has 2 args
        .funcArg3 = NULL,
        .funcArg4 = NULL,
        .funcArg6 = NULL,
        .funcArg6 = NULL
      },
  
```
Note: Only the funcArg${argCount} pointer will be used, so set the rest to NULL.
