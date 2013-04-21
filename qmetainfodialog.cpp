#include "qmetainfodialog.h"

QMetaInfoDialog::QMetaInfoDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    imageTimer = new QTimer(this);
    imageTimer->setInterval(1000);
    connect(imageTimer, SIGNAL(timeout()), this, SLOT(changeImage()));
}

void QMetaInfoDialog::setItem(TDetails value)
{
    m_item = value;
    init();
}

void QMetaInfoDialog::init()
{
    imageIndex = 0;
    /*
    channelLineEdit->setText(m_item.channelName);
    titleLineEdit->setText(m_item.recTitle);
    recStartLineEdit->setText(m_item.timerStart);
    recEndLineEdit->setText(m_item.timerEnd);
    langLineEdit->setText(m_item.contentLang);
    catLineEdit->setText(m_item.category);
    infoTextBrowser->setText(m_item.info);

    if (m_item.pixmapArray.count() == 0) {
        return;
    }

    QPixmap *image = new QPixmap();
    image->loadFromData(m_item.pixmapArray.at(imageIndex));
    imageLabel->setPixmap(*image);
    imageTimer->start();
    */
}

void QMetaInfoDialog::changeImage()
{
   /* imageIndex++;
    if (imageIndex == m_item.pixmapArray.count()) {
        imageIndex = 0;
    }

    QPixmap *image = new QPixmap();
    image->loadFromData(m_item.pixmapArray.at(imageIndex));
    imageLabel->setPixmap(*image);
    */
}
