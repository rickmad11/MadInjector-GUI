#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <fstream>
#include <QFileDialog>
#include <QMovie>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon("WindowIcon.jpg"));

    pProcess_gui = new ProcessGUI();
    pProcess_gui->SetMainWindowUI(ui);

    ui->selected_process_textButton->setReadOnly(true);
    ui->selected_dll_text->setReadOnly(true);
    ui->PID_text_Button->setReadOnly(true);
    ui->Arch_textButton->setReadOnly(true);

    ui->plainTextEdit->setReadOnly(true);

    TPbuttonGroup = new QButtonGroup(this);
    TPbuttonGroup->addButton(ui->TPWorkerThreadEntry);
    TPbuttonGroup->addButton(ui->TPWorkInsert);
    TPbuttonGroup->addButton(ui->TPJobInsert);
    TPbuttonGroup->addButton(ui->TPDirect);
    TPbuttonGroup->addButton(ui->TPWait);
    TPbuttonGroup->addButton(ui->TPTimer);
    TPbuttonGroup->addButton(ui->TPIo);
    TPbuttonGroup->addButton(ui->TPAlpc);

    flag_check_box.insert(ui->CheckBox_TSAS, Flag::THREAD_START_ADDRESS_SPOOF);
    flag_check_box.insert(ui->checkBox_THFD, Flag::THREAD_HIDE_FROM_DEBUGGER);
    flag_check_box.insert(ui->checkBox_TSTA, Flag::THREAD_SKIP_THREAD_ATTACH);
    flag_check_box.insert(ui->checkBox_HandleHijack, Flag::HIJACK_HANDLE);
    flag_check_box.insert(ui->ExceptionSupport, Flag::EXCEPTION_SUPPORT);
    flag_check_box.insert(ui->SecurityCookie, Flag::SECURITY_COOKIE);
    flag_check_box.insert(ui->BoundImports, Flag::BOUND_IMPORTS);
    flag_check_box.insert(ui->DelayedImports, Flag::DELAY_IMPORTS);
    flag_check_box.insert(ui->TLS, Flag::TLS);
    flag_check_box.insert(ui->SystemProcessOnly, Flag::SYSTEM_PROCESS_ONLY);
    flag_check_box.insert(ui->ModuleModification, Flag::MODIFY_MODULE);
    flag_check_box.insert(ui->DeleteBoundImports, Flag::REMOVE_BOUND_IMPORTS);
    flag_check_box.insert(ui->DeleteImports, Flag::REMOVE_IMPORTS);
    flag_check_box.insert(ui->DeleteRelocations, Flag::REMOVE_RELOCATIONS);
    flag_check_box.insert(ui->DeleteResources, Flag::REMOVE_RESOURCES);
    flag_check_box.insert(ui->DeleteExportTable, Flag::REMOVE_EXPORT_TABLE);
    flag_check_box.insert(ui->DeleteTLS, Flag::REMOVE_TLS);
    flag_check_box.insert(ui->DeleteDebugData, Flag::REMOVE_DEBUG_DATA);
    flag_check_box.insert(ui->DeleteDOSHeader, Flag::REMOVE_DOS_HEADER);
    flag_check_box.insert(ui->DeleteRichHeader, Flag::REMOVE_RICH_HEADER);
    flag_check_box.insert(ui->DeletePEHeader, Flag::REMOVE_PE_HEADER);
    flag_check_box.insert(ui->DeleteDOSStub, Flag::REMOVE_DOS_STUB);
    flag_check_box.insert(ui->RsrcRemove, Flag::SECTION_REMOVE_RSRC);
    flag_check_box.insert(ui->PdataRemove, Flag::SECTION_REMOVE_PDATA);
    flag_check_box.insert(ui->RelocRemove, Flag::SECTION_REMOVE_RELOC);
    flag_check_box.insert(ui->TPWorkerThreadEntry, Flag::WORKER_THREAD_CREATION);
    flag_check_box.insert(ui->TPWorkInsert, Flag::POOL_TP_WORK_INSERTION);
    flag_check_box.insert(ui->TPJobInsert, Flag::POOL_TP_JOB_INSERTION);
    flag_check_box.insert(ui->TPDirect, Flag::POOL_TP_DIRECT);
    flag_check_box.insert(ui->TPWait, Flag::POOL_TP_WAIT);
    flag_check_box.insert(ui->TPTimer, Flag::POOL_TP_TIMER);
    flag_check_box.insert(ui->TPIo, Flag::POOL_TP_IO);
    flag_check_box.insert(ui->TPAlpc, Flag::POOL_TP_ALPC);
    flag_check_box.insert(ui->unlink_module_internal, Flag::MI_UNLINK_MODULE_INTERNAL);

    setWindowTitle("MadInjector");
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::runtime_checks);
    timer->start(100);

    mad_dll = new QLibrary(R"(MadInjector.dll)");
    mad_dll->load();

    if(!mad_dll->isLoaded()){
        MadConsoleLog("dll didnt load check if dll is in the same folder as the exe");
    }

    if(mad_dll->isLoaded()){
        pfMadInjector = reinterpret_cast<decltype(pfMadInjector)>(mad_dll->resolve("wMadInjector"));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete TPbuttonGroup;
    mad_dll->unload();
    delete mad_dll;
}

