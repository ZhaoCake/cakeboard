#pragma once
#include "cakeboard.h"
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

namespace cakeboard {

class DeviceFactory {
public:
    static std::shared_ptr<Device> createDevice(
        const std::string& type,
        const std::string& id,
        const nlohmann::json& config
    );
};

} // namespace cakeboard 