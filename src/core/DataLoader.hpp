#pragma once

#include "GameData.hpp"
#include <string>

namespace core {

class DataLoader {
public:
    data::GameDatabase loadAll(const std::string& dataPath);
    void saveSettings(const std::string& path, const data::SettingsData& settings);
    void saveProgress(const std::string& path, const data::SaveData& save);
};

} // namespace core

