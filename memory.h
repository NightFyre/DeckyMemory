/**
 * Name: DeckyMemory
 * Version: 0.0.1
 * OS: Arch Linux
 * Author: NightFyre
 * Contributors:
 - 
 - 
 - 
*/

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] SECTION NAVIGATION & LIBRARY NOTES
//
//---------------------------------------------------------------------------------------------------
/*
NAGIVATION
* search for "[SECTION] < HEADER >" to quickly access a sections contents

# [SECTION] INCLUDES
# [SECTION] DEFINITIONS
# [SECTION] STRUCTURES
# [SECTION] CLASSES
# [SECTION] FUNCTIONS

*/ 

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] INCLUDES
//
//---------------------------------------------------------------------------------------------------
#include "pch.h"

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] DEFINITIONS
//
//---------------------------------------------------------------------------------------------------

#define TRUE                1
#define FALSE               0
#define NULL                0
typedef void*               HANDLE;
typedef wchar_t				WCHAR;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef int                 INT;
typedef unsigned int        UINT;
typedef signed char         INT8,   * PINT8;
typedef signed short        INT16,  * PINT16;
typedef signed int          INT32,  * PINT32;
typedef int64_t             INT64,  * PINT64;
typedef unsigned char       UINT8,  * PUINT8;
typedef unsigned short      UINT16, * PUINT16;
typedef unsigned int        UINT32, * PUINT32;
typedef uint64_t            UINT64, * PUINT64;

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] STRUCTURES
//
//---------------------------------------------------------------------------------------------------

/*
    -   contains   various information about a process module
*/
typedef struct MODULE_INFO
{
    HANDLE			    pHandle;		            //	handle to the module
    uintptr_t 		    baseAddress;	            //	first byte
    uintptr_t 		    endAddress;		            //	last byte
    size_t			    czSize;			            //	sie of module
    std::string 	    dwName;			            //	module name
    std::string		    mapsPath;		            //  "/proc/<pid>/maps"
    std::string		    modulePath;		            //	"/proc/<pid>/maps/<thisModule>"
    std::string		    pFlags;			            //  protection flags
} MODULE_INFORMATION, *MODULE_INFORMATION;

/*
    -   contains various information about a running process
*/
typedef struct PROCESS_INFO
{
    std::string			processName;				//	Literal Process Name
    std::string			cmdPath;					//	"/proc/<pid>/cmdline"
    std::string			cmdLine;					//	Process Command Line (launch params)
    DWORD				dwProcID;					//	Process ID "/proc/<pid>/"
    HANDLE              pHandle;                    //  handle to the process
    MODULE_INFO     	mainModule;					//	first module
    std::vector<struct MODULE_INFO> procModules;	//	list of all modules
} PROCESS_INFORMATION, *PROCESS_INFORMATION;

/*
    -   contains various information about a monitor
*/
typedef struct MONITOR_INFO
{
    int                 displaySizeX;               //  Display Width
    int                 displaySizeY;               //  Display Height
    int                 displayPosX;                //  Display PosX
    int                 displayPosY;                //  Display PosY
    int                 clientSizeX;                //  Client Screen Width    
    int                 clientSizeY;                //  Client Screeb Height 
    GLFWmonitor*        primaryMonitor;             //  Primary Monitor
    GLFWmonitor*        activeMonitor;              //  Active Monitor (determined by mouse cursor pos)
    int32_t             monitorCount;               //  total # of displays
} MONITOR_INFORMATION, *MONITOR_INFORMATION;

/*
    -   contains various information about a window
*/
typedef struct WINDOW_INFO
{
    int                 posX;                       //  window posX
    int                 posY;                       //  window posY
    int                 sizeX;                      //  window sizeX
    int                 sizeY;                      //  window sizeY
    GLFWwindow*         pWindow;                    //  Window Pointer
    MONITOR_INFO        wndwMonitor;                //  MONITOR_INFORMATION structure
} WINDOW_INFORMATION, *WINDOW_INFORMATION;

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] CLASSES
//
//---------------------------------------------------------------------------------------------------

/**
    * Class DeckyMemory.CDeckyArray
    * Size -> 0x0020 (FullSize[0x0020] - InheritedSize[0x0000])
    * Templte Array Class
*/
template<typename T>
class CDeckyArray
{
private:
    int64_t     _count;
    int64_t     _max;
    T*          _data;

public: //  constructors
    DeckyArray();   //  Empty constructor to be filled
    DeckyArray(int64_t initialCount, initialMax, initialDataPtr) : _count(initialCount), _max(initialMax), _data(initialDataPtr) {} // Default Constructor

public: //  operators
    T* operator[](int64_t index) { return (T*)_Data[_index]; }

public: //  functions
    T*          Data() const { return _data; }
    int64_t     Count() const { return _count; }
    int64_t     Max() const { return _max; }
    bool        isValidIndex(int64_t index) const { return index < _count; }

    /*
        * NestedClass DeckyMemory.CDeckyEnumerator
        * Size -> 0x00C (FullSize[0x00C] - InheritedSize[0x000C])
    */
    class CDeckyEnumerator
    {
    private:    //  private
        CDeckyArray&        _array;
        int64_t             _arrayIndex;
    
    public: //  constructor
        CDeckyEnumerator(DeckyArray<T&> inArray, int64_t index) : _array(inArray), _arrayIndex(index) {}

