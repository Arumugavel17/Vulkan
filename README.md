# Vulkan Graphics Project

This repository contains a Vulkan-based graphics application with cross-platform support for **Windows** and **Linux**. Follow the instructions below to set up and build the project.

---

## 🪟 Windows Setup (Using Visual Studio)

### ✅ Prerequisites
- [Visual Studio 2022 or later](https://visualstudio.microsoft.com/)
- "Desktop development with C++" workload
- Git
- [Vulkan SDK (LunarG)](https://vulkan.lunarg.com/sdk/home)

### 📦 Setup Steps

1. **Install Visual Studio**
   - Download and install from the [official site](https://visualstudio.microsoft.com/).
   - During installation, select **"Desktop development with C++"**.

2. **Download and Install the Vulkan SDK**
   - Go to the [LunarG Vulkan SDK](https://vulkan.lunarg.com/sdk/home) Downloads page.
   - Download the latest Windows SDK version (e.g., 1.4.309.0).
   - Run the installer and follow the instructions.

3. **Configure Vulkan SDK Environment Variables**
   - The installer will automatically set the following environment variables:
      - VULKAN_SDK – Path to the installed SDK
      - Adds the SDK's Bin directory to the system PATH
   - To verify:
      1. Open the command prompt.
      2. Run: echo %VULKAN_SDK%
      It should print something like: C:\VulkanSDK\1.4.309.0
4. **Clone the Repository**
   ```bash
   git clone --recurse-submodules https://github.com/Arumugavel17/Vulkan.git
   ```

5. **Open the Project in Visual Studio**
   - Launch Visual Studio.
   - Go to `File` → `Open` → `CMake...`
   - Select the root folder of the cloned repository.

6. **Build and Run**
   - Visual Studio will automatically configure the CMake project.
   - Select the build configuration (`Debug` or `Release`) and architecture (`x64`).
   - Use the build and run buttons to compile and execute the project.

---

## 🐧 Linux Setup

### ✅ Prerequisites
Install the required system packages:

```bash
sudo apt install libwayland-dev libxkbcommon-dev \
libxinerama-dev libxcursor-dev libx11-dev libxext-dev \
libxi-dev libxrandr-dev wayland-protocols vulkan-validationlayers
```

Make sure `cmake`, `g++`, and `git` are also installed.

### 📦 Setup Steps

1. **Download and Extract the Vulkan SDK**
   ```bash
   wget https://sdk.lunarg.com/sdk/download/1.4.309.0/linux/vulkansdk-linux-x86_64-1.4.309.0.tar.xz
   tar -xf vulkansdk-linux-x86_64-1.4.309.0.tar.xz
   ```

2. **Set Up the Vulkan Environment**
   ```bash
   cd 1.4.309.0/x86_64/bin
   source .
   cd 1.4.309.0/x86_64/include
   source .
   1.4.309.0/x86_64/bin/lib
   source .
   ```

3. **Clone the Repository**
   ```bash
   git clone --recurse-submodules https://github.com/Arumugavel17/Vulkan.git
   cd Vulkan
   ``` 

   > ℹ️ To persist the environment setup across sessions, add the above `source` line to your `~/.bashrc` or `~/.zshrc`.

4. **Build the Project**
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

5. **Run the Application**
   ```bash
   cd Vulkan (inside build)
   ./Vulkan
   ```

---

## 📌 Notes

- Always clone the repository using `--recurse-submodules` to ensure GLFW and other dependencies are fetched.
- This project uses **CMake** as the build system and includes cross-platform support for **X11** and **Wayland** on Linux.
- The Vulkan SDK is required to build and run the application.

---

## 📂 Repository Structure

```
Vulkan/
├── src/                    # Source files
├── vendor/                 # Submodules like GLFW
├── CMakeLists.txt          # CMake build configuration
└── README.md               # This file
```

---

## 💬 Troubleshooting

- ❗ *"validation layers requested, but not available!"*  
  → Ensure you have installed `vulkan-validationlayers` and sourced the Vulkan SDK properly.

- ❗ *"vkDestroyDebugUtilsMessengerEXT(): Invalid handle"*  
  → This may occur if the debug messenger wasn't created successfully; always check for `VK_SUCCESS`.

---

## 🔗 Resources

- [Vulkan SDK (LunarG)](https://vulkan.lunarg.com/)
- [GLFW](https://www.glfw.org/)
- [Visual Studio](https://visualstudio.microsoft.com/)

---

Happy rendering! 🚀
