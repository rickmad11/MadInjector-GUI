#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QLibrary>
#include <QCheckBox>
#include "processgui.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void MadConsoleLog(const QString text);

private:
    void runtime_checks();

private slots:
    void SelectProcessClicked();
    void Inject();
    void BrowseDLL();
    void UnlinkModule();
    void UnloadMappedDll();
    void SaveCurrentSettings();
    void LoadSettings();
    void DefaultSettings();

private:
    unsigned long long get_flag(std::map<std::wstring, unsigned long long> const& rmap, const std::wstring& method_string);
    void get_additional_flags(unsigned long long& additional_flags);
    void SetCheckBoxVisibility(QList<QCheckBox*> widgets, bool visible);
    void CheckCompatibility(QString const& qs_method);

private:
    Ui::MainWindow *ui;
    ProcessGUI* pProcess_gui = nullptr;
    QButtonGroup* TPbuttonGroup = nullptr;
    QLibrary* mad_dll;
    bool(*pfMadInjector)(const wchar_t*, unsigned int, unsigned long long, unsigned long long, unsigned long long, bool);

private:
    QString selected_dllpath;

    enum Flag : unsigned long long
    {
        LOAD_LIBRARY                    = 1ULL,
        LDR_LOAD_DLL                    = 1ULL << 1,
        LDR_P_LOAD_DLL                  = 1ULL << 2,
        LDR_P_LOAD_DLL_INTERNAL         = 1ULL << 3,
        MI_MANUAL_MAP                   = 1ULL << 4,

        CREATE_REMOTE_THREAD            = 1ULL,
        NT_CREATE_THREAD_EX             = 1ULL << 1,
        SET_WINDOWS_HOOK                = 1ULL << 2,
        THREAD_HIJACK                   = 1ULL << 3,
        QUEUE_USER_APC                  = 1ULL << 4,
        KERNEL_CALLBACK_TABLE 	        = 1ULL << 5,
        VECTORED_EXCEPTION_HANDLER      = 1ULL << 7,

        CREATE_NEW_CONSOLE              = 1ULL,
        USE_EXISTING_CONSOLE            = 1ULL << 1,

        THREAD_HIDE_FROM_DEBUGGER       = 1ULL << 2,
        THREAD_SKIP_THREAD_ATTACH       = 1ULL << 3,
        THREAD_START_ADDRESS_SPOOF      = 1ULL << 4,

        HIJACK_HANDLE                   = 1ULL << 5,
        SYSTEM_PROCESS_ONLY             = 1ULL << 12,

        EXCEPTION_SUPPORT               = 1ULL << 7,
        SECURITY_COOKIE                 = 1ULL << 8,
        BOUND_IMPORTS                   = 1ULL << 9,
        DELAY_IMPORTS                   = 1ULL << 10,
        TLS                             = 1ULL << 11,
        MODIFY_MODULE                   = 1ULL << 14,
        REMOVE_BOUND_IMPORTS            = 1ULL << 15,
        REMOVE_IMPORTS                  = 1ULL << 16,
        REMOVE_RELOCATIONS              = 1ULL << 17,
        REMOVE_RESOURCES                = 1ULL << 18,
        REMOVE_EXPORT_TABLE             = 1ULL << 19,
        REMOVE_TLS                      = 1ULL << 20,
        REMOVE_DEBUG_DATA               = 1ULL << 21,
        REMOVE_PE_HEADER                = 1ULL << 22,
        REMOVE_DOS_HEADER               = 1ULL << 23,
        REMOVE_RICH_HEADER              = 1ULL << 24,
        REMOVE_DOS_STUB                 = 1ULL << 25,
        SECTION_REMOVE_RELOC            = 1ULL << 27,
        SECTION_REMOVE_RSRC             = 1ULL << 28,
        SECTION_REMOVE_PDATA            = 1ULL << 29,

        THREAD_POOL_EXECUTION           = 1ULL << 30,
        WORKER_THREAD_CREATION          = 1ULL << 31,
        POOL_TP_WORK_INSERTION          = 1ULL << 32,
        POOL_TP_JOB_INSERTION           = 1ULL << 33,
        POOL_TP_DIRECT                  = 1ULL << 34,
        POOL_TP_WAIT                    = 1ULL << 35,
        POOL_TP_TIMER                   = 1ULL << 36,
        POOL_TP_IO                      = 1ULL << 37,
        POOL_TP_ALPC                    = 1ULL << 38,

        INSTRUMENTATION_CALLBACK        = 1ULL << 39,
        TLS_CALLBACK_EXECUTION          = 1ULL << 41,

        MI_UNLINK_MODULE                = 1ULL << 13,
        UNLOAD_MAPPED_MODULE            = 1ULL << 26,
        MI_UNLINK_MODULE_INTERNAL       = 1ULL << 40,
    };

    std::array<const char*, 4> compatibility_list = {"ThreadPool", "InstrumentationCallback", "VectoredExceptionHandler", "TlsCallback"};

    QHash<QCheckBox*, Flag> flag_check_box;

    std::map<std::wstring, unsigned long long> injection_methods =
        {
            {L"LoadLibrary", Flag::LOAD_LIBRARY},
            {L"LdrLoadDll", Flag::LDR_LOAD_DLL},
            {L"LdrpLoadDll", Flag::LDR_P_LOAD_DLL},
            {L"LdrpLoadDllInternal", Flag::LDR_P_LOAD_DLL_INTERNAL},
            {L"ManualMap", Flag::MI_MANUAL_MAP},
        };

    std::map<std::wstring, unsigned long long> execution_methods =
        {
            {L"CreateRemoteThread", Flag::CREATE_REMOTE_THREAD},
            {L"NtCreateThreadEx", Flag::NT_CREATE_THREAD_EX},
            {L"SetWindowsHook", Flag::SET_WINDOWS_HOOK},
            {L"ThreadHijack", Flag::THREAD_HIJACK},
            {L"QueueUserAPC", Flag::QUEUE_USER_APC},
            {L"KernelCallbackTable", Flag::KERNEL_CALLBACK_TABLE},
            {L"VectoredExceptionHandler", Flag::VECTORED_EXCEPTION_HANDLER},
            {L"ThreadPool", Flag::THREAD_POOL_EXECUTION},
            {L"InstrumentationCallback", Flag::INSTRUMENTATION_CALLBACK},
            {L"TlsCallback", Flag::TLS_CALLBACK_EXECUTION}
        };
};
#endif // MAINWINDOW_H
