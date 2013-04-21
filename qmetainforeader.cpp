#include "qmetainforeader.h"
#include <QFile>

QMetaInfoReader::QMetaInfoReader(QObject *parent) :
    QObject(parent)
{
}

bool QMetaInfoReader::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        emit error("Error: Cannot read file " + fileName + ": " + file.errorString());
        return false;
    }

    reader.setDevice(&file);

    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "SECMeta") {
                readRootindexElement();
            } else {
                reader.raiseError(QObject::tr("Not a bookindex file"));
            }
        } else {
            reader.readNext();
        }
    }

    file.close();
    if (reader.hasError()) {
        emit error("Error: Failed to parse file " + fileName + ": " + reader.errorString());
        return false;
    } else if (file.error() != QFile::NoError) {
        emit error("Error: Cannot read file " + fileName + ": " + reader.errorString());
        return false;
    }
    return true;
}

void QMetaInfoReader::readRootindexElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            if (reader.name() == "ContentInformation") {
                readEntryElement();
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}

void QMetaInfoReader::readEntryElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            if (reader.name() == "Chaptering") {
                readEntryElement();
            } else if (reader.name() == "ChapterSegment") {
                readEntryElement();
            } else if (reader.name() == "KeyFrame") {
                readEntryElement();
            } else if (reader.name() == "InlineMedia") {
                readElement();
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}

void QMetaInfoReader::readElement()
{
    QString element = reader.readElementText();
    keyFrames << QByteArray::fromBase64(element.toLocal8Bit());
    m_keyframe = element.toLocal8Bit();
    if (reader.isEndElement())
        reader.readNext();
}

void QMetaInfoReader::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            skipUnknownElement();
        } else {
            reader.readNext();
        }
    }
}

QByteArray QMetaInfoReader::keyFrame()
{
    return m_keyframe;
}
