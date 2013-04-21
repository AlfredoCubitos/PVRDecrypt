#ifndef QDRMDECRYPT_H
#define QDRMDECRYPT_H


#include "mdb.h"

#include <QThread>
#include <QStringList>
#include <QVector>
#include <QByteArray>
#include <QMap>


typedef struct
{
    QString sourceFolder;
    QString inFileName;
    QString mdbFileName;
    QString outFileName;
    QString infFileName;
    QStringList fields; //{channelName, recTitle, recTime, duration, info, imageDef, contentLang, category}
    QString channelName;
    QString fileSize;
    QString rawTitle;
    QString recTime;
    QString duration;
    QString recTitle;
    QString info;
    QString contentLang;
    QString category;
    QString imageDef;
    QString timerStart;
    QString timerEnd;
    QVector<QByteArray> pixmapArray;
    QString genFileName;
}   TDetails;



class QDRMDecrypt : public QThread
{
    Q_OBJECT
public:
    explicit QDRMDecrypt(QObject *parent = 0);
    virtual void setMdbFile(const QString value);
    QString mdbFile() const;
    virtual void setOutputFile(const QString value);
    QString outputFile() const;
    virtual void setInputFile(const QString value);
    QString inputFile() const;
    virtual void setOutputFolder(const QString value);
    virtual void setInputFolder(const QString value);
    void stop();


    QMap<QString,QString> fileDetails;

public:
    bool closeThread;
    int div;

protected:
    void run();

private:

    uchar inputDRMKey;
    uchar outputDRMKey[16];
    MDB key;
    QString m_mdbfile;
    QString m_outputfile;
    QString m_inputfile;
    QString m_outputfolder;
    QString m_inputfolder;

private:

    void getDRMFromMDB(char *file);
    bool process_section(uchar *data , uchar *outdata);
    bool mainProcess();

    QString errorString;

signals:
    void valueChanged(int newValue);
    void valueChanged(qint64 newValue);
    void processingIndex(int index);
    void maximuxValue(int value);
    void finished(bool normal);
    void error(QString value);

public slots:

};

#endif // QDRMDECRYPT_H
