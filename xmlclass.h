#ifndef XMLCLASS_H
#define XMLCLASS_H

#include <QXmlStreamReader>

class xmlClass : public QObject
{
    Q_OBJECT
public:
    explicit xmlClass(QObject *parent = 0);



class XmlStreamReader
{
public:
    XmlStreamReader(QObject *tree);

    bool readFile(const QString &fileName);

private:
    void readRootindexElement();
    void readEntryElement(QTreeWidgetItem *parent);
    void readPageElement(QTreeWidgetItem *parent);
    void skipUnknownElement();

    QXmlStreamReader reader;
};


#endif // XMLCLASS_H
