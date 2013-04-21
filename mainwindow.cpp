#include "mainwindow.h"
#include <QSettings>
#include <QFileDialog>
#include <QProgressDialog>
#include <QTextCodec>
#include <QCheckBox>
#include <QDirModel>
#include <QFileSystemModel>
#include <QBuffer>
#include <QPicture>
#include <QDesktopServices>
#include "qmetainforeader.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    mainWindowclass(parent)
{
    setupUi(this);
    init();
}

void MainWindow::init()
{
    getConfig("config.ini");
    addStatusProgressBar();
    addStatusLabel();

    connect(&drmDecryptThread, SIGNAL(valueChanged(int)), statusBarProgress, SLOT(setValue(int)));
    connect(&drmDecryptThread, SIGNAL(valueChanged(qint64)), this, SLOT(bytesProcessed(qint64)));
    connect(&drmDecryptThread, SIGNAL(maximuxValue(int)), statusBarProgress, SLOT(setMaximum(int)));
    connect(&drmDecryptThread, SIGNAL(error(QString)), this, SLOT(errorSlot(QString)));
    connect(&drmDecryptThread, SIGNAL(finished(bool)), this, SLOT(finishedDecrypt(bool)));
    connect(&drmDecryptThread, SIGNAL(processingIndex(int)), this, SLOT(processingIndex(int)));

    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editCell(int,int)));



    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);


}

MainWindow::~MainWindow()
{
    saveConfig("config.ini");
    drmDecryptThread.stop();
}



void MainWindow::cellTest(QTableWidgetItem* item)
{
    if(item->isSelected())
        QMessageBox::information(this,"Cell",QString(item->text()));
    tempDetails.rawTitle = item->text();
    readinf.writeInfFile(tempDetails);

}

void MainWindow::bytesProcessed(qint64 value)
{

    if (value < 1024)
        tempString = QString("%1 B").arg(value,'f',2);
    if (value / 1024 > 0) {
        tempString = QString("%1 kB processed").arg( ((value/1024), 'f', 2 ));
    }
    if (value / (1024*1024) > 0) {

        double val = ((double) value/(1024*1024));
         tempString = QString("%1 MB processed").arg(val, 0, 'f', 2);
    }
    if (value / (1024*1024*1024) > 0) {
        double val = ((double)value/(1024*1024*1024));

        tempString = QString("%1 GB processed").arg(val, 0, 'f', 2 );
    }
    statusBarLabelBytesProcessed->setText(tempString);
}

void MainWindow::editCell(int row, int col)
{
    connect (tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(cellTest(QTableWidgetItem*)));

}

void MainWindow::getConfig(QString value)
{;
    QSettings settings( value, QSettings::IniFormat);
    settings.beginGroup("config");
    setInputFolder(settings.value("sourcefolder").toString());
    setOutputFolder(settings.value("targetfolder").toString());
    this->setGeometry(settings.value("geometry").toRect());
    settings.endGroup();
}

void MainWindow::saveConfig(QString value)
{
    QSettings settings( value, QSettings::IniFormat);
    settings.beginGroup("config");
    settings.setValue("sourcefolder",inputFolder());
    settings.setValue("targetfolder",outputFolder());
    settings.setValue("geometry",this->geometry());
    settings.endGroup();
}

void MainWindow::addList(QString value)
{
    QFileInfo tempFileInfo(value);
    int row = tableWidget->rowCount();
    QPixmap *image = new QPixmap();
    QString tempMTAFileName = value.left(value.length()-3)+"mta";
    QFile tempMTAFile(tempMTAFileName);
    QLabel *tempLabel = new QLabel();
    QTableWidgetItem *newItem;

    tableWidget->insertRow(row);
    tableWidget->setRowHeight(row, 90);

    setInputFolder(tempFileInfo.absolutePath());

    drmDecryptThread.fileDetails["sourceFolder"] = QString(tempFileInfo.absolutePath());
    drmDecryptThread.fileDetails["inFilename"] = tempFileInfo.fileName();
    drmDecryptThread.fileDetails["mdbFileName"] = tempFileInfo.fileName().left(tempFileInfo.fileName().length()-3)+"mdb";
   // drmDecryptThread.fileDetails["outFileName"] = tempFileInfo.fileName().left(tempFileInfo.fileName().length()-3)+"ts";

    tempDetails = readInfFile(value.left(value.length()-3)+"inf");
    tempDetails.infFileName = value.left(value.length()-3)+"inf";
    tempDetails.sourceFolder = inputFolder();
    tempDetails.inFileName = tempFileInfo.fileName();
    tempDetails.fileSize = QString::number(tempFileInfo.size()/1024/1024)+ " MB";
    tempDetails.mdbFileName = tempFileInfo.fileName().left(tempFileInfo.fileName().length()-3)+"mdb";
    tempDetails.outFileName = tempFileInfo.fileName().left(tempFileInfo.fileName().length()-3)+"ts";

    tempLabel->setGeometry(0,0,160,90);
    tempLabel->setScaledContents(true);
    if (tempMTAFile.exists()) {
        QMetaInfoReader metaInfoReader;
        connect(&metaInfoReader, SIGNAL(error(QString)), this, SLOT(errorSlot(QString)));
        metaInfoReader.keyFrames.clear();
        if (metaInfoReader.readFile(tempMTAFileName)) {
            tempDetails.pixmapArray = metaInfoReader.keyFrames;
            image->loadFromData(tempDetails.pixmapArray.at(0));
        }
    }
    tempLabel->setPixmap(*image);
    tableWidget->setCellWidget(row, 0, tempLabel);

    newItem = new QTableWidgetItem(tempDetails.inFileName);
    tableWidget->setItem(row, 1, newItem);
  //  drmDecrypt->decryptTasks.append(tempDetails);
    for (int i = 0; i < 7; ++i) {
        newItem = new QTableWidgetItem(tempDetails.fields.at(i));
        tableWidget->setItem(row, i+2, newItem);
    }

    newItem = new QTableWidgetItem(tempDetails.fileSize);
    newItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tableWidget->setItem(row, 7, newItem);

    newItem = new QTableWidgetItem;
    newItem->setIcon(QIcon(":/remove"));
    tableWidget->setItem(row, tableWidget->columnCount()-1, newItem);

    for (int i = 1; i < tableWidget->columnCount(); ++i) {
        tableWidget->resizeColumnToContents(i);
    }
}

