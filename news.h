#ifndef NEWS_H
#define NEWS_H

#include <QWidget>
#include <QNetworkAccessManager>

class News : public QWidget
{
    Q_OBJECT
public:
    News(QString url);

private slots:
    void fileIsReady(QNetworkReply * reply);
};

#endif // NEWS_H
