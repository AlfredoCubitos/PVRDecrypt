#include "mdb.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>

MDB::MDB(QObject *parent) :
    QObject(parent)
{

}

QByteArray MDB::getKey(const QString fn)
{

    filename = fn;
    if(mdbOpen())
    {

       return mdbParse();
    }
}

QByteArray MDB::mdbParse()
{
    QByteArray result;

    char buf;
    file.seek(qint64(8));
    for (int j = 0; j < 0x10; j++)
    {
        file.getChar(&buf);

        result[(j & 0x0c) + (3 - (j & 0x03))] = buf;

    }
    return result;

}

bool MDB::mdbOpen()
{
    file.setFileName(filename);
    if (file.bytesAvailable() > 0)
    {
        file.open(QIODevice::ReadOnly);
        return true;
    }else{
        this->errorHandling("cannot open " + filename);
    }
   return false;
}

void MDB::errorHandling(const QString error)
{
    qDebug() << error;
    //QMessageBox::warning(QWidget *MainWindow,tr("Fehler"),tr("%1").arg(error));
}
