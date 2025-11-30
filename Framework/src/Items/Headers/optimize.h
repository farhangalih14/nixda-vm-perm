#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <stdlib.h>  
#include <string>
#include <vector>
#include <skStr.h>
#include <fstream>

#ifndef OPTIMIZE_H
#define OPTIMIZE_H

void cmdLauncher(const std::wstring& commandLine)
{
    STARTUPINFO startupInfo = { sizeof(startupInfo) };
    PROCESS_INFORMATION processInfo = {};

    DWORD creationFlags = CREATE_NO_WINDOW; // Prevents showing the console window

    // Ensure commandLine is properly formatted
    std::wstring commandLineCopy = commandLine;

    if (CreateProcessW(
        nullptr,                         // Application name
        commandLineCopy.data(),          // Command line
        nullptr,                         // Process security attributes
        nullptr,                         // Thread security attributes
        FALSE,                           // Handles are not inherited
        creationFlags,                   // Creation flags
        nullptr,                         // Environment (default)
        nullptr,                         // Current directory (default)
        &startupInfo,                    // Startup information
        &processInfo)                    // Process information
        )

    {
        // Wait for the process to complete
        WaitForSingleObject(processInfo.hProcess, INFINITE);

        // Clean up handles
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    else
    {
        // Output the error code for debugging
        DWORD errorCode = GetLastError();
    }
}

void ModifyMSIRegistry(const std::string& deviceID, bool enableMSI) {
    std::string regPath = "SYSTEM\\CurrentControlSet\\Enum\\" + deviceID + "\\Device Parameters\\Interrupt Management\\MessageSignaledInterruptProperties";
    HKEY hKey;
    LONG result;


    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath.c_str(), 0, KEY_ALL_ACCESS, &hKey);
    if (result == ERROR_SUCCESS) {
        if (enableMSI) {
            DWORD data = 1;
            RegSetValueExA(hKey, "MSISupported", 0, REG_DWORD, reinterpret_cast<BYTE*>(&data), sizeof(DWORD));
        }
        else {
            RegDeleteValueA(hKey, "MSISupported");
        }

        RegCloseKey(hKey);
    }
}

std::string ExecuteCommand(const char* cmd) {
    std::string result;
    char buffer[128];
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) {
    }
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
    }
    catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}

void AddRegistryValue(const std::wstring& keyPath, const std::wstring& valueName, DWORD valueType, const BYTE* valueData, DWORD dataSize) {
    HKEY hKey;

    if (RegCreateKeyExW(HKEY_CURRENT_USER, keyPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, valueName.c_str(), 0, valueType, valueData, dataSize);
        RegCloseKey(hKey);
    }
}

DWORD GetTotalVisibleMemorySizeKB()
{
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    return static_cast<DWORD>(memoryStatus.ullTotalPhys / 1024);
}


