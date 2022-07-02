// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#pragma once
#include <filesystem>
#include <memory>
#include <string>

class PluginFactory;

class PluginInstance
{
public:
    explicit PluginInstance(std::filesystem::path dll_file_path);
    void                                Load(PluginFactory& plugin_factory) const;
    void                                Unload() const noexcept;
    [[nodiscard]] bool                  IsLoaded() const noexcept;
    [[nodiscard]] std::filesystem::path GetFilePath() const noexcept;
    [[nodiscard]] std::string           GetDisplayName() const noexcept;

private:
    class Impl;
    std::shared_ptr<Impl> impl;
};
