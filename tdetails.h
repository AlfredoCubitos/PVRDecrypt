#ifndef TDETAILS_H
#define TDETAILS_H

#include <QString>
#include <QDataStream>

class TDetails
{

private:
    QString d_channelName;
    QString d_fileSize;
    QString d_rawTitle;
    QString d_recTime;
    QString d_duration;
    QString d_recTitle;
    QString d_info;
    QString d_contentLang;
    QString d_category;
    QString d_imageDef;
    QString d_timerStart;
    QString d_timerEnd;
    QByteArray d_pixmapArray;
public:
    TDetails(QString channelName, QString fileSize, QString rawTitle, QString recTime, QString duration,
             QString recTitle,  QString info, QString contentLang, QString category, QString imageDef,
             QString timerStart,  QString timerEnd,  QByteArray pixmapArray)
    {
        d_channelName = channelName;
        d_fileSize = fileSize;
        d_rawTitle = rawTitle;
        d_recTime = recTime;
        d_duration = duration;
        d_recTitle = recTitle;
        d_info = info;
        d_contentLang = contentLang;
        d_category = category;
        d_imageDef = imageDef;
        d_timerStart = timerStart;
        d_timerEnd = timerEnd;
        d_pixmapArray = pixmapArray;

    }

    TDetails (){}

    /**
     * @brief getter methods
     */


    QString  channelName() const { return  d_channelName;}
    QString  fileSize() const { return  d_fileSize;}
    QString  rawTitle() const { return  d_rawTitle;}
    QString  recTime() const { return  d_recTime;}
    QString  duration() const { return  d_duration;}
    QString  recTitle() const { return  d_recTitle;}
    QString  info() const { return  d_info;}
    QString  contentLang() const { return  d_contentLang;}
    QString  category() const { return  d_category;}
    QString  imageDef() const { return  d_imageDef;}
    QString  timerStart() const { return  d_timerStart;}
    QString  timerEnd() const { return  d_timerEnd;}
    QByteArray  pixmapArray() const { return  d_pixmapArray;}

    void  setChannelName(const QString& channelName) { d_channelName = channelName;}
    void  setFileSize(const QString& fileSize) { d_fileSize = fileSize;}
    void  setRawTitle(const QString& rawTitle) { d_rawTitle = rawTitle;}
    void  setRecTime(const QString& recTime) { d_recTime = recTime;}
    void  setDuration(const QString& duration) { d_duration = duration;}
    void  setRecTitle(const QString& recTitle) { d_recTitle = recTitle;}
    void  setInfo(const QString& info) { d_info = info;}
    void  setContentLang(const QString& contentLang) { d_contentLang = contentLang;}
    void  setCategory(const QString& category) { d_category = category;}
    void  setImageDef(const QString& imageDef) { d_imageDef = imageDef;}
    void  setTimerStart(const QString& timerStart) { d_timerStart = timerStart;}
    void  setTimerEnd(const QString& timerEnd) { d_timerEnd = timerEnd;}
    void  setPixmapArray(const QByteArray& pixmapArray) { d_pixmapArray = pixmapArray;}


    TDetails(const TDetails& td)
    {

        d_channelName = td.d_channelName;
        d_fileSize = td.d_fileSize;
        d_rawTitle = td.d_rawTitle;
        d_recTime = td.d_recTime;
        d_duration = td.d_duration;
        d_recTitle = td.d_recTitle;
        d_info = td.d_info;
        d_contentLang = td.d_contentLang;
        d_category = td.d_category;
        d_imageDef = td.d_imageDef;
        d_timerStart = td.d_timerStart;
        d_timerEnd = td.d_timerEnd;
        d_pixmapArray = td.d_pixmapArray;

    }


    TDetails& operator=(const TDetails& td)
    {

        d_channelName = td.d_channelName;
        d_fileSize = td.d_fileSize;
        d_rawTitle = td.d_rawTitle;
        d_recTime = td.d_recTime;
        d_duration = td.d_duration;
        d_recTitle = td.d_recTitle;
        d_info = td.d_info;
        d_contentLang = td.d_contentLang;
        d_category = td.d_category;
        d_imageDef = td.d_imageDef;
        d_timerStart = td.d_timerStart;
        d_timerEnd = td.d_timerEnd;
        d_pixmapArray = td.d_pixmapArray;

        return *this;
    }


};

#endif // TDETAILS_H
