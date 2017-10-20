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

QString getEaseString(int easing)
{
    switch(easing)
    {
        default:
        case QEasingCurve::Linear:
            return "ease:Linear.easeNone";
        case QEasingCurve::InQuad:
            return "ease:Quad.easeIn";
        case QEasingCurve::OutQuad:
            return "ease:Quad.easeOut";
        case QEasingCurve::InOutQuad:
            return "ease:Quad.easeInOut";
        case QEasingCurve::OutInQuad:
            return "ease:Quad.easeOutIn";
        case QEasingCurve::InCubic:
            return "ease:Cubic.easeIn";
        case QEasingCurve::OutCubic:
            return "ease:Cubic.easeOut";
        case QEasingCurve::InOutCubic:
            return "ease:Cubic.easeInOut";
        case QEasingCurve::OutInCubic:
            return "ease:Cubic.easeOutIn";
        case QEasingCurve::InQuart:
            return "ease:Quart.easeIn";
        case QEasingCurve::OutQuart:
            return "ease:Quart.easeOut";
        case QEasingCurve::InOutQuart:
            return "ease:Quart.easeInOut";
        case QEasingCurve::OutInQuart:
            return "ease:Quart.easeOutIn";
        case QEasingCurve::InQuint:
            return "ease:Quint.easeIn";
        case QEasingCurve::OutQuint:
            return "ease:Quint.easeOut";
        case QEasingCurve::InOutQuint:
            return "ease:Quint.easeInOut";
        case QEasingCurve::OutInQuint:
            return "ease:Quint.easeOutIn";
        case QEasingCurve::InSine:
            return "ease:Sine.easeIn";
        case QEasingCurve::OutSine:
            return "ease:Sine.easeOut";
        case QEasingCurve::InOutSine:
            return "ease:Sine.easeInOut";
        case QEasingCurve::OutInSine:
            return "ease:Sine.easeOutIn";
        case QEasingCurve::InExpo:
            return "ease:Expo.easeIn";
        case QEasingCurve::OutExpo:
            return "ease:Expo.easeOut";
        case QEasingCurve::InOutExpo:
            return "ease:Expo.easeInOut";
        case QEasingCurve::OutInExpo:
            return "ease:Expo.easeOutIn";
        case QEasingCurve::InCirc:
            return "ease:Circ.easeIn";
        case QEasingCurve::OutCirc:
            return "ease:Circ.easeOut";
        case QEasingCurve::InOutCirc:
            return "ease:Circ.easeInOut";
        case QEasingCurve::OutInCirc:
            return "ease:Circ.easeOutIn";
        case QEasingCurve::InBack:
            return "ease:Back.easeIn";
        case QEasingCurve::OutBack:
            return "ease:Back.easeOut";
        case QEasingCurve::InOutBack:
            return "ease:Back.easeInOut";
        case QEasingCurve::OutInBack:
            return "ease:Back.easeOutIn";
        case QEasingCurve::InElastic:
            return "ease:Elastic.easeIn";
        case QEasingCurve::OutElastic:
            return "ease:Elastic.easeOut";
        case QEasingCurve::InOutElastic:
            return "ease:Elastic.easeInOut";
        case QEasingCurve::OutInElastic:
            return "ease:Elastic.easeOutIn";
        case QEasingCurve::InBounce:
            return "ease:Bounce.easeIn";
        case QEasingCurve::OutBounce:
            return "ease:Bounce.easeOut";
        case QEasingCurve::InOutBounce:
            return "ease:Bounce.easeInOut";
        case QEasingCurve::OutInBounce:
            return "ease:Bounce.easeOutIn";
    }
}

