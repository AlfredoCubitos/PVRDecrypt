#include "readinf.h"
#include <QTextCodec>
#include <QTime>
#include <QDebug>

int CHANNELNAME        = 0x0000; //
int RAWTITLE           = 0x0100; // I"ve never seen this on tv
int CHANNELNUMBER      = 0x0200; //
int X0300              = 0x0300; //
int RECTIME            = 0x0304; // start time
int DURATION           = 0x0308; // duration
int X0314              = 0x0314; //
int RECTITLE           = 0x0318; // title
int X0530              = 0x0530; // 0x08 or 0x2A
int LANGCODE           = 0x0534; //
int X0544              = 0x0544; // 02 or 04
int X054C              = 0x054C; // 02 or 04
int X056E              = 0x056E; // 3602 or 00 00
int GUIDANCEINFOLENGTH = 0x065C; // Guidance Information string length (0=no guidance info)
int INFOLENGTH         = 0x065E; //
int GUIDANCEINFO       = 0x0660; // Guidance Information text
int INFO               = 0x06F6; //
int TIMERSTART         = 0x18F8; // start time if timer was used
int TIMEREND           = 0x18FC; // end time if timer was used
int HD                 = 0x1904; // HD flag (1=HD, 0=SD)
int SUBTITLES          = 0x1906; // Subtitles (1=present, 0=absent)
int TELETEXT           = 0x1908; // Teletext flag (1=present, 0=absent)
int AUDIODESC          = 0x190A; // Audio Description flag (1=present, 0=absent)
int SUBTITLES4HI       = 0x190C; // Subtitles for hearing impaired (1=present, 0=absent)
int CONTENTLANG        = 0x1910; // Language of content (reverse!)
int CATEGORY           = 0x1B0E; // see CATIDX
int ASPECTRATIO        = 0x1D0C; // Aspect Ratio (0=1:1, 1=4:3, 2=16:9)
int IMAGEDEF           = 0x1D10; // Image Definition (0=1080i, 1=1080p, 2=288i, 3=480i, 4=480p, 5=576i, 6=576p, 7=720p)
int LOCK               = 0x1D14; // (1=locked, 0=unlocked)
int MINAGE             = 0x1D18; // Minimum Recommended Age (0x01=4 years, 0x02=5 years, 0x03=6 years, and so on up to 0x0F=18 years, or FF=suitable for all or not rated)
int FAVORITE           = 0x1D1C; // Favourites flags (bits 0-4 for Favourite 1 to 5) (It"s not a simple flag!)
int SPLIT              = 0x1D20; // Split flag (1=no split)
int LANGIDX            = 0x1D24; // Index of the Language string in the list of translated strings
int CATIDX             = 0x1D26; // Index of the category string in the list of translated strings

mainWindowclass::mainWindowclass(QWidget *parent) :
    QMainWindow(parent)
{
    imageDefArray << "1080i" << "1080p" << "288i" << "480i" << "480p" << "576i" << "576p" << "720p";
    //metaInfoReader = new QMetaInfoReader(this);
    drmDecrypt = new QDRMDecrypt(this);
}

mainWindowclass::~mainWindowclass()
{
     delete drmDecrypt;
//    delete metaInfoReader;
}



QString mainWindowclass::getOffset(QFile *inFile, int startOffset, int endOffset, QString byteOrder)
{
    QString str;
    inFile->seek(startOffset);
    QByteArray utf16 = inFile->read(endOffset);

    QTextStream ds(&utf16, QIODevice::ReadOnly);

    if (byteOrder == "BE")
         ds.setCodec("UTF-16BE");
    if (byteOrder == "LE")
        ds.setCodec("UTF-16LE");

    while (!ds.atEnd())
        str.append(ds.read(1));

    return str.toLatin1();
}

int mainWindowclass::getOffsetHex(QFile *inFile, int startOffset, int endOffset, int size)
{
    bool ok;
    QByteArray sourceByte;
    QByteArray tempBA;
    inFile->seek(startOffset);
    sourceByte = inFile->read(endOffset);

    tempBA.resize(size);
    for (int i = 0; i < size; ++i) {
        tempBA[i] = sourceByte[(size-1)-i];
    }
    return tempBA.toHex().toUInt(&ok,16);
}