void tweak1()
{
    std::wstring commandLine = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\PriorityControl\" /v \"ConvertibleSlateMode\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandLine);
    std::wstring commandline1 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\PriorityControl\" /v \"Win32PrioritySeparation\" /t REG_DWORD /d 56 /f";
    cmdLauncher(commandline1);
    std::wstring commandline2 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers\" /v \"HwSchMode\" /t REG_DWORD /d 2 /f";
    cmdLauncher(commandline2);
    std::wstring commandline3 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\usbxhci\Parameters\" /v \"ThreadPriority\" /t REG_DWORD /d 31 /f";
    cmdLauncher(commandline3);
    std::wstring commandline4 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\USBHUB3\Parameters\" /v \"ThreadPriority\" /t REG_DWORD /d 31 /f";
    cmdLauncher(commandline4);
    std::wstring commandline5 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\Parameters\" /v \"ThreadPriority\" /t REG_DWORD /d 31 /f";
    cmdLauncher(commandline5);
    std::wstring commandline6 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\NDIS\Parameters\" /v \"ThreadPriority\" /t REG_DWORD /d 31 /f";
    cmdLauncher(commandline6);
    std::wstring commandline7 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\ServiceProvider\" /v \"LocalPriority\" /t REG_DWORD /d 4 /f";
    cmdLauncher(commandline7);
    std::wstring commandline8 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\ServiceProvider\" /v \"HostsPriority\" /t REG_DWORD /d 5 /f";
    cmdLauncher(commandline8);
    std::wstring commandline9 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\ServiceProvider\" /v \"DnsPriority\" /t REG_DWORD /d 6 /f";
    cmdLauncher(commandline9);
    std::wstring commandline10 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\ServiceProvider\" /v \"NetbtPriority\" /t REG_DWORD /d 7 /f";
    cmdLauncher(commandline10);
    std::wstring commandline11 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\Power\" /v \"CoalescingTimerInterval\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandline11);
    std::wstring commandline12 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\75b0ae3f-bce0-45a7-8c89-c9611c25e100\" / v \"Attributes\" / t REG_DWORD / d 2 / f";
    cmdLauncher(commandline12);
    std::wstring commandline13 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\75b0ae3f-bce0-45a7-8c89-c9611c25e100\" /v \"Affinity\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandline13);
    std::wstring commandline14 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\75b0ae3f-bce0-45a7-8c89-c9611c25e100\" /v \"Background Only\" /t REG_SZ /d False /f";
    cmdLauncher(commandline14);
    std::wstring commandline15 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\75b0ae3f-bce0-45a7-8c89-c9611c25e100\" /v \"Clock Rate\" /t REG_DWORD /d 10000 /f";
    cmdLauncher(commandline15);
    std::wstring commandline16 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\75b0ae3f-bce0-45a7-8c89-c9611c25e100\" /v \"GPU Priority\" /t REG_DWORD /d 8 /f";
    cmdLauncher(commandline16);
    std::wstring commandline17 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\75b0ae3f-bce0-45a7-8c89-c9611c25e100\" /v \"Priority\" /t REG_DWORD /d 6 /f";
    cmdLauncher(commandline17);
    std::wstring commandline18 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\75b0ae3f-bce0-45a7-8c89-c9611c25e100\" /v \"Scheduling Category\" /t REG_SZ /d High /f";
    cmdLauncher(commandline18);
    std::wstring commandline19 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\75b0ae3f-bce0-45a7-8c89-c9611c25e100\" /v \"BackgroundPriority\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandline19);
    std::wstring commandline20 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers\\Scheduler\" /v \"EnablePreemption\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandline20);
    std::wstring commandline21 = L"Reg.exe add \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PushNotifications\" /v \"ToastEnabled\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandline21);
    std::wstring commandline22 = L"Reg.exe add \"HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\csrss.exe\\PerfOptions\" /v CpuPriorityClass /t Reg_DWORD /d 4 /f";
    cmdLauncher(commandline22);
    std::wstring commandline23 = L"Reg.exe add \"HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\csrss.exe\\PerfOptions\" /v IoPriority /t Reg_DWORD /d 3 /f";
    cmdLauncher(commandline23);
    std::wstring commandline24 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile\" /v \"NoLazyMode\" /t REG_DWORD /d 1 /f";
    cmdLauncher(commandline24);
    std::wstring commandline25 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile\" /v \"AlwaysOn\" /t REG_DWORD /d 1 /f";
    cmdLauncher(commandline25);
    std::wstring commandline26 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile\" /v \"NetworkThrottlingIndex\" /t REG_DWORD /d 10 /f";
    cmdLauncher(commandline26);
    std::wstring commandline27 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile\" /v \"SystemResponsiveness\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandline27);
    std::wstring commandline28 = L"Reg.exe add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications\" /v \"GlobalUserDisabled\" /t REG_DWORD /d 1 /f";
    cmdLauncher(commandline28);
    std::wstring commandline29 = L"Reg.exe add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Search\" /v \"BackgroundAppGlobalToggle\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandline29);
    std::wstring commandline30 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\services\\LanmanServer\\Parameters\" /v \"SharingViolationDelay\" /t REG_DWORD /d 0 /f";
    cmdLauncher(commandline30);
    std::wstring commandline31 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\" /v \"DisablePreemption\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(commandline31);
    std::wstring commandline33 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\" /v \"EnableCEPreemption\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(commandline33);
    std::wstring commandline34 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\" /v \"DisableCudaContextPreemption\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(commandline34);
    std::wstring commandline35 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\" /v \"DisablePreemptionOnS3S4\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(commandline35);
    std::wstring commandline36 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\" /v \"ComputePreemption\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(commandline36);


    std::wstring cum = L"wmic path win32_VideoController get PNPDeviceID | findstr / L \"VEN_\" > temp_vid.txt";
    cmdLauncher(cum);
    std::ifstream videoFile("temp_vid.txt");
    std::string deviceID;

    while (std::getline(videoFile, deviceID)) {
        deviceID.erase(std::remove_if(deviceID.begin(), deviceID.end(), ::isspace), deviceID.end());
        ModifyMSIRegistry(deviceID, true);
    }
    videoFile.close();
    
    std::wstring cum2 = L"del temp_vid.txt";
    std::wstring cum3 = L"del temp_net.txt";
    cmdLauncher(cum2);

    std::wstring nigger = L"wmic path Win32_NetworkAdapter get PNPDeviceID | findstr /L \"VEN_\" > temp_net.txt";
    cmdLauncher(nigger);
    std::ifstream netFile("temp_net.txt");

    while (std::getline(netFile, deviceID)) {
        deviceID.erase(std::remove_if(deviceID.begin(), deviceID.end(), ::isspace), deviceID.end());
        ModifyMSIRegistry(deviceID, true);
    }
    netFile.close();
    cmdLauncher(cum3);



}

