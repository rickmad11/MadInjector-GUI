#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <windows.h>
#include <Tlhelp32.h>
#include <Shlobj.h>
#include <string>

class Process
{
public:
    Process() = default;

    static void SetAllSystemProcesses(std::vector<std::pair<std::wstring, unsigned long long>> /*out*/ & process_list,
                                      std::vector<HICON> /*out*/& icon_vec,
                                      std::vector<bool>& /*out*/arch);

private:

};

#endif // PROCESS_H
