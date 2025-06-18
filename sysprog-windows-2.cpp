// sysprog-windows-2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>

#include <cstdio>
#include <tchar.h>

int main(int argc, char** argv) {
  TCHAR program_name[MAX_PATH];
  GetModuleFileName(NULL, program_name, MAX_PATH);

  if ((argc != 1 && argc != 3)
      || (argc == 3 && strncmp(argv[1], "/s", 3) != 0)) {
    ::_tprintf(TEXT("Usage: %s [/s DIR]"), program_name);
  }
}

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
