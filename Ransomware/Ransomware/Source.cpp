#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <Wincrypt.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <set>
#include "md5.h"


// Global variables
#define SCAN 1
#define SCANALL 2
#define OPENFILE 3
#define PATH 4
#define SCANTEXT 5
#define STATUS 6
#define SCANNING 7

HWND hwnd;

// The main window class name.
static TCHAR szWindowClass[] = _T("AntiVirus");


// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("TuAntiVirus");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


OPENFILENAME ofn;
DWORD error_value;


bool openFileDialog(TCHAR szFileName[])
{
	const TCHAR* FilterSpec = "All Files(.)\0*.*\0";
	const TCHAR* Title = "Open";

	const TCHAR* myDir = "C:\\c_plus_plus_trial";

	TCHAR szFileTitle[MAX_PATH] = { '\0' };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	*szFileName = 0;

	/* fill in non-variant fields of OPENFILENAME struct. */
	ofn.lStructSize = sizeof(OPENFILENAME);

	ofn.hwndOwner = GetFocus();
	ofn.lpstrFilter = FilterSpec;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = myDir;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	// false if failed or cancelled
	if (GetOpenFileName(&ofn) == 1)
	{

		MessageBox(NULL, "Opening filepath", "SUCCESS !!!", MB_OK);

		return 1;
	}
	else
	{

		MessageBox(NULL, "error", "FAILURE !!!", MB_OK);
		return 0;
	}
}



