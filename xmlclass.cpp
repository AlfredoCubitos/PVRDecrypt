#include "xmlclass.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"
#include <QXmlStreamReader>
#include <QStringList>
#include <QTextStream>

XmlStreamReader::XmlStreamReader(QObject *tree)
{
    treeWidget = tree;
}

bool XmlStreamReader::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString());
        return false;
    }

    reader.setDevice(&file);

    reader.readNext();
    while (!reader.atEnd()) {
        qDebug() << reader.isStartElement() << reader.name();
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
        qDebug() << "Error: Failed to parse file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(reader.errorString());
        return false;
    } else if (file.error() != QFile::NoError) {
        qDebug() << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString());
        return false;
    }
    return true;
}

void XmlStreamReader::readRootindexElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        qDebug() << reader.name();
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        qDebug() << reader.isStartElement() << reader.name();

        if (reader.isStartElement()) {
            if (reader.name() == "ContentInformation") {
                readEntryElement(treeWidget->invisibleRootItem());
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}

void XmlStreamReader::readEntryElement(QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, reader.attributes().value("term").toString());

    reader.readNext();
    while (!reader.atEnd()) {
        qDebug() << reader.name();
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        qDebug() << reader.isStartElement() << reader.name();

        if (reader.isStartElement()) {
            if (reader.name() == "Chaptering") {
                readEntryElement(item);
            } else if (reader.name() == "ChapterSegment") {
                readEntryElement(item);
            } else if (reader.name() == "KeyFrame") {
                readEntryElement(item);
            } else if (reader.name() == "InlineMedia") {
                readPageElement(item);
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}

void XmlStreamReader::readPageElement(QTreeWidgetItem *parent)
{
    QString page = reader.readElementText();
    if (reader.isEndElement())
        reader.readNext();

    QString allPages = parent->text(1);
    if (!allPages.isEmpty())
        allPages += ", ";
    allPages += page;
    parent->setText(1, allPages);
}

void XmlStreamReader::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        qDebug() << reader.name();
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        qDebug() << reader.isStartElement() << reader.name();

        if (reader.isStartElement()) {
            skipUnknownElement();
        } else {
            reader.readNext();
        }
    }
}