void mainWindowclass::writeInfFile(TDetails value)
{
    QFile *outFile = new QFile(value.infFileName);
    outFile->open(QIODevice::ReadWrite);
    qDebug() << value.infFileName << "Text " << outFile->errorString();

    setOffset( outFile,RECTITLE, X0530-RECTITLE, "LE", value.rawTitle);
}

void mainWindowclass::setOffset(QFile *inFile, int startOffset, int endOffset, QString byteOrder, QString data)
{
    inFile->seek(startOffset);
    int i;
    i = 0;

    QTextStream out(inFile);


    if (byteOrder == "BE")
         out.setCodec("UTF-16BE");
    if (byteOrder == "LE")
        out.setCodec("UTF-16LE");


    for(int i=startOffset;i < endOffset - data.length();++i)
        data.append(0);
        out << data  ;
      //  inFile->write(outByte);

}

TDetails mainWindowclass::readInfFile(QString value)
{
    TDetails result;
    QDateTime timestamp;


    QFile *inFile = new QFile(value);

    inFile->open(QIODevice::ReadOnly);



    result.fields << getOffset(inFile, CHANNELNAME, RAWTITLE-CHANNELNAME,"BE");
    result.recTitle = getOffset(inFile, RECTITLE, X0530-RECTITLE, "LE");
    result.fields << result.recTitle;
    timestamp.setTime_t(getOffsetHex(inFile, RECTIME, DURATION-RECTIME, 4));
    result.recTime = timestamp.toString("yyyy-MM-dd hh:mm:ss");
    result.fields << result.recTime;
    timestamp.setTime_t(getOffsetHex(inFile, DURATION, X0314-DURATION, 2)+24*60*60-3600);
    result.duration = timestamp.toString("hh:mm:ss");
    result.fields << result.duration;
    int timerstart = getOffsetHex(inFile, TIMERSTART, TIMEREND-TIMERSTART, 4);
    if (timerstart > 0) {
        timestamp.setTime_t(getOffsetHex(inFile, TIMERSTART, TIMEREND-TIMERSTART, 4));
        result.timerStart = timestamp.toString("yyyy-MM-dd hh:mm:ss");
    }
    int timerend = getOffsetHex(inFile, TIMERSTART, TIMEREND-TIMERSTART, 4);
    if (timerend > 0) {
        timestamp.setTime_t(timerend);
        result.timerEnd = timestamp.toString("yyyy-MM-dd hh:mm:ss");
    }

    result.info = getOffset(inFile, INFO, TIMERSTART-INFO, "BE");
    result.imageDef = imageDefArray.at(getOffsetHex(inFile, IMAGEDEF, LOCK-IMAGEDEF, 1));
    result.fields << result.imageDef;
    result.contentLang = getOffset(inFile, CONTENTLANG, CATEGORY-CONTENTLANG, "LE");
    result.fields << result.contentLang;
    result.category = getOffset(inFile, CATEGORY, ASPECTRATIO-CATEGORY, "LE");
    result.fields << result.category;
    result.rawTitle = getOffset(inFile, RAWTITLE, CHANNELNUMBER-RAWTITLE, "BE");

    //tempInt = getOffsetHex(inFile, LANGIDX, CATIDX-LANGIDX, 2);
    //imageDef = imageDefArray.at(tempInt);
    //qDebug() << imageDef << tempInt;

    //tempInt = getOffsetHex(inFile, CATIDX, 4, 2);
    //imageDef = imageDefArray.at(tempInt);
    //qDebug() << imageDef << tempInt;

    inFile->close();
    return result;
}

void mainWindowclass::setOutputFolder(const QString &value)
{
    m_outputfolder = value;
}

QString mainWindowclass::outputFolder() const
{
    return m_outputfolder;
}

void mainWindowclass::setInputFolder(const QString &value)
{
    m_inputfolder = value;
}

QString mainWindowclass::inputFolder() const
{
    return m_inputfolder;
}