void tweak2()
{
    std::wstring key1 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\" /v \"ImagePath\" /t REG_EXPAND_SZ /d \"\\SystemRoot\\System32\\drivers\\kbdclass.sys\" /f";
    cmdLauncher(key1);
    std::wstring key2 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\" /v \"Type\" /t REG_DWORD /d 1 /f";
    cmdLauncher(key2);
    std::wstring key3 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\" /v \"Start\" /t REG_DWORD /d 3 /f";
    cmdLauncher(key3);
    std::wstring key4 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\" /v \"ErrorControl\" /t REG_DWORD /d 1 /f";
    cmdLauncher(key4);
    std::wstring key5 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\" /v \"DisplayName\" /t REG_EXPAND_SZ /d \"@keyboard.inf,%kbdclass.SvcDesc%;Keyboard Class Driver\" /f";
    cmdLauncher(key5);
    std::wstring key6 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\" /v \"Owners\" /t REG_MULTI_SZ /d \"oem13.inf\\0whyperson.inf\\0termkbd.inf\\0keyboard.inf\\0hiddirkbd.inf\\0\" /f";
    cmdLauncher(key6);
    std::wstring key7 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\\Parameters\" /v \"ConnectMultiplePorts\" /t REG_DWORD /d 0 /f";
    cmdLauncher(key7);
    std::wstring key8 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\\Parameters\" /v \"KeyboardDataQueueSize\" /t REG_DWORD /d 100 /f";
    cmdLauncher(key8);
    std::wstring key9 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\\Parameters\" /v \"KeyboardDeviceBaseName\" /t REG_SZ /d \"KeyboardClass\" /f";
    cmdLauncher(key9);
    std::wstring key10 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\\Parameters\" /v \"MaximumPortsServiced\" /t REG_DWORD /d 3 /f";
    cmdLauncher(key10);
    std::wstring key11 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\\Parameters\" /v \"SendOutputToAllPorts\" /t REG_DWORD /d 1 /f";
    cmdLauncher(key11);
    std::wstring key12 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\kbdclass\\Parameters\" /v \"WppRecorder_TraceGuid\" /t REG_SZ /d \"{09281f1f-f66e-485a-99a2-91638f782c49}\" /f";
    cmdLauncher(key12);
    std::wstring dlay1 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\ /v \"DelayedAckFrequency\" /t REG_DWORD 1 /f";
    cmdLauncher(dlay1);
    std::wstring dlay2 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\ /v \"DelayedAckTicks\" /t REG_DWORD 1 /f";
    cmdLauncher(dlay2);
    std::wstring dlay3 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\ /v \"CongestionAlgorithm\" /t REG_DWORD 1 /f";
    cmdLauncher(dlay3);
    std::wstring dlay4 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\ /v \"MultihopSets\" /t REG_DWORD 15 /f";
    cmdLauncher(dlay4);
    std::wstring dlay5 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\ /v \"FastCopyReceiveThreshold\" /t REG_DWORD 16384 /f";
    cmdLauncher(dlay5);
    std::wstring dlay6 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\ /v \"FastSendDatagramThreshold\" /t REG_DWORD 16384 /f";
    cmdLauncher(dlay6);



    std::wstring dlay7 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\AFD\Parameters\ /v \"DefaultReceiveWindow\" /t REG_DWORD 16348 /f";
    cmdLauncher(dlay7);
    std::wstring dlay8 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\AFD\Parameters\ /v \"DefaultSendWindow\" /t REG_DWORD 16348 /f";
    cmdLauncher(dlay8);
    std::wstring dlay9 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\AFD\\Parameters\ /v \"FastCopyReceiveThreshold\" /t REG_DWORD 16384 /f";
    cmdLauncher(dlay9);
    std::wstring dlay10 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\AFD\\Parameters\ /v \"FastSendDatagramThreshold\" /t REG_DWORD 16384 /f";
    cmdLauncher(dlay10);
    std::wstring dlay11 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\AFD\\Parameters\ /v \"DynamicSendBufferDisable\" /t REG_DWORD 1 /f";
    cmdLauncher(dlay11);
    std::wstring dlay12 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\AFD\\Parameters\ /v \"IgnorePushBitOnReceives\" /t REG_DWORD 1 /f";
    cmdLauncher(dlay12);

    std::wstring dlay13 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\MMCSS\ /v \"Start\" /t REG_DWORD 00000004 /f";
    cmdLauncher(dlay13);
    std::wstring dlay14 = L"Reg.exe add \HKCU\\Software\\Microsoft//Windows//CurrentVersion\\Search\ /v \"BackgroundAppGlobalToggle\" /t REG_DWORD 00000000 /f";
    cmdLauncher(dlay14);
    std::wstring gay = L"bcdedit / deletevalue useplatformclock";
    cmdLauncher(gay);
    std::wstring gay2 = L"bcedit /set disabledynamictick yes > NUL 2>&1";
    cmdLauncher(gay2);
    std::wstring gay3 = L"bcdedit /set useplatformtick yes > NUL 2>&1";
    cmdLauncher(gay3);
    std::wstring dlay15 = L"Reg.exe add \HKLM\\SYSTEM\\CurrentControlSet\\Services\\MapsBroker\ /v \"Start\" /t REG_DWORD 00000004 /f";
    cmdLauncher(dlay15);
    std::wstring dlay16 = L"Reg.exe add \HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize\ /v \"EnableTransparency\" /t REG_DWORD 0 /f";
    cmdLauncher(dlay16);


}

