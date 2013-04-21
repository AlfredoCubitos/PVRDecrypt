#ifndef MDB_H
#define MDB_H

#include <QObject>
#include <QByteArray>
#include <QFile>



class MDB : public QObject
{
    Q_OBJECT
public:
    explicit MDB(QObject *parent = 0);
    QByteArray getKey(const QString fn);


private:
    QString filename;
    QFile file;
    void errorHandling(const QString error);
    QByteArray mdbParse();
    bool mdbOpen();
    
signals:
    
public slots:
    
};

#endif // MDB_H
