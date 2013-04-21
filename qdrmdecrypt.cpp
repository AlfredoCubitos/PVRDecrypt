#include "qdrmdecrypt.h"
#include "aes.c"

#include <sys/stat.h>
#include <QDebug>
#include <QFile>
#include <QThread>

QDRMDecrypt::QDRMDecrypt(QObject *parent) :
    QThread(parent)
{
    closeThread=false;
}

void QDRMDecrypt::run()
{
    if (!closeThread)
        mainProcess();
    closeThread = false;

}


void QDRMDecrypt::stop()
{
    closeThread=true;
}


bool QDRMDecrypt::process_section (uchar *data , uchar *outdata)
{
    uchar iv[0x10];
    uchar *inbuf;
//    unsigned int i, n;
    uchar *outbuf;

    memcpy(outdata, data, 188);

    if((data[3]&0xC0)==0x00)
    {
       return false;            //unverschlüsselt
    }

    int offset=4;
    /*
    if(data[3]&0x20)
        offset += (data[4] +1);      // skip adaption field
    */
    outdata[3]&=0x3f;                 // remove scrambling bits
    inbuf  = data + offset;
    outbuf = outdata + offset;

    int rounds = (188 - offset) / 0x10;
    // AES CBC
    memset(iv, 0, 16);
    for (int i =  0; i <rounds; i++) {
        unsigned char *out = outbuf + i* 0x10;
        for(int n = 0; n < 16; n++) out[n] ^= iv[n];
        aes_decrypt_128(inbuf + i* 0x10, outbuf + i * 0x10, outputDRMKey);
        memcpy(iv, inbuf + i * 0x10, 16);
    }
    return true;
}



bool QDRMDecrypt::mainProcess()
{
    int  sync_find=0;
    qint64 filesize = 0, i , j;
    qint64 dec_count = 0;
    char outdata[1024];
    char buffer[1024];
    QByteArray keyBa;
    char *keyData;
    int progress;

    QString mdbfile = QString(fileDetails["sourceFolder"]+"/"+fileDetails["mdbFileName"]);
    QString infile = QString(fileDetails["sourceFolder"]+"/"+fileDetails["inFilename"]);
    QString outfile = QString(fileDetails["outFolder"] + "/" + fileDetails["outFileName"] +".ts");



/**
 * get the key from mdb file
 **/
    keyBa = key.getKey(mdbfile);
    keyData = keyBa.data();


    for (int i=0; i< keyBa.size();++i)
    {
        outputDRMKey[i] = keyData[i];
    }



    QFile srfFile(infile);

    if(! srfFile.open(QIODevice::ReadOnly))
    {
        qDebug() <<  "Cannot open" << infile;
        errorString += "\n";
        errorString += "Cannot open " + QString(infile) + " srf file!";
        emit error(errorString);
        return false;
    }


    QFile tsFile(outfile);

    qDebug() <<  " open" << outfile;
    if(! tsFile.open(QIODevice::WriteOnly))
    {
        qDebug() <<  "Cannot open" << outfile;
        errorString += "\n";
        errorString += "Cannot open " + QString(outfile) + " ts file!";
        emit error(errorString);
        return false;
    }


    /**
     * @brief get filesize
     */
    filesize = srfFile.bytesAvailable();
    div = 0;
    if(filesize > 2147483648)
        div = 10;
    if (filesize > 4294967295)
        div = 100;
    /**
     * @brief set value for progressbar which ist int
     * @brief GB filesize is bigger than int
     **/
    progress = filesize / div;

    qDebug() << "filesze: " << filesize;
    emit maximuxValue(progress);

    srfFile.read(buffer,sizeof(buffer));

    for(i=0; i<(1024 - 188); i++){
        if (buffer[i] == 0x47 && buffer[i+188] == 0x47 && buffer[i+188+188] == 0x47){
            sync_find = 1;
            srfFile.seek(i);
            break;
        }
    }

    if (sync_find) {
        for(i=0;i<filesize;i+=188) {
            if (closeThread) break;
            srfFile.read(buffer,qint64(188));
            if (buffer[0] != 0x47)  {
                srfFile.seek(i);
                sync_find = 0;
                while (sync_find == 0) {
                    srfFile.read(buffer,sizeof(buffer));
                    for(j=0; j<(1024 - 188); j++){
                        if (buffer[j] == 0x47 && buffer[j+188] == 0x47 && buffer[j+188+188] == 0x47){
                            sync_find = 1;
                            srfFile.seek(i);
                            break;
                        }
                    }
                    i+=1024;
                }
            }else{
                dec_count += 188;

                process_section ((uchar *)buffer ,(uchar *) outdata);
                tsFile.write(outdata,qint64(188));

            }

            if (i % 7440 == 0) {
                emit valueChanged(dec_count);
                emit valueChanged((int)(dec_count / div));
            }
        }
    }
    if (!closeThread) emit valueChanged(filesize);

//    qDebug() << "drmdecrypt successfull done," << dec_count << "bytes of" << filesize << "decrypted ...!";

    return true;
}



void QDRMDecrypt::setMdbFile(const QString value)
{
    m_mdbfile = value;
}

QString QDRMDecrypt::mdbFile() const
{
    return m_mdbfile;
}

void QDRMDecrypt::setOutputFile(const QString value)
{
    m_outputfile = value;
}

QString QDRMDecrypt::outputFile() const
{
    return m_outputfile;
}

void QDRMDecrypt::setInputFile(const QString value)
{
    m_inputfile = value;
}

QString QDRMDecrypt::inputFile() const
{
    return m_inputfile;
}

void QDRMDecrypt::setOutputFolder(const QString value)
{
    m_outputfolder = value;
}

void QDRMDecrypt::setInputFolder(const QString value)
{
    m_inputfolder = value;
}