void MainWindow::SetCheckBoxVisibility(QList<QCheckBox*> widgets, bool visible) {
    for (QCheckBox* const widget : widgets)
        widget->setHidden(!visible);
}

void MainWindow::CheckCompatibility(QString const& qs_method){
    ui->inject_button->setDisabled((ui->injection_method_comboBox->currentText() == "ManualMap"));

    for(const char* str : compatibility_list){
        if(qs_method == str){
            ui->inject_button->setDisabled(true);
        }
    }
}

void MainWindow::runtime_checks(){
    SetCheckBoxVisibility(
        {ui->CheckBox_TSAS, ui->checkBox_THFD, ui->checkBox_TSTA},
        (ui->exec_methods_comboBox->currentText() == "NtCreateThreadEx")
        );

    SetCheckBoxVisibility(
        {ui->ExceptionSupport, ui->DelayedImports, ui->BoundImports,
        ui->SecurityCookie, ui->TLS, ui->ModuleModification,
        ui->DeleteBoundImports, ui->DeleteImports, ui->DeleteRelocations,
        ui->DeleteResources, ui->DeleteExportTable, ui->DeleteTLS,
        ui->DeleteDebugData, ui->DeleteDOSHeader, ui->DeleteRichHeader,
        ui->DeletePEHeader, ui->DeleteDOSStub, ui->RsrcRemove,
        ui->RelocRemove, ui->PdataRemove},
        (ui->injection_method_comboBox->currentText() == "ManualMap")
        );

    SetCheckBoxVisibility(
        {ui->SystemProcessOnly},
        ui->checkBox_HandleHijack->isChecked()
        );

    SetCheckBoxVisibility(
        {ui->TPWorkerThreadEntry, ui->TPWorkInsert, ui->TPJobInsert,
        ui->TPDirect, ui->TPWait, ui->TPIo,
        ui->TPAlpc, ui->TPTimer},
        (ui->exec_methods_comboBox->currentText() == "ThreadPool")
        );

    if(ui->Arch_textButton->toPlainText() == "x86"){
        CheckCompatibility(ui->exec_methods_comboBox->currentText());
        ui->unlink_module_internal->setDisabled(true);
    }
    else{
        ui->inject_button->setDisabled(false);
        ui->unlink_module_internal->setDisabled(false);
    }
}

void MainWindow::MadConsoleLog(const QString text)
{
    ui->plainTextEdit->appendPlainText(QString("MadInfo: ") + text);
}

void MainWindow::SelectProcessClicked()
{
    pProcess_gui->show();
    pProcess_gui->UpdateProcessTree();
}

unsigned long long MainWindow::get_flag(std::map<std::wstring, unsigned long long> const& rmap, const std::wstring& method_string) {
    auto it = rmap.find(method_string);
    if (it != rmap.cend()) {
        return it->second;
    }
    return 0;
}

void MainWindow::get_additional_flags(unsigned long long& additional_flags){

    for (auto it = flag_check_box.begin(); it != flag_check_box.end(); ++it) {
        if (it.key()->isChecked()) {
            additional_flags |= it.value();
        }
    }
}

