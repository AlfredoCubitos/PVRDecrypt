#ifndef READINF_H
#define READINF_H

#include <QMainWindow>
#include "qdrmdecrypt.h"
#include <QFile>

class mainWindowclass : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainWindowclass(QWidget *parent = 0);
    ~mainWindowclass();
    TDetails readInfFile(QString value);
    QString getOffset(QFile *inFile, int startOffset, int endOffset, QString byteOrder);
    int getOffsetHex(QFile *inFile, int startOffset, int endOffset, int size);
    virtual void setOutputFolder(const QString &value);
    QString outputFolder() const;
    virtual void setInputFolder(const QString &value);
    QString inputFolder() const;
    QDRMDecrypt *drmDecrypt;

public:
    QStringList imageDefArray;
    void writeInfFile(TDetails value);
    void setOffset(QFile *inFile, int startOffset, int endOffset, QString byteOrder, QString data);

private:
    QString m_outputfolder;
    QString m_inputfolder;



protected:
    
signals:
    
public slots:

    
};

#endif // MAINWINDOWCLASS_H

