#include "news.h"

#include <QDesktopServices>
#include <QMouseEvent>
#include <QLabel>
#include <QGridLayout>
#include <QNetworkReply>
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
    QWidget *grid = new QWidget();
    QVBoxLayout *box = new QVBoxLayout();
    QGridLayout *layout = new QGridLayout();
    grid->setLayout(layout);
    box->addWidget(grid);
    box->addStretch();
    setLayout(box);
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomElement docElem = doc.documentElement();
    if(docElem.nodeName() == "AnimationMaker")
    {
        QDomElement news = docElem.firstChildElement();
        if(news.nodeName() == "News")
        {
            for(int i=0; i < news.childNodes().count(); i++)
            {
                QDomNode node = news.childNodes().at(i);
                if(node.nodeName() == "Entry")
                {
                    QDomElement entry = node.toElement();
                    layout->addWidget(new QLabel(entry.attribute("text")), i, 0);
                    for(int j=0; j < entry.childNodes().count(); j++)
                    {
                        QDomNode n = entry.childNodes().at(j);
                        if(n.nodeName() == "Link")
                        {
                            QDomElement l = n.toElement();
                            QLabel *link = new QLabel();
                            link->setText("<a href='" + l.attribute("url") + "'>" + l.attribute("caption")+ "</a>");
                            layout->addWidget(link, i, j + 1);
                            connect(link, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated(QString)));
                        }
                    }
                }
            }
        }
        else
            layout->addWidget(new QLabel("A format error occured while reading the news..."), 0, 0);
    }
    else
        layout->addWidget(new QLabel("An error occured while reading the news..."), 0, 0);
}

void News::finished(int)
{
    QFile err(m_err);
    if(err.exists() && err.size() > 0)
        QMessageBox::warning(this, "Error opening link", "Unable to open the link to the webpage.\nThe reason might be that you are running this application from inside a SNAP installation.\nPlease install snapd-xdg-open using\n$ sudo apt-get install snapd-xdg-open\n or copy the url\n" + m_url +"\nand open it in a browser manually.");
}

void News::linkActivated(QString url)
{
#ifdef Q_OS_LINUX
    // When packaging with SNAP on linux the xdg-open will no function until snapd-xdg-open will be installed additionally
    m_err = tr(qgetenv("SNAP_USER_DATA").constData()) + "/xdg-open.err";
    QFile err(m_err);
    if(err.exists())
        err.remove();

    m_url = url;
    m_proc = new QProcess();
    QStringList arguments;
    arguments << url;
    connect(m_proc, SIGNAL(finished(int)), this, SLOT(finished(int)));
    m_proc->setStandardErrorFile("");
    m_proc->start("xdg-open", arguments);
#else
    QDesktopServices::openUrl(QUrl(url));
#endif
}
