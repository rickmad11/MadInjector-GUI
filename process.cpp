#include "process.h"

#include <array>

void Process::SetAllSystemProcesses(std::vector<std::pair<std::wstring, unsigned long long> > &process_list,
                                    std::vector<HICON> /*out*/& icon_vec,
                                    std::vector<bool>& /*out*/arch)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    (void)Process32First(hSnapShot, &pe32);
    do{
        process_list.emplace_back(std::make_pair(pe32.szExeFile, pe32.th32ProcessID));

        HANDLE pHandle = OpenProcess(PROCESS_QUERY_INFORMATION, false, pe32.th32ProcessID);

        BOOL arch_flag = false;
        (void)IsWow64Process(pHandle, &arch_flag);
        arch.push_back(arch_flag);

        char path_to_process[MAX_PATH];
        DWORD buffer_size = std::size(path_to_process);
        if(QueryFullProcessImageNameA(pHandle, NULL, path_to_process, &buffer_size)){
            HICON icon;
            if(SHDefExtractIconA(path_to_process, 0, NULL, &icon, nullptr, 32) != S_OK){
                icon_vec.push_back(nullptr);
                continue;
            }
            icon_vec.push_back(icon);
        }
        else
            icon_vec.push_back(nullptr);

        pHandle ? CloseHandle(pHandle) : 0;

    }while(Process32Next(hSnapShot, &pe32));

    hSnapShot ? CloseHandle(hSnapShot) : 0;
}
