#ifndef NEWS_H
#define NEWS_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QUrl>

class News : public QWidget
{
    Q_OBJECT
public:
    News(QString url);

private slots:
    void fileIsReady(QNetworkReply * reply);
    void anchorClicked(QUrl url);
    void finished(int code);

private:
    QProcess *m_proc;
    QString m_url;
    QString m_err;
};

#endif // NEWS_H
