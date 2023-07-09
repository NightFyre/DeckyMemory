#pragma once

// C++ Standard Libraries
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <chrono>
#include <fstream>
#include <sstream>

// Linux Libs
#include <dirent.h>
#include <unistd.h>
#include <X11/Xlib.h>

//  OpenGL / GLFW
#include <GLFW/glfw3.h>

//  Dear ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"