int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"),
			_T("TuAntiVirus"), NULL);

		return 1;
	}
	// Store instance handle in our global variable
	hInst = hInstance;

	// The parameters to CreateWindowEx explained:
	// WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		szWindowClass,
		szTitle,
		WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd)
	{
		MessageBox(NULL,
			_T("Call to Createwindow failed"),
			_T("TuAntivirus"), NULL);
		return 1;
	}
	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;

}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR logo[] = _T("Tu AntiVirus");
	TCHAR path[] = _T("Path:");
	TCHAR md5value[] = _T("MD5:");
	TCHAR status[] = _T("Status:");
	TCHAR scanning[] = _T("Scanning:");
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// Here your application is laid out.
		// For this introduction, we just print out "Tu AntiVirus!"
		// in the bottom left corner.
		TextOut(hdc, 20, 500, logo, _tcslen(logo));
		TextOut(hdc, 20, 100, path, _tcslen(path));
		TextOut(hdc, 20, 120, md5value, _tcslen(md5value));
		TextOut(hdc, 20, 140, status, _tcslen(status));
		TextOut(hdc, 20, 160, scanning, _tcslen(scanning));
		// End application-specific layout section.
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		CreateWindow(TEXT("Button"), TEXT("Scan"),
			WS_CHILD | WS_VISIBLE,
			20, 20, 100, 80,
			hWnd, (HMENU)SCAN, NULL, NULL);
		CreateWindow(TEXT("Button"), TEXT("ScanAll"),
			WS_CHILD | WS_VISIBLE,
			120, 20, 100, 80,
			hWnd, (HMENU)SCANALL, NULL, NULL);
		CreateWindow(TEXT("Button"), TEXT("Open Result"),
			WS_CHILD | WS_VISIBLE,
			220, 20, 100, 80,
			hWnd, (HMENU)OPENFILE, NULL, NULL);
		CreateWindow("Static", "...",
			WS_VISIBLE | WS_CHILD,
			65, 100, 700, 20,
			hWnd, (HMENU)PATH, NULL, NULL);
		CreateWindow("Static", "...",
			WS_VISIBLE | WS_CHILD,
			65, 120, 700, 20,
			hWnd, (HMENU)SCANTEXT, NULL, NULL);
		CreateWindow("Static", "N/A",
			WS_VISIBLE | WS_CHILD,
			65, 140, 700, 20,
			hWnd, (HMENU)STATUS, NULL, NULL);
		CreateWindow("Static", "...",
			WS_VISIBLE | WS_CHILD,
			85, 160, 600, 20,
			hWnd, (HMENU)SCANNING, NULL, NULL);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == SCAN)
		{
			std::string infected = "infected";
			std::string clean = "clean";
			TCHAR openFileName[MAX_PATH];
			openFileDialog(openFileName);

			//path to hashcodes.txt to compare if the file is infected or not with malware for more md5 hashes visit virusshare.com
			std::ifstream hash("D:\\Visual Studio\\C++\\TuAntivirus\\TuAntivirus\\hashcodes.txt");
			std::string line;
			std::string scan = "Scanning";
			std::string done = "Done";
			std::ofstream file;
			std::string result = "result.txt";
			bool found = false;
			while (std::getline(hash, line) && !found)
			{
				if (line.find(md5(openFileName)) != std::string::npos)
				{
					found = true;
				}
			}
			if (found)
			{
				file.open(result.c_str(), std::ios_base::app);
				file << openFileName << " " << md5(openFileName) << " Infected " << "\n";
				file.close();
				SetWindowText(GetDlgItem(hwnd, PATH), openFileName);
				SetWindowText(GetDlgItem(hwnd, SCANTEXT), md5(openFileName).c_str());
				SetWindowText(GetDlgItem(hwnd, STATUS), infected.c_str());
				SetWindowText(GetDlgItem(hwnd, SCANNING), scan.c_str());
			}
			else
			{
				file.open(result.c_str(), std::ios_base::app);
				file << openFileName << " " << md5(openFileName) << " Clean " << "\n";
				file.close();
				SetWindowText(GetDlgItem(hwnd, PATH), openFileName);
				SetWindowText(GetDlgItem(hwnd, SCANTEXT), md5(openFileName).c_str());
				SetWindowText(GetDlgItem(hwnd, STATUS), clean.c_str());
				SetWindowText(GetDlgItem(hwnd, SCANNING), scan.c_str());
			}
			SetWindowText(GetDlgItem(hwnd, SCANNING), done.c_str());
		}
		else if (LOWORD(wParam) == SCANALL)
		{
			std::string scan = "Scanning";
			std::string done = "Done";
			std::string infected = "infected";
			std::string clean = "clean";
			std::ofstream file;
			std::set<std::string> ignoredirs = { "system32","Program Files","Program Files (x86)","$Recycle.Bin","Windows","AppData","ProgramData" };
			std::string path = "C:\\Users";
			for (std::filesystem::recursive_directory_iterator entry(path), end; entry != end; ++entry)
			{
				if (ignoredirs.find(entry->path().filename().string()) != ignoredirs.end())
				{
					entry.disable_recursion_pending();
				}
				else if (!std::filesystem::is_directory(entry->path()) && entry->path().has_extension())
				{
					std::ifstream hash("D:\\Visual Studio\\C++\\TuAntivirus\\TuAntivirus\\hashcodes.txt");
					std::string result = "result.txt";
					std::string line;
					bool found = false;
					while (std::getline(hash, line) && !found)
					{
						if (line.find(md5(entry->path().string())) != std::string::npos)
						{
							found = true;
						}
					}
					if (found)
					{
						file.open(result.c_str(), std::ios_base::app);
						file << entry->path().string().c_str() << " " << md5(entry->path().string()) << " Infected " << "\n";
						file.close();
						SetWindowText(GetDlgItem(hwnd, PATH), entry->path().string().c_str());
						SetWindowText(GetDlgItem(hwnd, SCANTEXT), md5(entry->path().string()).c_str());
						SetWindowText(GetDlgItem(hwnd, STATUS), infected.c_str());
						SetWindowText(GetDlgItem(hwnd, SCANNING), scan.c_str());
					}
					else
					{
						file.open(result.c_str(), std::ios_base::app);
						file << entry->path().string().c_str() << " " << md5(entry->path().string()) << " Clean " << "\n";
						file.close();
						SetWindowText(GetDlgItem(hwnd, PATH), entry->path().string().c_str());
						SetWindowText(GetDlgItem(hwnd, SCANTEXT), md5(entry->path().string()).c_str());
						SetWindowText(GetDlgItem(hwnd, STATUS), clean.c_str());
						SetWindowText(GetDlgItem(hwnd, SCANNING), scan.c_str());
					}
				}
				

			}

			SetWindowText(GetDlgItem(hwnd, SCANNING), done.c_str());
		}
		else if (LOWORD(wParam) == OPENFILE)
		{
			
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
