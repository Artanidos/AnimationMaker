#include "news.h"

#include <QDesktopServices>
#include <QMouseEvent>
#include <QLabel>
#include <QGridLayout>
#include <QNetworkReply>
#include <QDomDocument>

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
                            link->setOpenExternalLinks(true);
                            link->setText("<a href='" + l.attribute("url") + "'>" + l.attribute("caption")+ "</a>");
                            layout->addWidget(link, i, j + 1);
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
