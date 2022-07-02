// Taeju Kwon
// Linking Assignment
// CS180
// 2021 Fall
#include "PluginInstance.h"
#include "plugin_api.h"

#include <sstream>

#define NAME_OF(v) #v

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // SM_*
#define NOMENUS           // MF_*
#define NOICONS           // IDI_*
#define NOKEYSTATES       // MK_*
#define NOSYSCOMMANDS     // SC_*
#define NORASTEROPS       // Binary and Tertiary raster ops
#define NOSHOWWINDOW      // SW_*
#define OEMRESOURCE       // OEM Resource values
#define NOATOM            // Atom Manager routines
#define NOCLIPBOARD       // Clipboard routines
#define NOCOLOR           // Screen colors
#define NOCTLMGR          // Control and Dialog routines
#define NODRAWTEXT        // DrawText() and DT_*
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
#define NOUSER            // All USER defines and routines
#define NONLS             // All NLS defines and routines
#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
#define NOMINMAX          // Macros min(a,b) and max(a,b)
#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions

#include <Windows.h>

namespace 
{
    // Call this if LoadLibary fails to get the error descriptin
    std::string GetLastWin32ErrorAsString()
    {
        // Get the error message, if any.
        const DWORD error_message_id = ::GetLastError();
        if (error_message_id == 0)
            return std::string(); // No error message has been recorded

        LPSTR        message_buffer = nullptr;
        const size_t size =
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           nullptr, error_message_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           reinterpret_cast<LPSTR>(&message_buffer), 0, nullptr);
        if (!message_buffer)
            return "Error ID : " + std::to_string(error_message_id);

        std::ostringstream message;
        message << "Error ID : " << error_message_id << '\t' << message_buffer;
        // Free the buffer.
        LocalFree(message_buffer);

        return message.str();
    }
}
//typedef double (*DLL_SUM)(double a, double b); 
// -> double 리턴값 / a, b parameter
class PluginInstance::Impl
{
    private:
        std::filesystem::path file_path;
        HINSTANCE hinstLib;
        std::string description;
        bool is_init;
        typedef bool (*plug_init)(PluginFactory& plugin_factory);
        typedef const char* (*plug_display)();
    public:
        explicit Impl(std::filesystem::path dll_file_path)
        {
           file_path = dll_file_path;
        }

        void Load(PluginFactory& plugin_factory) 
        {
            hinstLib = LoadLibrary(file_path.c_str());    //ostype handle to dll
            
            if(hinstLib == NULL)
            {
                GetLastWin32ErrorAsString();
            } //error check

            plug_init init = reinterpret_cast<plug_init>(GetProcAddress(hinstLib, "PluginInit" ));  
            if( init == NULL ) 
            {
                GetLastWin32ErrorAsString();
            } 
            is_init = (*init)(plugin_factory);

            plug_display display = reinterpret_cast<plug_display>(GetProcAddress(hinstLib, "PluginDisplayName" )); 
            if( display == NULL)
            {
                GetLastWin32ErrorAsString();
            }
            description = (*display)();
        }
    void Unload() const noexcept
    {
        FreeLibrary(hinstLib);
    }
    [[nodiscard]] bool IsLoaded() const noexcept
    {
        if(hinstLib == NULL)
        {
            return false;
        }else 
        {
            return true;
        }
    }

    [[nodiscard]] std::filesystem::path GetFilePath() const noexcept
    {
        return file_path;
    }
    [[nodiscard]] std::string GetDisplayName() const noexcept
    {
        return description;
    }
};

    PluginInstance::PluginInstance(std::filesystem::path dll_file_path)
    {
        impl = std::make_shared<Impl>(dll_file_path);
    }

    void PluginInstance::Load(PluginFactory& plugin_factory) const
    {
        return impl->Load(plugin_factory);
    }

    void PluginInstance::Unload() const noexcept
    {
        return impl->Unload();
    }

    [[nodiscard]] bool PluginInstance::IsLoaded() const noexcept
    {
        return impl->IsLoaded();
    }

    [[nodiscard]] std::filesystem::path PluginInstance::GetFilePath() const noexcept
    {
        return impl->GetFilePath();
    }

    [[nodiscard]] std::string PluginInstance::GetDisplayName() const noexcept
    {
        return impl->GetDisplayName();
    }
    