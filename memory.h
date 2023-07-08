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

/// NEW STRUCTURE TEMPLATE
/*
    typedef struct S<StructName>
    {
        // Member variables
        ...
        ...
        ...
    } <StructSecondary>, <StructPointer>;
*/

/// NEW CLASS TEMPLATE
/*
    * Class DeckyMemory.C<ClassName>
    * Size -> 0x0000 (FullSize[0x0000] - InheritedSize[0x0000])
    class C<ClassName>
    {
    private:	//	"_<VarName>" PRIVATE MEMBER VARIABLES
    public:     //  "<VarName>" PUBLIC MEMBER VARIABLES      
    public:	    //	"_<FncName>" CUSTOM FUNCTIONS for Private Memmbers
    public:	    //	"<FncName>" PUBLIC MEMBER FUNCTIONS
    };  //  Size: 0x0000
*/

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] INCLUDES
//
//---------------------------------------------------------------------------------------------------


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
    contains   various information about a process module
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
    contains various information about a running process
*/
typedef struct PROCESS_INFO
{
    std::string			processName;				//	Literal Process Name
    std::string			cmdPath;					//	"/proc/<pid>/cmd"
    std::string			cmdLine;					//	Process Command Line (launch params)
    DWORD				dwProcID;					//	Process ID
    HANDLE              pHandle;                    //  handle to the process
    MODULE_INFO     	mainModule;					//	first module
    std::vector<struct MODULE_INFO> procModules;	//	list of all modules
} PROCESS_INFORMATION, *PROCESS_INFORMATION;


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

/*
    -   attempts to obtain a processID using the input title (iterates directory -> "/proc/<id>")
    [in] title
    [out] dwProcID
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
    [out] outInfo = ProcessInformation structure
    returns: true if output structure is valid
*/
BOOL GetProcessInfo(const char* title, PROCESS_INFO& outInfo);

/*
    - attempts to read bytes at the destination address in the target process
    [in] dwProcID = target process id
    [in] dst = destination address
    returns: result
*/
template<typename T>
T ReadMemory(DWORD dwProcID, void* dst);

/*
    - attepts to write memory to the target process using the desired address and input buffer
    [in] dwProcID = target process id
    [in] src = destination address
    [in] buffer = bytes to write
    returns: true if success
*/
template<typename T>
BOOL WriteMemory(DWORD dwProcID, void* dst, void* buffer);