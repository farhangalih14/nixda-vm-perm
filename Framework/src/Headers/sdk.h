#pragma once
#include <string>
#include "json.hpp"
#pragma warning(disable : 4996)

#define SAFEGUARD_EXPORTS

#ifdef _WIN32
#ifdef SAFEGUARD_EXPORTS
#define SAFEGUARD_API __declspec(dllexport)
#else
#define SAFEGUARD_API __declspec(dllimport)
#endif
#else
#define SAFEGUARD_API
#endif

class SDK
{
public:
    SAFEGUARD_API SDK(const std::string& sdk_key);
    SAFEGUARD_API bool isSessionValid();
    SAFEGUARD_API int registerUser(const std::string& username, const std::string& key);        // returns the level of the redeemed key. -1 is faliure. Otherwise its good
    SAFEGUARD_API bool authenticateUser(const std::string& username);                           // Self explanatory
    SAFEGUARD_API bool extendSubscription(const std::string& username, const std::string& key); // Self explanatory
    SAFEGUARD_API bool getVariable(const std::string& name, nlohmann::json& result, std::string sessionToken);
    SAFEGUARD_API bool downloadFile(const std::string& fileName, bool toRam); // If toRAM is true, it dowloads it to m_fileContent. Use getFileConteny to get the bytearray
    // NOTE: THIS FUNCTION DOES NOT WORK YET, PLEASE DO NOT TRY TO USE IT.
    SAFEGUARD_API bool injectDLL(const std::string& fileName, const std::string& targetProcessName); // Needs to be like explorer.exe   
    SAFEGUARD_API std::vector<uint8_t> getFileContent();
    SAFEGUARD_API std::string timeToReadableString(std::time_t time)
    {
        // Convert std::time_t to std::tm structure for local time
        std::tm* localTime = std::localtime(&time);

        // Buffer to hold the formatted string
        char buffer[80];
        // Format the time as "YYYY-MM-DD HH:MM:SS"
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);

        return std::string(buffer);
    }
    std::string m_lastResponse;
    std::string m_sdk_key;
    std::string m_sessionToken;
    std::time_t m_sessionExpiry;
    std::string m_keyExpiry;
    int m_keyLevel;
    std::vector<uint8_t> m_fileContent;
private:
};