#include "news.h"

#include <QDesktopServices>
#include <QMouseEvent>
#include <QLabel>
#include <QGridLayout>
#include <QNetworkReply>
#include <QTextBrowser>
#include <QDomDocument>
#include <QMessageBox>

News::News(QString url)
{
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileIsReady(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(url)));
}

void News::fileIsReady( QNetworkReply *reply)
{
    QTextBrowser *browser = new QTextBrowser;
    browser->setOpenLinks(false);
    QVBoxLayout *box = new QVBoxLayout();
    box->addWidget(browser);
    setLayout(box);
    browser->setHtml(reply->readAll());
    connect(browser, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchorClicked(QUrl)));
}

void News::finished(int)
{
    QFile err(m_err);
    if(err.exists() && err.size() > 0)
        QMessageBox::warning(this, "Error opening link", "Unable to open the link to the webpage.\nThe reason might be that you are running this application from inside a SNAP installation.\nPlease install snapd-xdg-open using\n$ sudo apt-get install snapd-xdg-open\n or copy the url\n" + m_url +"\nand open it in a browser manually.");
}

void News::anchorClicked(QUrl url)
{
    QDesktopServices::openUrl(url);
}