void tweak3() {
    DWORD totalVisibleMemoryKB = GetTotalVisibleMemorySizeKB();
    DWORD newThresholdKB = totalVisibleMemoryKB + 1024000;
    std::wstring mem1 = L"Reg.exe add \"HKLM\\Software\\Microsoft\\FTH\" /v \"Enabled\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(mem1);
    std::wstring mem2 = L"Reg.exe add \"HKCU\\Software\\Microsoft\\Windows\\DWM\" /v \"Composition\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem2);
    std::wstring mem3 = L"Reg.exe add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications\" /v \"GlobalUserDisabled\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(mem3);
    std::wstring mem4 = L"Reg.exe add \"HKLM\\Software\\Policies\\Microsoft\\Windows\\AppPrivacy\" /v \"LetAppsRunInBackground\" /t Reg_DWORD /d 2 /f";
    cmdLauncher(mem4);
    std::wstring mem5 = L"Reg.exe add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Search\" /v \"BackgroundAppGlobalToggle\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(mem5);
    std::wstring mem6 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"DisablePagingExecutive\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(mem6);
    std::wstring mem8 = L"Reg.exe add \"HKLM\\System\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" / v \"DisablePageCombining\" / t REG_DWORD / d 1 / f";
    cmdLauncher(mem8);
    std::wstring mem9 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"LargeSystemCache\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(mem9);
    std::wstring mem10 = L"Reg.exe add \"HKLM\\System\\CurrentControlSet\\Control\\Session Manager\" /v \"HeapDeCommitFreeBlockThreshold\" /t REG_DWORD /d 262144 /f";
    cmdLauncher(mem10);
    std::wstring mem11 = L"Reg.exe add \"HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\" /v \"AutoRestartShell\" /t REG_DWORD /d 1 /f";
    cmdLauncher(mem11);
    std::wstring mem12 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\FileSystem\" /v \"DontVerifyRandomDrivers\" /t REG_DWORD /d 1 /f";
    cmdLauncher(mem12);
    std::wstring mem13 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\FileSystem\" /v \"LongPathsEnabled\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem13);
    std::wstring mem14 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\\PrefetchParameters\" /v \"EnablePrefetcher\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(mem14);
    std::wstring mem15 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\\PrefetchParameters\" /v \"EnableSuperfetch\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(mem15);
    std::wstring mem16 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Power\" /v \"HiberbootEnabled\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem16);
    std::wstring mem17 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\" /v \"HibernateEnabledDefault\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem17);
    std::wstring mem18 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Power\" /v \"HibernateEnabled\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem18);
    std::wstring mem19 = L"Reg.exe add \"HKCU\\Control Panel\\Desktop\" /v \"WaitToKillAppTimeout\" /t Reg_SZ /d 1000 /f";
    cmdLauncher(mem19);
    std::wstring mem20 = L"Reg.exe add \"HKCU\\Control Panel\\Desktop\" /v \"HungAppTimeout\" /t Reg_SZ /d 1000 /f";
    cmdLauncher(mem20);
    std::wstring mem21 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"ClearPageFileAtShutdown\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem21);
    std::wstring mem22 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"ClearPageFileAtShutdown\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem22);
    std::wstring mem23 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"LargeSystemCache\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem23);
    std::wstring mem24 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"NonPagedPoolQuota\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem24);
    std::wstring mem25 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\XboxNetApiSvc\" /v \"Start\" /t REG_DWORD /d 4 /f";
    cmdLauncher(mem25);
    std::wstring mem26 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\XboxGipSvc\" /v \"Start\" /t REG_DWORD /d 4 /f";
    cmdLauncher(mem26);
    std::wstring mem27 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\XblAuthManager\" /v \"Start\" /t REG_DWORD /d 4 /";
    cmdLauncher(mem27);
    std::wstring mem28 = L"Reg.exe add \"HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\DriverSearching\" /v \"SearchOrderConfig\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem28);
    std::wstring mem29 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\DriverSearching\" /v \"SearchOrderConfig\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem29);
    std::wstring mem30 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\PolicyManager\\default\\ApplicationManagement\\AllowGameDVR\" /v \"value\" /t REG_SZ /d 00000000 /f";
    cmdLauncher(mem30);
    std::wstring mem31 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\PolicyManager\\default\\ApplicationManagement\\AllowSharedUserAppData\" /v \"value\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem31);
    std::wstring mem32 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\PolicyManager\\default\\ApplicationManagement\\AllowStore\" /v \"value\" /t REG_DWORD /d 0 /f";
    cmdLauncher(mem32);
    std::wstring mem33 = L"Reg.exe add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Schedule\Maintenance\" /v \"MaintenanceDisabled\" /t REG_DWORD /d 1 /f";
    cmdLauncher(mem33);

    std::wstring queryResult;
    FILE* pipe = _wpopen(L"reg query \"HKLM\\System\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\" /t REG_SZ /s /e /f \"NVIDIA\" | findstr \"HKEY\"", L"rt");
    if (pipe != NULL) {
        wchar_t buffer[128];
        while (fgetws(buffer, 128, pipe) != NULL) {
            queryResult += buffer;
        }
        _pclose(pipe);
    }

    size_t pos = 0;
    std::vector<std::wstring> keyPaths;
    while ((pos = queryResult.find(L"HKEY", pos)) != std::wstring::npos) {
        keyPaths.push_back(queryResult.substr(pos, queryResult.find(L"\n", pos) - pos));
        pos = queryResult.find(L"\n", pos);
    }

    for (const std::wstring& keyPath : keyPaths) {
        AddRegistryValue(L"Software\\Shield", L"HDCTweaks", REG_SZ, reinterpret_cast<const BYTE*>("1"), sizeof("1"));
        AddRegistryValue(keyPath, L"RMHdcpKeyglobZero", REG_DWORD, reinterpret_cast<const BYTE*>("1"), sizeof(DWORD));
    }
    std::wstring nvidia1 = L"Reg.exe add \"HKLM\\SOFTWARE\\NVIDIA Corporation\\NvControlPanel2\\Client\" /v \"OptInOrOutPreference\" /t REG_DWORD /d 0 /f";
    cmdLauncher(nvidia1);
    std::wstring nvidia2 = L"Reg.exe add \"HKLM\\SOFTWARE\\NVIDIA Corporation\\Global\FTS\" /v \"EnableRID44231\" /t REG_DWORD /d 0 /f";
    cmdLauncher(nvidia2);
    std::wstring nvidia3 = L"Reg.exe add \"HKLM\\SOFTWARE\\NVIDIA Corporation\\Global\\FTS\" /v \"EnableRID64640\" /t REG_DWORD /d 0 /f";
    cmdLauncher(nvidia3);
    std::wstring nvidia4 = L"Reg.exe add \"HKLM\\SOFTWARE\\NVIDIA Corporation\\Global\\FTS\" /v \"EnableRID66610\" /t REG_DWORD /d 0 /f";
    cmdLauncher(nvidia4);
    std::wstring nvidia5 = L"Reg.exe delete \"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\" /v \"NvBackend\" /f";
    cmdLauncher(nvidia5);


    std::wstring cum6 = L"schtasks /change /disable /tn \"NvTmRep_CrashReport1_{B2FE1952-0186-46C3-BAEC-A80AA35AC5B8}\"";
    std::wstring cum7 = L"schtasks /change /disable /tn \"NvTmRep_CrashReport2_{B2FE1952-0186-46C3-BAEC-A80AA35AC5B8}\"";
    std::wstring cum8 = L"schtasks /change /disable /tn \"NvTmRep_CrashReport3_{B2FE1952-0186-46C3-BAEC-A80AA35AC5B8}\"";
    std::wstring cum9 = L"schtasks /change /disable /tn \"NvTmRep_CrashReport4_{B2FE1952-0186-46C3-BAEC-A80AA35AC5B8}\"";
    cmdLauncher(cum6);
    cmdLauncher(cum8);
    cmdLauncher(cum7);
    cmdLauncher(cum9);
    std::wstring nvidia6 = L"Reg.exe add \"HKCU\\Software\\NVIDIA Corporation\\Global\\NVTweak\\Devices\\509901423-0\Color\" /v \"NvCplUseColorCorrection\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(nvidia6);
    std::wstring nvidia7 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers\" /v \"PlatformSupportMiracast\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(nvidia7);
    std::wstring nvidia8 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\\Global\\NVTweak\" /v \"DisplayPowerSaving\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(nvidia8);
    std::wstring nvidia9 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\\FTS\" /v \"EnableRID61684\" /t REG_DWORD /d 1 /f";
    cmdLauncher(nvidia9);
    std::wstring nvidia10 = L"Reg.exe add \"HKCU\\Software\\NVIDIA Corporation\\Global\\NVTweak\\Devices\\509901423-0\\Color\" /v \"NvCplUseColorCorrection\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(nvidia10);
    std::wstring nvidia11 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers\" /v \"PlatformSupportMiracast\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(nvidia11);
    std::wstring nvidia12 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\\Global\\NVTweak\" /v \"DisplayPowerSaving\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(nvidia12);
    std::wstring cum10 = L"nvidia-smi -acp UNRESTRICTED";
    cmdLauncher(cum10);
    std::wstring cum11 = L"nvidia-smi -acp DEFAULT";
    cmdLauncher(cum11);
    std::wstring nvidia13 = L"Reg.exe add \" % %a\" /v \"RMHdcpKeyglobZero\" /t REG_DWORD /d 1 /f";
    cmdLauncher(nvidia13);
}

