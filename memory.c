#include "memory.h"

//---------------------------------------------------------------------------------------------------
// 
//	----------	[SECTION] PROCESS
//
//---------------------------------------------------------------------------------------------------

BOOL Process::GetProcessID(const char* title, DWORD& dwProcID);

uintptr_t Proces::GetModuleBase(DWORD dwProcID, const char* dwModule);

BOOL Process::GetProcessInfo(const char* title, PROCESS_INFO& outInfo);

BOOL Process::GetModuleInfo(DWORD dwProcID, const char* dwModule, MODULE_INFO& outInfo);


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