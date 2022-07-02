/*
 * Rudy Castan
 * CS180
 */
#include <CPUSpeed.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS     //- CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES //- VK_*
#define NOWINMESSAGES     //- WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       //- WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      //- SM_*
#define NOMENUS           //- MF_*
#define NOICONS           //- IDI_*
#define NOKEYSTATES       //- MK_*
#define NOSYSCOMMANDS     //- SC_*
#define NORASTEROPS       //- Binary and Tertiary raster ops
#define NOSHOWWINDOW      //- SW_*
#define OEMRESOURCE       //- OEM Resource values
#define NOATOM            //- Atom Manager routines
#define NOCLIPBOARD       //- Clipboard routines
#define NOCOLOR           //- Screen colors
#define NOCTLMGR          //- Control and Dialog routines
#define NODRAWTEXT        //- DrawText() and DT_*
#define NOGDI             //- All GDI defines and routines
#define NOKERNEL          //- All KERNEL defines and routines
#define NOUSER            //- All USER defines and routines
// #define NONLS             //- All NLS defines and routines
#define NOMB             //- MB_* and MessageBox()
#define NOMEMMGR         //- GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE       //- typedef METAFILEPICT
#define NOMINMAX         //- Macros min(a,b) and max(a,b)
#define NOMSG            //- typedef MSG and associated routines
#define NOOPENFILE       //- OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL         //- SB_* and scrolling routines
#define NOSERVICE        //- All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND          //- Sound driver routines
#define NOTEXTMETRIC     //- typedef TEXTMETRIC and associated routines
#define NOWH             //- SetWindowsHook and WH_*
#define NOWINOFFSETS     //- GWL_*, GCL_*, associated routines
#define NOCOMM           //- COMM driver routines
#define NOKANJI          //- Kanji support stuff.
#define NOHELP           //- Help engine interface.
#define NOPROFILER       //- Profiler interface.
#define NODEFERWINDOWPOS //- DeferWindowPos routines
#define NOMCX            //- Modem Configuration Extensions
#include <Windows.h>

#include <stdexcept>
#include <string>
#include <string_view>

namespace
{
    std::string utf8_encode(const std::wstring_view& wstr)
    {
        if (wstr.empty())
            return std::string();
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
        std::string strTo(static_cast<std::string::size_type>(size_needed), 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }
}

namespace CS180
{
    double FindCPUSpeedGHz()
    {
        static const double CPUCyclesPerNanoSecond = []
        {
            HKEY       registry_key_handle{};
            const long error_code =
                RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ,
                             &registry_key_handle);

            if (error_code != ERROR_SUCCESS)
            {
                WCHAR Buffer[1024] = {};
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, static_cast<DWORD>(error_code), 0, Buffer, _MAX_PATH,
                              0);
                std::string message = "Failed to get CPU Information :\n";
                message += utf8_encode(Buffer);
                throw std::runtime_error(message);
            }

            DWORD MHz           = 0;
            DWORD size_of_dword = sizeof(MHz);
            RegQueryValueEx(registry_key_handle, L"~MHz", nullptr, nullptr, reinterpret_cast<LPBYTE>(&MHz),
                            &size_of_dword);
            RegCloseKey(registry_key_handle);
            return static_cast<double>(MHz) / 1000.0;
        }();
        return CPUCyclesPerNanoSecond;
    }
}
