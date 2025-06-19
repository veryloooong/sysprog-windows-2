// sysprog-windows-2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>

#include <cstdio>
#include <tchar.h>
#include <strsafe.h>

void print_err(TCHAR const* where) {
  ::_tprintf(TEXT("%s failed with error code %d"), where, GetLastError());
}

int main(int argc, char** argv) {
  TCHAR program_name[MAX_PATH];
  ::GetModuleFileName(NULL, program_name, MAX_PATH);
  if ((argc != 1 && argc != 3)
      || (argc == 3 && strncmp(argv[1], "/s", 3) != 0)) {
    ::_tprintf(TEXT("Usage: %s [/s DIR]"), program_name);
  }

  TCHAR path[MAX_PATH];
  size_t len;
  if (argc == 1) {
    if (::GetCurrentDirectory(MAX_PATH, path) == 0) {
      print_err(TEXT("GetCurrentDirectory"));
      return 1;
    }
    if (FAILED(::StringCchLength(path, MAX_PATH, &len))) {
      print_err(TEXT("StringCchLength"));
      return 1;
    }
    if (len > MAX_PATH - 3) {
      ::_tprintf(TEXT("Directory too long"));
      return 1;
    }
  }
  else if (argc == 3) {
    int bufsize = ::MultiByteToWideChar(CP_UTF8, 0, argv[2], -1, NULL, 0);  
    if (bufsize == 0) {  
      print_err(TEXT("MultiByteToWideChar"));  
      return 1;  
    }  

    if (bufsize > MAX_PATH - 3) {  
      ::_tprintf(TEXT("Input string too long"));  
      return 1;  
    }  

    if (::MultiByteToWideChar(CP_UTF8, 0, argv[2], -1, path, MAX_PATH) == 0) {  
      print_err(TEXT("MultiByteToWideChar"));  
      return 1;  
    }  
    
    if (FAILED(::StringCchLength(path, MAX_PATH, &len))) {
      print_err(TEXT("StringCchLength"));
      return 1;
    }
  }

  ::_tprintf(TEXT("Directory:%s\n\n"), path);
  
  if (FAILED(::StringCchCat(path, MAX_PATH, TEXT("\\*")))) {
    print_err(TEXT("StringCchCat"));
    return 1;
  }
  
  WIN32_FIND_DATA data;
  HANDLE find = ::FindFirstFile(path, &data);

  if (find == INVALID_HANDLE_VALUE) {
    print_err(TEXT("FindFirstFile"));
    return 1;
  }

  ::_tprintf(TEXT("FileName\t\t\t\tFileType\t\tSize\tCreatedTime\n"));
  
  do {
    const TCHAR* filetype;
    LARGE_INTEGER filesize;

    SYSTEMTIME created_time;
    TCHAR created_time_str[256];
    TCHAR created_date_str[256];
    if (::FileTimeToSystemTime(&data.ftCreationTime, &created_time) == 0) {
      print_err(TEXT("FileTimeToSystemTime"));
      return 1;
    }
    int bufsize = ::GetTimeFormatEx(LOCALE_NAME_SYSTEM_DEFAULT, 0, &created_time, NULL, NULL, 0);
    if (bufsize == 0) {
      print_err(TEXT("GetTimeFormatEx"));
      return 1;
    }
    if (::GetTimeFormatEx(LOCALE_NAME_SYSTEM_DEFAULT, 0, &created_time, NULL, created_time_str, bufsize) == 0) {
      print_err(TEXT("GetTimeFormatEx"));
      return 1;
    }
    if (::GetDateFormatEx(LOCALE_NAME_SYSTEM_DEFAULT, 0, &created_time, NULL, created_date_str, 256, NULL) == 0) {
      print_err(TEXT("GetDateFormatEx"));
      return 1;
    }

    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      filetype = TEXT("Directory");
      ::_tprintf(TEXT("%s\t\t\t\t%s\t\t-\t%s %s\n"), data.cFileName, filetype, created_date_str, created_time_str);
    }
    else {
      filetype = TEXT("File");
      filesize.LowPart = data.nFileSizeLow;
      filesize.HighPart = data.nFileSizeHigh;
      ::_tprintf(TEXT("%s\t\t\t\t%s\t\t%lld\t%s %s\n"), data.cFileName, filetype, filesize.QuadPart, created_date_str, created_time_str);
    }
  } while (::FindNextFile(find, &data) != 0);

  if (::GetLastError() != ERROR_NO_MORE_FILES) {
    print_err(TEXT("FindNextFile"));
  }

  ::FindClose(find);
  return 0;
}

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