void MainWindow::Inject()
{
    MadConsoleLog(QString("Trying to Inject into [") + ui->selected_process_textButton->toPlainText() + "] Method: "+ ui->injection_method_comboBox->currentText());

    if(!mad_dll->isLoaded()){
        MadConsoleLog("dll didnt load check if dll is in the same folder as the exe");
        return;
    }

    if(selected_dllpath.isEmpty()){
        MadConsoleLog("Select a DLL first");
        return;
    }

    if(ui->PID_text_Button->toPlainText().isEmpty()){
        MadConsoleLog("Select a Process first");
        return;
    }

    std::wstring dllpath = selected_dllpath.toStdWString();
    std::wstring injection_method = ui->injection_method_comboBox->currentText().toStdWString();
    std::wstring exec_method = ui->exec_methods_comboBox->currentText().toStdWString();

    bool is_x64 = true;
    if(ui->Arch_textButton->toPlainText() == "x86")
        is_x64 = false;

    unsigned long long additional_flags = 0;

    if(ui->checkBox_DebugConsole->isChecked())
        additional_flags |= is_x64 ? Flag::CREATE_NEW_CONSOLE : Flag::USE_EXISTING_CONSOLE;

    get_additional_flags(additional_flags);

    unsigned long long injection_flag = get_flag(injection_methods, injection_method);
    unsigned long long execution_flag = get_flag(execution_methods, exec_method);

    bool status = pfMadInjector(dllpath.c_str(),
                               std::stoi(ui->PID_text_Button->toPlainText().toStdString().c_str()),
                               injection_flag,
                               execution_flag,
                               additional_flags,
                               is_x64);

   if(status)
       MadConsoleLog("Success!");
   else
       MadConsoleLog("Failed to inject");
}

void MainWindow::BrowseDLL()
{
    QString qs_dllpath = QFileDialog::getOpenFileName(this, tr("Open File"), selected_dllpath.isEmpty() ? "C://" : selected_dllpath, "DLL files (*.dll);;All files (*.*)");
    MadConsoleLog(QString("DLL filepath: [") + qs_dllpath + "]");
    selected_dllpath = qs_dllpath.isEmpty() ? selected_dllpath : qs_dllpath;

    std::string s_dllpath = qs_dllpath.toStdString();
    size_t pos = s_dllpath.rfind('/');
    s_dllpath.erase(0, pos + 1);

    ui->selected_dll_text->setPlainText(QString(s_dllpath.c_str()));
}

void MainWindow::UnlinkModule(){
    if(!mad_dll->isLoaded()){
        MadConsoleLog("dll didnt load check if dll is in the same folder as the exe");
        return;
    }

    unsigned long long additional_flags = 0;
    get_additional_flags(additional_flags);
    std::wstring dllpath = selected_dllpath.toStdWString();

    bool status = pfMadInjector(dllpath.c_str(),
                               std::stoi(ui->PID_text_Button->toPlainText().toStdString().c_str()),
                               0,
                               0,
                               (additional_flags | Flag::MI_UNLINK_MODULE),
                               true);

    if(status)
        MadConsoleLog("Success!");
    else
        MadConsoleLog("Failed to Unlink Module");
}

void MainWindow::UnloadMappedDll(){
    if(!mad_dll->isLoaded()){
        MadConsoleLog("dll didnt load check if dll is in the same folder as the exe");
        return;
    }

    bool is_x64 = true;
    if(ui->Arch_textButton->toPlainText() == "x86")
        is_x64 = false;

    std::wstring dllpath = selected_dllpath.toStdWString();
    std::wstring exec_method = ui->exec_methods_comboBox->currentText().toStdWString();
    std::wstring injection_method = ui->injection_method_comboBox->currentText().toStdWString();

    unsigned long long execution_flag = get_flag(execution_methods, exec_method);
    unsigned long long injection_flag = get_flag(injection_methods, injection_method);
    unsigned long long additional_flags = 0;

    if(ui->checkBox_DebugConsole->isChecked())
        additional_flags |= is_x64 ? Flag::CREATE_NEW_CONSOLE : Flag::USE_EXISTING_CONSOLE;

    get_additional_flags(additional_flags);

    bool status = pfMadInjector(nullptr,
                               std::stoi(ui->PID_text_Button->toPlainText().toStdString().c_str()),
                               injection_flag,
                               execution_flag,
                               (additional_flags | Flag::UNLOAD_MAPPED_MODULE),
                               true);

    if(status)
        MadConsoleLog("Success!");
    else
        MadConsoleLog("Failed to Unload Mapped Module");
}

