#include "memory.h"

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] PROCESS
//
//---------------------------------------------------------------------------------------------------

BOOL Process::GetProcessID(const char* title, DWORD& dwProcID)
{
    bool result = FALSE;

    //	Open Process Directory
    DIR* pdir = opendir("/proc");	
    if (!pdir)
        return result;

    //	Process Directory Entrypoint
    struct dirent* dirEntry = nullptr;
    while ((dirEntry = readdir(pdir)) != nullptr)
    {
        //	Check if the entry is both a directory & numeric value
        if (dirEntry->d_type == DT_DIR && isdigit(dirEntry->d_name[0]))
        {
            //	obtain process id
            DWORD tempPID = strtol(dirEntry->d_name, nullptr, 10);

            //	Read cmdline , can also parse [com] but cmd is better for obtaining the proc id
            char fileName[64];
            snprintf(fileName, sizeof(fileName), "/proc/%d/cmdline", tempPID);
            FILE* dwFile = fopen(fileName, "r");
            if (dwFile)
            {
                char cmdLine[256];
                if (fgets(cmdLine, sizeof(cmdLine), dwFile) != nullptr)
                {
                    //	Remove trailing newline char
                    cmdLine[strcspn(cmdLine, "\n")] = '\0';

                    //	get the string following the last "/"
                    std::string cmdString(cmdLine);
                    std::size_t lastSlash = cmdString.find_last_of('/');
                    std::string processName = cmdString.substr(lastSlash + 1);

                    //	Compare Process Name
                    if (strncmp(processName.c_str(), title, sizeof(title)))
                    {
                        // Match Found
                        result = TRUE;
                        procID = tempPID;
                        fclose(dwFile);
                        closedir(pdir);
                        return result;
                    }
                }
            }
            fclose(dwFile);
        }
    }
    closedir(pdir);
    return result;
}

uintptr_t Proces::GetModuleBase(DWORD dwProcID, const char* dwModule)
{
    uintptr_t result = NULL;
    MODULE_INFO modInfo;
    if (GetModuleInfo(procID, dwModule, modInfo))
        result = modInfo.baseAddress;
    return result;
}

BOOL Process::GetProcessInfo(const char* title, PROCESS_INFO& outInfo)
{
    DWORD procID;
    if (GetProcessID(title, procID))
    {
        outInfo.processName = std::to_string(title);

        if (GetAllProcessModules(ProcID, outInfo.procModules))
        {
            // Declare Main Module
            outInfo.mainModule = outInfo.procModules[0];
            return TRUE;
        }
    }
    return FALSE;
}

BOOL Process::GetModuleInfo(DWORD dwProcID, const char* dwModule, MODULE_INFO& outInfo)
{
    //	Get modules
    std::vector<SModuleInfo> modules;
    if (GetAllProcessModules(dwProcID, modules))
    {
        // Loop through modules 
        for (auto mod : modules)
        {
            if (strcmp(mod.dwName.c_str(), dwModule, sizeof(mod.dwName))
            {
                module = mod;
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL Process::GetAllProcessModules(DWORD dwProcID, std::vector<MODULE_INFO>& outArray)
{
    // Establish Folder Name
    std::string fileName = "/proc/" + std::to_string(procID) + "/maps";

    //	Open File Stream
    std::ifstream file(fileName);

    //	Read File line by line
    std::string line;
    while (std::getline(file, line))
    {
        //	
        std::istringstream iss(line);
        std::string moduleRange;
        std::string moduleFlags;

        //	
        if (iss >> moduleRange >> moduleFlags)
        {
            size_t pos = moduleRange.find('-');
            if (pos != std::string::npos)
            {
                /*
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
                */
                HANDLE 		handle 		= nullptr;
                uintptr_t 	baseAddress	= std::stoull(moduleRange.substr(0, pos), nullptr, 16);
                uintptr_t 	endAddress 	= std::stoull(moduleRange.substr(pos + 1), nullptr, 16);
                size_t    	size 		= endAddress - baseAddress;
                std::string modName		= "";                       //  @TODO:
                std::string mapspath 	= fileName;
                std::string modulePath  = "";                       //  @TODO:
                std::string flags 		= moduleFlags;  

                modules.push_back({handle, baseAddress, endAddress, size, modName, mapspath, modulePath, flags});
            }
        }
    }
    return (outArray.size() > NULL);
}

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] MEMORY
//
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] DISPLAY
//
//---------------------------------------------------------------------------------------------------

GLFWmonitor* Display::GetPrimaryMonitor() { return glfwGetPrimaryMonitor(); }

BOOL Display::GetMonitorInfo(MONITOR_INFO& outInfo)
{
    bool result = FALSE;
    GLFWmonitor* pMonitor = GetPrimaryMonitor();
    if (pMonitor != nullptr)
    {
        outInfo.primaryMonitor = pMonitor;
        glfwGetMonitorWorkarea(outInfo.primaryMonitor, &outInfo.displayPosX, &outInfo.displayPosY, &outInfo.clientSizeX, &outInfo.clientSizeY);
        result = TRUE;
    }
    return result;
}


//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] WINDOW
//
//---------------------------------------------------------------------------------------------------

BOOL Window::initializeGLFW(const char* title, WINDOW_INFO& outInfo)
{
    if (glfwInit())
        return FALSE;

    MONITOR_INFO mInfo;
    if (!GetMonitorInfo(&mInfo))
        return FALSE;

    //  Create Window
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(mInfo.clientSizeX, mInfo.clientSizeY, title, nullptr, nulltpr);
    if (!window)
    {
        glfwTerminate();
        return FALSE;
    }

    //  Populate structure
    outInfo.pWindow = window;
    outInfo.wndwMonitor = mInfo;
    glfwMakeContextCurrent(window);

    return TRUE;
}


void Window::initializeDearImGui(GLFWwindow* window)
{
    //  Init Dear ImGui Context
    ImGui::CreateContext();
    ImGuiIO& io     = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename  = NULL;
    
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}


void Window::PresentScene(GLFWwindow* window, void(* menuFnc), bool& bEXIT)
{
    if (!glfwWindowShouldClose(window) && bEXIT)
    {
        //  Logic for overlay exit
        return;
    }

    glfwPollEvents();
    
    // Begin Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //  render menu
    menuFnc();   

    //  Render Draw Scene
    ImGui::Render();

    //  End Scene
    glViewport(NULL, NULL, static_cast<int>(ImGui::GetIO().DisplaySize.x), static_cast<int>(ImGui::GetIO().DisplaySize.y));
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //  Swap Buffers
    glfwSwapBuffers(window);
}


void Window::ShutdownDearImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void Window::ShutdownGLFW(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] GUI
//
//---------------------------------------------------------------------------------------------------

IMGUI_API void GUI::TextCentered(const char* text, const ImVec4& color, ...)
{
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(text).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    va_list args;
    va_start(args, text);
    ImGui::TextColored(color, text, args);
    va_end(args);
}

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] Overlay
//
//---------------------------------------------------------------------------------------------------

void Overlay::menu()
{
    if (!ImGui::Begin("##DeckyMemory_ImGuiWindow", &dwExitFlag, wndwFlags | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::End();
        return;
    }

    GUI::TextCentered("Hello World from SteamDeck!", IM_COL32_WHITE);

    ImGui::End();
}