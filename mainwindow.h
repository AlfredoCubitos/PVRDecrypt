#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMessageBox>
#include "readinf.h"
#include "qdrmdecrypt.h"
#include <QProgressBar>
#include "qmetainfodialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public mainWindowclass, public Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();


private:
    void getConfig(QString value);
    QLabel *statusBarLabelEnd;
    QLabel *statusBarLabelBytesProcessed;
    QProgressBar *statusBarProgress;
    QStringList fileNames;
    QString tempString;
    TDetails tempDetails;
    mainWindowclass readinf;


    QDRMDecrypt drmDecryptThread;
    bool getDRMValues();

private:
    void saveConfig(QString value);
    void addStatusLabel();
    void addStatusProgressBar();
    void setOutputFolder(const QString &value);
    void setInputFolder(const QString &value);
    void addInputFolder(const QString &value);
    void removeInputFolder(const QString &value);
    void addList(QString value);
    void enableFuncion(bool value);
    void enableDecrypt(bool value);

private slots:
    void on_actionDecrypt_triggered();
    void errorSlot(QString value);
    void on_actionBrowseOutputFolder_triggered();
    void finishedDecrypt(bool normal);
    void on_actionRemove_triggered();
    void on_actionAbout_triggered();
    void on_actionMetaWindow_triggered();
    void on_actionAdd_files_triggered();
    void editCell(int, int);
    void bytesProcessed(qint64 value);
    void on_actionStop_triggered();
    void processingIndex(int index);
    void on_actionOpen_out_folder_triggered();
    void cellTest(QTableWidgetItem* item);

};

#endif // MAINWINDOW_H

