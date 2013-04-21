#ifndef QMETAINFODIALOG_H
#define QMETAINFODIALOG_H

#include "ui_qmetainfodialog.h"
#include "qdrmdecrypt.h"
#include "readinf.h"
#include <QTimer>

class QMetaInfoDialog : public QDialog, private Ui::QMetaInfoDialog
{
    Q_OBJECT
    
public:
    explicit QMetaInfoDialog(QWidget *parent = 0);
    void setItem(TDetails value);

public:

private:
    TDetails m_item;
    QTimer *imageTimer;
    int imageIndex;

private:
    void init();

private slots:
    void changeImage();

};

#endif // QMETAINFODIALOG_H
