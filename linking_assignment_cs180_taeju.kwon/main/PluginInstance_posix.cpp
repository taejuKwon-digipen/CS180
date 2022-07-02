// Taeju Kwon
// Linking Assignment
// CS180
// 2021 Fall
#include "PluginInstance.h"
#include "plugin_api.h"

#include <dlfcn.h>

// if the call to dlopen() fails then you should call dlerror() to get the error description

class PluginInstance::Impl
{
    private:
        std::filesystem::path file_path;
        void* hinstLib;
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
            hinstLib = dlopen(file_path.c_str(), RTLD_LAZY ); 
            
            if(hinstLib == NULL)
            {
                dlerror();
            } //error check

            plug_init init = reinterpret_cast<plug_init>(dlsym(hinstLib, "PluginInit" ));  
            if( init == NULL ) 
            {
                dlerror();
            } 
            is_init = (*init)(plugin_factory);

            plug_display display = reinterpret_cast<plug_display>(dlsym(hinstLib, "PluginDisplayName" )); 
            if( display == NULL)
            {
                dlerror();
            }
            description = (*display)();
        }
    void Unload() const noexcept
    {
        dlclose(hinstLib);
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