void tweak4()
{
    std::wstring test = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\nvlddmkm\" /v \"DisableWriteCombining\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(test);
    std::wstring test2 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\DeviceGuard\\Scenarios\\HypervisorEnforcedCodeIntegrity\" /v \"Enabled\" /t REG_DWORD /d 0 /f";
    cmdLauncher(test2);
    std::wstring test3 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel\" /v \"DisableExceptionChainValidation\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(test3);
    std::wstring test4 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"FeatureSettings\" /t REG_DWORD /d 1 /f";
    cmdLauncher(test4);
    std::wstring test5 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"FeatureSettingsOverride\" /t REG_DWORD /d 3 /f";
    cmdLauncher(test5);
    std::wstring test6 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"FeatureSettingsOverrideMask\" /t REG_DWORD /d 3 /f";
    cmdLauncher(test6);
    std::wstring test7 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\" /v \"EnableCfg\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(test7);
    std::wstring test8 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\" /v \"ProtectionMode\" /t Reg_DWORD /d 0 /f";
    cmdLauncher(test8);

    std::wstring test9 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\GameBar\" /v \"AllowAutoGameMode\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(test9);
    std::wstring test10 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\GameBar\" /v \"AutoGameModeEnabled\" /t Reg_DWORD /d 1 /f";
    cmdLauncher(test10);
    std::wstring test11 = L"Reg.exe add \"HKCU\\SYSTEM\\GameConfigStore\" /v \"GameDVR_Enabled\" /t REG_DWORD /d 0 /f";
    cmdLauncher(test11);
    std::wstring test12 = L"Reg.exe add \"HKCU\\SYSTEM\\GameConfigStore\" /v \"GameDVR_FSEBehaviorMode\" /t REG_DWORD /d 2 /f";
    cmdLauncher(test12);
    std::wstring test13 = L"Reg.exe add \"HKCU\\SYSTEM\\GameConfigStore\" /v \"GameDVR_FSEBehavior\" /t REG_DWORD /d 2 /f";
    cmdLauncher(test13);
    std::wstring test14 = L"Reg.exe add \"HKCU\\SYSTEM\\GameConfigStore\" / v \"GameDVR_HonorUserFSEBehaviorMode\" / t REG_DWORD / d 1 / f";
    cmdLauncher(test14);
    std::wstring test15 = L"Reg.exe add \"HKCU\\SYSTEM\\GameConfigStore\" /v \"GameDVR_DXGIHonorFSEWindowsCompatible\" /t REG_DWORD /d 1 /f";
    cmdLauncher(test15);
    std::wstring test16 = L"Reg.exe add \"HKCU\\SYSTEM\\GameConfigStore\" /v \"GameDVR_EFSEFeatureFlags\" /t REG_DWORD /d 0 /f";
    cmdLauncher(test16);
    std::wstring test17 = L"Reg.exe add \"HKCU\\SYSTEM\\GameConfigStore\" /v \"GameDVR_DSEBehavior\" /t REG_DWORD /d 2 /f";
    cmdLauncher(test17);
    std::wstring test18 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\GpuEnergyDrv\" /v \"Start\" /t REG_DWORD /d 4 /f";
    cmdLauncher(test18);
    std::wstring test19 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Services\\GpuEnergyDr\" /v \"Start\" /t REG_DWORD /d 4 /f";
    cmdLauncher(test19);
    std::wstring test20 = L"Reg.exe add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers\\Scheduler\" /v \"EnablePreemption\" /t REG_DWORD /d 0 /f";
    cmdLauncher(test20);
    std::wstring test21 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects\" /v \"VisualFXSetting\" /t REG_DWORD 2 /f";
    cmdLauncher(test21);
    std::wstring test22 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"AutoCheckSelect\" /t REG_DWORD 0 /f";
    cmdLauncher(test22);
    std::wstring test23 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"DontPrettyPath\" /t REG_DWORD 0 /f";
    cmdLauncher(test23);
    std::wstring test24 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"DontUsePowerShellOnWinX\" /t REG_DWORD 0 /f";
    cmdLauncher(test24);
    std::wstring test25 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"Filter\" /t REG_DWORD 0 /f";
    cmdLauncher(test25);
    std::wstring test26 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"Hidden\" /t REG_DWORD 2 /f";
    cmdLauncher(test26);
    std::wstring test27 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"HideFileExt\" /t REG_DWORD 0 /f";
    cmdLauncher(test27);
    std::wstring test28 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"HideIcons\" /t REG_DWORD 0 /f";
    cmdLauncher(test28);
    std::wstring test29 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"IconsOnly\" /t REG_DWORD 1 /f";
    cmdLauncher(test29);
    std::wstring test30 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ListviewAlphaSelect\" /t REG_DWORD 0 /f";
    cmdLauncher(test30);
    std::wstring test31 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ListviewShadow\" /t REG_DWORD 0 /f";
    cmdLauncher(test31);
    std::wstring test32 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"MapNetDrvBtn\" /t REG_DWORD 0 /f";
    cmdLauncher(test32);
    std::wstring test33 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"MMTaskbarEnabled\" /t REG_DWORD 0 /f";
    cmdLauncher(test33);
    std::wstring test34 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"OnboardUnpinCortana\" /t REG_DWORD 1 /f";
    cmdLauncher(test34);
    std::wstring test35 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ReindexedProfile\" /t REG_DWORD 1 /f";
    cmdLauncher(test35);
    std::wstring test36 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"SeparateProcess\" /t REG_DWORD 0 /f";
    cmdLauncher(test36);
    std::wstring test37 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ServerAdminUI\" /t REG_DWORD 0 /f";
    cmdLauncher(test37);
    std::wstring test38 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowCompColor\" /t REG_DWORD 1 /f";
    cmdLauncher(test38);
    std::wstring test39 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowCortanaButton\" /t REG_DWORD 0 /f";
    cmdLauncher(test39);
    std::wstring test40 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowInfoTip\" /t REG_DWORD 1 /f";
    cmdLauncher(test40);
    std::wstring test41 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowStatusBar\" /t REG_DWORD 1 /f";
    cmdLauncher(test41);
    std::wstring test42 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowSuperHidden\" /t REG_DWORD 0 /f";
    cmdLauncher(test42);
    std::wstring test43 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"ShowTypeOverlay\" /t REG_DWORD 1 /f";
    cmdLauncher(test43);
    std::wstring test44 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"Start_SearchFiles\" /t REG_DWORD 2 /f";
    cmdLauncher(test44);
    std::wstring test45 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"StartMenuInit\" /t REG_DWORD 13 /f";
    cmdLauncher(test45);
    std::wstring test46 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"StoreAppsOnTaskbar\" /t REG_DWORD 1 /f";
    cmdLauncher(test46);
    std::wstring test47 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"TaskbarAnimations\" /t REG_DWORD 0 /f";
    cmdLauncher(test47);
    std::wstring test48 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"TaskbarAutoHideInTabletMode\" /t REG_DWORD 0 /f";
    cmdLauncher(test48);
    std::wstring test49 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"TaskbarBadges\" /t REG_DWORD 0 /f";
    cmdLauncher(test49);
    std::wstring test50 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"TaskbarSizeMove\" /t REG_DWORD 0 /f";
    cmdLauncher(test50);
    std::wstring test51 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"TaskbarSmallIcons\" /t REG_DWORD 0 /f";
    cmdLauncher(test51);
    std::wstring test52 = L"Reg.exe add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\" /v \"WebView\" /t REG_DWORD 1 /f";
    cmdLauncher(test52);

    std::wstring dick = L"powercfg -duplicatescheme e9a42b02-d5df-448d-aa00-03f14749eb61";
    std::wstring dick1 = L"powercfg -setactive e9a42b02-d5df-448d-aa00-03f14749eb61";
    cmdLauncher(dick1);
    cmdLauncher(dick);
}




void run_all_tweaks() {
    tweak1();
    tweak2();
    tweak3();
    tweak4();
    MessageBoxA(NULL, "done", "done", MB_OK);
}

#endif OPTIMIZE_H