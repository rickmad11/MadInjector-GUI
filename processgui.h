#ifndef PROCESSGUI_H
#define PROCESSGUI_H

#include "qtreewidget.h"
#include <QMainWindow>

namespace Ui {
class ProcessGUI;
class MainWindow;
}

class ProcessGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProcessGUI(QWidget *parent = nullptr);
    ~ProcessGUI();

public:
    void UpdateProcessTree();
    void SetMainWindowUI(class Ui::MainWindow* const ui);
    void ForceSelectProcess(std::wstring const& process_name);

private:
    Ui::ProcessGUI *ui;

private slots:
    void RefreshProcesses();
    void FindProcess(QString const user_str);
    void SelectProcess();

private:
    void ClearList();

private:
    class Ui::MainWindow* mw_ui = nullptr;
    QTreeWidgetItem* item[1000]{};
    QList<QTreeWidgetItem*> process_tree_list;
    QMovie* pray_meme = nullptr;
    QString cached_user_str;
};

#endif // PROCESSGUI_H