void HtmlExport::exportAnimation(AnimationScene *scene, QStatusBar *bar)
{
    QList<QGraphicsItem*> itemList;
    scene->setPlayheadPosition(0);

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

    QString tweenArray = "";
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
                    int width = rect->width();
                    int height = rect->height();
                    qreal opacity = (qreal)rect->opacity() / 100.0;
                    QString brushColor = rect->brushColor().name();

                    html << "<rect ";
                    html << "id=\"" + rect->id() + QString::number(i) + "\" ";
                    QString var = "";
                    QString toValue = "";
                    int frameNumber = 0;
                    foreach(QString property, rect->keyframes()->keys())
                    {
                        KeyFrame *from = rect->keyframes()->value(property);

                        if(property == "left")
                        {
                            x = from->value().toInt();
                            var = "x";
                            if(from->easing() >= 0)
                                toValue = QString::number(from->next()->value().toInt());
                        }
                        else if(property == "top")
                        {
                            y = from->value().toInt();
                            var = "y";
                            if(from->easing() >= 0)
                                toValue = QString::number(from->next()->value().toInt());
                        }
                        else if(property == "width")
                        {
                            width = from->value().toInt();
                            var = "width";
                            if(from->easing() >= 0)
                                toValue = QString::number(from->next()->value().toInt());
                        }
                        else if(property == "height")
                        {
                            height = from->value().toInt();
                            var = "height";
                            if(from->easing() >= 0)
                                toValue = QString::number(from->next()->value().toInt());
                        }
                        else if(property == "opacity")
                        {
                            opacity = (qreal)from->value().toInt() / 100.0;
                            var = "opacity";
                            if(from->easing() >= 0)
                                toValue = QString::number((qreal)from->next()->value().toInt() / 100.0);
                        }
                        else if(property == "brushColor")
                        {
                            brushColor = from->value().toString();
                            var = "fill";
                            if(from->easing() >= 0)
                                toValue = '"' + from->next()->value().toString() + '"';
                        }
                        else if(property == "penColor")
                        {
                            brushColor = from->value().toString();
                            var = "stroke";
                            if(from->easing() >= 0)
                                toValue = '"' + from->next()->value().toString() + '"';
                        }
                        else
                            qDebug() << "animation for attribute " + property + " not yet implemented";


                        if(from->easing() >= 0)
                        {
                            QString obj = "tween" + QString::number(i) + "_" + QString::number(frameNumber);
                            if(!tweenArray.isEmpty())
                                tweenArray += ", ";
                            tweenArray += obj;
                            KeyFrame *to = from->next();
                            js << "var " + obj + " = TweenLite.to(\"#" + rect->id() + QString::number(i) + "\", ";
                            js << QString::number(double(to->time() - from->time())/1000.0);
                            js << ", {attr:{";
                            js << var;
                            js << ":";
                            js << toValue;
                            js << "}, delay: ";
                            js << QString::number((double)from->time() / 1000.0);
                            js << ", " + getEaseString(from->easing());
                            js << "});\n";
                        }
                        frameNumber++;
                    }

                    html << "x=\"" + QString::number(x) + "\" ";
                    html << "y=\"" + QString::number(y) + "\" ";
                    html << "width=\"" + QString::number(width) + "\" ";
                    html << "height=\"" + QString::number(height) + "\" ";
                    html << "fill=\"" + rect->brushColor().name() + "\" ";
                    html << "stroke=\"" + rect->penColor().name() + "\" ";
                    html << "stroke-width=\"1\" ";
                    html << "opacity=\"" + QString::number((qreal)rect->opacity() / 100.0) + "\" ";
                    html << "/>\n";
                }
            }
        }
    }
    js << "var tl = new TimelineLite();\n";
    js << "tl.add([" + tweenArray + "]);\n";
    js << "tl.play();\n";
    js << "}\n";
    jsFile.close();

    html << "</svg>\n";
    html << "<script src='assets/js/CSSPlugin.min.js'></script>\n";
    html << "<script src='assets/js/EasePack.min.js'></script>\n";
    html << "<script src='assets/js/TweenLite.min.js'></script>\n";
    html << "<script src='assets/js/TimelineLite.min.js'></script>\n";
    html << "<script src='assets/js/AttrPlugin.min.js'></script>\n";
    html << "<script  src='assets/js/animationmaker.js'></script>\n";
    html << "</body>\n";
    html << "</html>\n";
    htmlFile.close();

    bar->showMessage("Animation has been exported to HTML");
}


