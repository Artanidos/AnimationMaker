/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "htmlexport.h"
#include "animationitem.h"
#include "animationscene.h"
#include <QStatusBar>
#include <QFileDialog>
#include <QTest>
#include <QMessageBox>

void HtmlExport::exportAnimation(AnimationScene *scene, QStatusBar *bar)
{
    QList<QGraphicsItem*> itemList;

    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::Directory);
    dialog->setWindowTitle(tr("Export Animation to HTML"));
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if(fileName.isEmpty())
        return;

    QDir dir(fileName);
    dir.mkdir("assets");
    dir.cd("assets");
    dir.mkdir("css");
    dir.mkdir("images");
    dir.mkdir("js");
    dir.cdUp();

    itemList = scene->items(Qt::AscendingOrder);
    bar->showMessage("Exporting animation to " + fileName);


    QFile htmlFile(fileName + "/index.html");
    if(!htmlFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0, "Error", "Unable to open file " + fileName + "/index.html");
        return;
    }
    QFile jsFile(fileName + "/assets/js/animationmaker.js");
    if(!jsFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0, "Error", "Unable to open file " + fileName + "/assets/js/animationmaker.js");
        htmlFile.close();
        return;
    }

    QTextStream html(&htmlFile);
    html.setCodec("UTF-8");
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<title>AnimationMaker</title>\n";
    html << "<link rel=\"stylesheet\" href=\"assets/css/style.css\">\n";
    html << "</head>\n";
    html << "<body>\n";

    html << "<svg width=\"" + QString::number(scene->width()) + "\" height=\"" + QString::number(scene->height()) +"\">\n";
    html << "<rect id=\"scene\" x=\"0\" y=\"0\" width=\"" + QString::number(scene->width()) + "\" height=\"" + QString::number(scene->height()) + "\" fill=\"" + scene->backgroundRect()->brushColor().name() + "\"/>\n";

    QTextStream js(&jsFile);
    js.setCodec("UTF-8");
    js << "window.onload = function(){\n";

    for(int i = 0; i < itemList.count(); i++)
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(itemList.at(i));
        if(item)
        {
            if(!item->isSceneRect())
            {
                Rectangle *rect = dynamic_cast<Rectangle*>(item);
                if(rect)
                {
                    int x = rect->x();
                    int y = rect->y();
                    html << "<rect ";
                    html << "id=\"" + rect->id() + QString::number(i) + "\" ";
                    QString var = "";

                    foreach(QString property, rect->keyframes()->keys())
                    {
                        KeyFrame *from = rect->keyframes()->value(property);

                        if(property == "left")
                        {
                            x = from->value().toInt();
                            var = "x";
                        }
                        else if(property == "top")
                        {
                            y = from->value().toInt();
                            var = "y";
                        }

                        if(from->easing() >= 0)
                        {
                            KeyFrame *to = from->next();
                            js << "TweenLite.to(\"#" + rect->id() + QString::number(i) + "\", ";
                            js << QString::number(double(to->time() - from->time())/1000.0);
                            js << ", {attr:{";
                            js << var;
                            js << ":";
                            js << QString::number(to->value().toInt());
                            js << "}, delay: ";
                            js << QString::number((double)from->time() / 1000.0);
                            switch(from->easing())
                            {
                                default:
                                case 0:
                                    js << ", ease:Linear.easeNone";
                                    break;
                                case 1:
                                    js << ", ease:Quad.easeIn";
                                    break;
                                    // TODO EASING
                            }
                            js << "});\n";
                        }
                    }
                    //TweenLite.to("#logo", 2, {left:600, ease:Bounce.easeOut, delay: 1});
                    //TweenLite.to("#circle", 2, {x:700, attr:{r:10}, delay: 1});

                    html << "x=\"" + QString::number(x) + "\" ";
                    html << "y=\"" + QString::number(y) + "\" ";
                    html << "width=\"" + QString::number(rect->width()) + "\" ";
                    html << "height=\"" + QString::number(rect->height()) + "\" ";
                    html << "fill=\"" + rect->brushColor().name() + "\" ";
                    html << "stroke=\"" + rect->penColor().name() + "\" ";
                    html << "stroke-width=\"1\" ";
                    html << "/>\n";
                }
            }
        }
    }
    js << "}\n";
    jsFile.close();

    html << "</svg>\n";
    html << "<script src='assets/js/CSSPlugin.min.js'></script>\n";
    html << "<script src='assets/js/EasePack.min.js'></script>\n";
    html << "<script src='assets/js/TweenLite.min.js'></script>\n";
    html << "<script src='assets/js/AttrPlugin.min.js'></script>\n";
    html << "<script  src='assets/js/animationmaker.js'></script>\n";
    html << "</body>\n";
    html << "</html>\n";
    htmlFile.close();

    bar->showMessage("Animation has been exported to HTML");
}
