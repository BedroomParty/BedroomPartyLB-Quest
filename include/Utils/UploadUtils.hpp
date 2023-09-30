#pragma once

#include <functional>
#include <string>

namespace BedroomPartyLB::UploadUtils
{
    void TryUploadScore(std::string url, std::string body);
    void UploadScore(std::string url, std::string requestBody, std::function<void(bool,int)> callback);
}