    public: //  operators   
        bool operator!=(const CDeckyEnumerator& other) { return _arrayIndex != other.index; }
        T& operator*() const { return (T&)_array._data[_arrayIndex]; }
        CDeckyEnumerator& operator++()
        {
            ++index;
            return *this;
        }

    public:    //
        CDeckyEnumerator begin() { return CDeckyEnumerator(*this, 0); }
        CDeckyEnumerator end() { return CDeckyEnumerator(*this, _max); }
        void addObject(const T& value)
        {
            if (_count < _max)
                _data[_count++] = value;
        }
        void clear() const 
        {
            _count  = NULL;
            _max    = NULL;
            _data   = nullptr;
        }
    };  //  Size: 0x000C
};  //  Size:   0x0020


//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] FUNCTIONS
//
//---------------------------------------------------------------------------------------------------

///     NOTE:
namespace Process
{
    /*
        -   attempts to obtain a processID using the input title (iterates directory -> "/proc/<id>")
        [in] title = input process name
        [out] dwProcID = output process id
        returns: true or false    
    */
    BOOL GetProcessID(const char* title, DWORD& dwProcID);

    /*
        -   attempts to obtain the module base using the input string
        [in] dwProcID = target process ID
        [in] dwModule = target modules name
        returns: result will be non zero 
    */
    uintptr_t GetModuleBase(DWORD dwProcID, const char* dwModule);

    /*
        -   attempts to obtain all information on a process & fill a ProcessInfomation structure
        [in] title = target process name
        [out] outInfo = ProcessInformation structure
        returns: true if output structure is valid
    */
    BOOL GetProcessInfo(const char* title, PROCESS_INFO& outInfo);

    /*
        -   attempts to obtain a module structure using the input string and processID
        [in] dwProcID = target process id
        [in] dwModule = target module name
        [out] outInfo = output MODULE_INFO structure
        returns: true if output structure is valid
    */
    BOOL GetModuleInfo(DWORD dwProcID, const char* dwModule, MODULE_INFO& outInfo);
}

///     NOTE:   
namespace Memory
{
    /*
        - attempts to read bytes at the destination address in the target process
        [in] dwProcID = target process id
        [in] dst = destination address
        returns: result
    */
    template<typename T>
    T ReadMemory(DWORD dwProcID, void* dst)
    {
        T result = NULL;

        struct iovec local[1];
        local[0].iov_base 	= &result;		//	buffer for bytes read
        local[0].iov_len 	= sizeof(T);	//	Size

        struct iovec remote[1];
        remote[0].iov_base 	= dst;			//	Address to be read
        remote[0].iov_len 	= sizeof(T);	//	size

        ssize_t bytes_read = process_vm_readv(procID, local, 1, remote, 1, 0);
        if (bytes_read == -1)
        {
            // ERROR
        }

        return result;	
    }

    /*
        - attempts to write memory to the target process using the desired address and input buffer
        [in] dwProcID = target process id
        [in] src = destination address
        [in] buffer = bytes to write
        returns: true if success
    */
    template<typename T>
    BOOL WriteMemory(DWORD dwProcID, void* dst, void* buffer)
    {
        struct iovec local[1];
        local[0].iov_base 	= buffer;		//	buffer for bytes written
        local[0].iov_len 	= sizeof(T);	//	Size

        struct iovec remote[1];
        remote[0].iov_base 	= dst;			//	Address to be written to
        remote[0].iov_len 	= sizeof(T);	//	size

        ssize_t bytes_written = process_vm_writev(procID, local, 1, remote, 1, 0);
        if (bytes_written == -1)
        {
            //  ERROR
        }

        return (bytes_written != -1);
    }
}

///     NOTE: USES GLFW
namespace Display
{
    /*
        -   retrieves a pointer to the primary monitor
        returns: primary glfw monitor
    */
    GLFWmonitor* GetPrimaryMonitor();
    
    /*
        -   obtains monitor information on the primary monitor
        [out] outInfo = output MONITOR_INFO structure
        returns: true if output structure is valid
    */
    BOOL GetMonitorInfo(MONITOR_INFO& outInfo);
}

///     NOTE: USES OPENGL3 + GLFW
namespace Window
{
    /*
        -   initialize GLFW library and create initial window
        [in]
        [out] WINDOW_INFO = output window information
        returns: true if window was created and WINDOW_IFO struct is valid
    */
    BOOL initializeGLFW(const char* title, WINDOW_INFO& outInfo);

    /*
        -   initialize dear imgui library & create render context
        [in] GLFWwindow* = window context for render dear imgui
    */
    void initializeDearImGui(GLFWwindow* window);

    /*
        -   render scene for the created window
        GLFWwindow* = window to render scene to
        [in] fnc* = menu function to be rendered
        [out] bool = exit flag
    */
    void PresentScene(GLFWwindow* window, void(*menuFnc), bool& bEXIT);

    /*
        -   destroys the input window context
    */
    void shutdownGLFW(GLFWwindow* window);
    
    /*
        -   destroys the input dear imgui context
    */
    void shutdownDearImGui();
}

///     NOTE: USES DEAR IMGUI
namespace GUI
{
    /*
        - renders text on GUI much like printf on console / terminal
        [in] text = input text to be displayed
        [in] color = output color
        [...] ... = extra params, string must be constructed properly
    */
    IMGUI_API void TextCentered(const char* text, const ImVec4& color = { 1, 1, 1, 1 }, ...);
}

///     NOTE:
namespace Overlay
{   
    /*
        -   simple dear imgui window
    */
    void menu();
}