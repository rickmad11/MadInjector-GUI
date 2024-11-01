#include "processgui.h"
#include "ui_processgui.h"

#include "process.h"
#include "ui_mainwindow.h"

#include <QMovie>

Q_GUI_EXPORT QPixmap qt_pixmapFromWinHICON(HICON icon);

ProcessGUI::ProcessGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessGUI)
{
    ui->setupUi(this);

    setWindowIcon(QIcon("WindowIcon.jpg"));

    ui->treeWidget->setColumnCount(3);

    ui->treeWidget->setColumnWidth(0, 55);
    ui->treeWidget->setColumnWidth(1, 55);
    ui->treeWidget->setColumnWidth(2, 130);

    setWindowTitle("MadInjector");
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    pray_meme = new QMovie(QApplication::applicationDirPath() + "/pray_meme.gif");
    ui->pray_meme->setMovie(pray_meme);
    pray_meme->start();
}

ProcessGUI::~ProcessGUI()
{
    delete ui;

    pray_meme ? delete pray_meme : void();

    for(size_t index = 0; index < std::size(item); index++)
        item[index] ? delete item[index] : void();
}

void ProcessGUI::ClearList()
{
    for(size_t index = 0; index < std::size(item); index++) {
        item[index] ? delete item[index] : void();
        item[index] = nullptr;
    }
    process_tree_list.clear();
}

void ProcessGUI::SetMainWindowUI(Ui::MainWindow * const ui)
{
    mw_ui = ui;
}

void ProcessGUI::RefreshProcesses()
{
    UpdateProcessTree();
}

void ProcessGUI::FindProcess(const QString user_str)
{
    cached_user_str = user_str;
    for (QTreeWidgetItem* item : process_tree_list) {
        bool matches = user_str.isEmpty() || item->text(2).contains(user_str, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

void ProcessGUI::SelectProcess()
{
    for (QTreeWidgetItem* item : process_tree_list) {
        if(item->isSelected()){
            mw_ui->selected_process_textButton->setText(item->text(2));
            mw_ui->PID_text_Button->setText(item->text(1));
            mw_ui->Arch_textButton->setText(item->text(0));
        }
    }
    this->close();
}

void ProcessGUI::ForceSelectProcess(std::wstring const& process_name){
    UpdateProcessTree();
    for (QTreeWidgetItem* item : process_tree_list) {
        QString q_string_process_name = item->data(2, Qt::DisplayRole).toString();

        if(q_string_process_name.toStdWString() == process_name){
            item->setSelected(true);
            continue;
        }

        item->setSelected(false);
    }
    SelectProcess();
}

void ProcessGUI::UpdateProcessTree()
{
    ClearList();

    std::vector<std::pair<std::wstring, unsigned long long>> processes = {};
    std::vector<HICON> icons = {};
    std::vector<bool> process_arch = {};

    Process::SetAllSystemProcesses(processes, icons, process_arch);

    if(processes.size() > std::size(item))
        processes.resize(std::size(item));

    for(size_t index = 0; index < processes.size(); index++){
        item[index] = new QTreeWidgetItem(ui->treeWidget);

        if(index < icons.size() && icons.at(index)){
            QPixmap p_icon = qt_pixmapFromWinHICON(icons.at(index));
            item[index]->setIcon(2, p_icon);
        }

        item[index]->setData(2, Qt::DisplayRole , QVariant(QString::fromWCharArray(processes.at(index).first.data()).toStdString().c_str()));
        item[index]->setData(1, Qt::DisplayRole , QVariant(QString::number(processes.at(index).second).toStdString().c_str()));

        if(process_arch.at(index))
            item[index]->setData(0, Qt::DisplayRole, QVariant("x86"));
        else
        item[index]->setData(0, Qt::DisplayRole, QVariant("x64"));

        ui->treeWidget->insertTopLevelItem(index, item[index]);
        process_tree_list.push_back(item[index]);
    }

    cached_user_str.toStdString().empty() ? void() : FindProcess(cached_user_str);
}

