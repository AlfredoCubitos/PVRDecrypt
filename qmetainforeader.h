#ifndef QMETAINFOREADER_H
#define QMETAINFOREADER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QStringList>
#include <QVector>

class QMetaInfoReader : public QObject
{
    Q_OBJECT
public:
    explicit QMetaInfoReader(QObject *parent = 0);
    QByteArray keyFrame();
    QVector<QByteArray> keyFrames;

public:
    bool readFile(const QString &fileName);

private:
    void readRootindexElement();
    void readEntryElement();
    void readElement();
    void skipUnknownElement();
    
private:
    QXmlStreamReader reader;
    QByteArray m_keyframe;

signals:
    void error(QString value);

public slots:
    
};

#endif // QMETAINFOREADER_H