void MainWindow::processingIndex(int index)
{
    tableWidget->selectRow(index+1);
}

bool MainWindow::getDRMValues()
{
    QList<QTableWidgetItem *> items;
    items = tableWidget->selectedItems();

    if(items.size() > 0)
    {
        drmDecryptThread.fileDetails["outFolder"] = selectOutputFolderLineEdit->text();
        drmDecryptThread.fileDetails["outFileName"] = QString(items.at(1)->text() +"-"+items.at(2)->text());
        return true;
    }

    return false;
}

void MainWindow::on_actionDecrypt_triggered()
{
    if (!getDRMValues())
    {
        emit errorSlot("Nothing selected to decrypt");

    }else {
        drmDecryptThread.start();
        enableDecrypt(false);
    }


}

void MainWindow::addStatusLabel()
{
    statusBarLabelBytesProcessed = new QLabel( statusBar1 );
    statusBarLabelBytesProcessed->resize(500,statusBarLabelBytesProcessed->height());
    statusBarLabelBytesProcessed->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    statusBarLabelBytesProcessed->setText("");
    statusBar()->addWidget( statusBarLabelBytesProcessed, 1 );
    statusBarLabelEnd = new QLabel( statusBar1 );
    statusBar()->addWidget( statusBarLabelEnd );
}

void MainWindow::addStatusProgressBar()
{
    statusBarProgress = new QProgressBar( statusBar1 );
    statusBar()->addWidget( statusBarProgress, 4 );
}

void MainWindow::errorSlot(QString value)
{
    QMessageBox::critical(this, "ERROR", value);
    drmDecryptThread.stop();
    enableDecrypt(true);
}

void MainWindow::on_actionBrowseOutputFolder_triggered()
{
    QString outfolder;

    outfolder = QFileDialog::getExistingDirectory(this, "Open srf", outputFolder(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    setOutputFolder(outfolder);

    drmDecryptThread.fileDetails["outFolder"] = outfolder;
}

void MainWindow::setOutputFolder(const QString &value)
{
    mainWindowclass::setOutputFolder(value);
    drmDecrypt->setOutputFolder(value);
    selectOutputFolderLineEdit->setText(value);
}
void MainWindow::setInputFolder(const QString &value)
{
    mainWindowclass::setInputFolder(value);
}

void MainWindow::finishedDecrypt(bool normal)
{
    QString finishMessage = "Tasks have been canceled";
    if (normal) finishMessage = "All tasks has been finished!";
    QMessageBox::information(this, "Task finish", finishMessage);
    statusBarProgress->reset();
    statusBarLabelBytesProcessed->clear();
    enableDecrypt(true);
}

void MainWindow::on_actionRemove_triggered()
{
    int row = tableWidget->currentRow();

    tableWidget->removeRow(row);

    enableFuncion(false);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About PVR Decrypt GUI",
                       "<p>Select <b>File->Open files</b> and "
                       "choose one or more encrypted recording. The files are then "
                       "loaded into the task list, if inf file exists "
                       "video meta datas are loaded too.</p>"

                       "<p>Select with first row checkbox "
                       "file naming structure. "
                       "If no structure setted, default naming "
                       "(Channel name - Title - Image definition) used.</p>");
}

void MainWindow::on_actionMetaWindow_triggered()
{
    QMetaInfoDialog *metaInfoDialog;
    metaInfoDialog = new QMetaInfoDialog(this);
    //metaInfoDialog->setItem(drmDecrypt->decryptTasks.at(row));
    metaInfoDialog->exec();
}

void MainWindow::on_actionAdd_files_triggered()
{
     QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select one or more files to open", inputFolder(), "Samsung recordings (*.srf)" );

    if (fileNames.count() == 0) return;

    enableFuncion(true);
    foreach (QString fileName, fileNames) {
        addList(fileName);
    }
}

void MainWindow::enableFuncion(bool value)
{
    actionDecrypt->setEnabled(value);
    actionRemove->setEnabled(value);
    actionStop->setEnabled(value);
    decryptPushButton->setEnabled(value);
    stopPushButton->setEnabled(value);
}

void MainWindow::enableDecrypt(bool value)
{
    actionDecrypt->setEnabled(value);
    actionRemove->setEnabled(value);
    decryptPushButton->setEnabled(value);
}


void MainWindow::on_actionStop_triggered()
{
    drmDecryptThread.stop();
    drmDecryptThread.wait();
    enableFuncion(true);

}


void MainWindow::on_actionOpen_out_folder_triggered()
{
    QDesktopServices::openUrl(outputFolder());
}