void MainWindow::SaveCurrentSettings(){
    MadConsoleLog("Saving Settings...");

    std::wofstream file ("SaveFile.MISF", std::ios::out | std::ios::trunc);

    if(!file)
        MadConsoleLog("Save File not found... Creating new one");

    //Flags for execution injection
    //-------------------------------------
    file << ui->exec_methods_comboBox->currentIndex() << L'\n';
    file << ui->injection_method_comboBox->currentIndex() << L'\n';
    //-------------------------------------

    //Selected process and dll path
    //-------------------------------------
    std::wstring process = ui->selected_process_textButton->toPlainText().toStdWString();
    std::wstring dll_path = selected_dllpath.toStdWString();
    file << (process.empty() ? L"emptyNULL" : process) << L'\n';
    file << (dll_path.empty() ? L"emptyNULL" : dll_path) << L'\n';
    //-------------------------------------

    //Additional flags
    //-------------------------------------
    for (auto it = flag_check_box.begin(); it != flag_check_box.end(); ++it) {
        if (it.key()->isChecked()) {
            file << it.value() << L'\n';
        }
    }
    //-------------------------------------

    file.close();
    MadConsoleLog("Settings Saved!");
}

//This functions is depended on the Save function. it should be parsed in the same order it was saved
void MainWindow::LoadSettings(){
    MadConsoleLog("Loading Settings...");

    std::wifstream file ("SaveFile.MISF");

    if(!file){
        MadConsoleLog("Save File not found");
        file.close();
        return;
    }

    //Flags for execution injection
    //-------------------------------------
    int  exec_index = 0;
    file >> exec_index;
    ui->exec_methods_comboBox->setCurrentIndex(exec_index);

    int  injec_index = 0;
    file >> injec_index;
    ui->injection_method_comboBox->setCurrentIndex(injec_index);
    //-------------------------------------

    //Selected process and dll path
    //-------------------------------------
    file.ignore(2, L'\n');

    std::wstring process_name {};
    std::getline(file, process_name);

    if(process_name != L"emptyNULL")
        pProcess_gui->ForceSelectProcess(process_name);

    std::wstring dll_path {};
    std::getline(file, dll_path);

    if(dll_path != L"emptyNULL"){
        selected_dllpath = QString::fromStdWString(dll_path);

        std::string s_dllpath = selected_dllpath.toStdString();
        size_t pos = s_dllpath.rfind('/');
        s_dllpath.erase(0, pos + 1);

        ui->selected_dll_text->setPlainText(QString(s_dllpath.c_str()));
    }
    //-------------------------------------

    //Additional flags
    //-------------------------------------
    std::vector<unsigned long long> flags;
    flags.reserve(50);

    unsigned long long _flag;
    while(file >> _flag)
        flags.emplace_back(_flag);

    for (auto it = flag_check_box.begin(); it != flag_check_box.end(); ++it) {
        for (auto i : flags){
            if(it.value() == static_cast<Flag>(i)){
                it.key()->setCheckState(Qt::Checked);
                break;
            }
            else
                it.key()->setCheckState(Qt::Unchecked);
        }
    }
    //-------------------------------------

    file.close();
    MadConsoleLog("Settings Loaded!");
}

void MainWindow::DefaultSettings(){
    ui->exec_methods_comboBox->setCurrentIndex(0);
    ui->injection_method_comboBox->setCurrentIndex(0);

    ui->selected_process_textButton->setText("");
    ui->PID_text_Button->setText("");
    ui->Arch_textButton->setText("");

    for (auto it = flag_check_box.begin(); it != flag_check_box.end(); ++it) {
        it.key()->setCheckState(Qt::Unchecked);
    }
}
