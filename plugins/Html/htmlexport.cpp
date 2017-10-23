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
#include "bitmap.h"
#include "ellipse.h"
#include "text.h"
#include "vectorgraphic.h"
#include <QStatusBar>
#include <QFileDialog>
#include <QTest>
#include <QMessageBox>
#include <QBuffer>

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

QString HtmlExport::getTweens(QString tweenArray, AnimationItem *item, int i)
{
    QString rc = "";
    QTextStream js(&rc);
    QString var = "";
    QString value = "";
    int frameNumber = 0;
    bool isEllipse = false;
    QStringList properties;
    Ellipse *ellipse = dynamic_cast<Ellipse*>(item);
    if(ellipse)
        isEllipse = true;

    // fill in animateable properties in correct order (width before left, height before top)
    properties.append("width");
    properties.append("height");
    properties.append("left");
    properties.append("top");
    properties.append("opacity");
    properties.append("brushColor");
    properties.append("penColor");
    properties.append("textColor");

    foreach(QString property, properties)
    {
        if(item->keyframes()->contains(property))
        {
            for(KeyFrame *from = item->keyframes()->value(property); from != NULL; from = from->next())
            {
                if(property == "left")
                {
                    if(isEllipse)
                    {
                        value = QString::number((qreal)from->value().toReal() + item->width() / 2.0);
                        var = "cx";
                    }
                    else
                    {
                        value = QString::number(from->value().toInt());
                        var = "x";
                    }
                }
                else if(property == "top")
                {
                    if(isEllipse)
                    {
                        value = QString::number((qreal)from->value().toReal() + item->height() / 2.0);
                        var = "cy";
                    }
                    else
                    {
                        value = QString::number(from->value().toInt());
                        var = "y";
                    }
                }
                else if(property == "width")
                {
                    if(isEllipse)
                    {
                        value = QString::number(from->value().toReal() / 2.0);
                        var = "rx";
                        // save the actual width for cx calculation
                        item->setWidth(from->value().toReal());
                    }
                    else
                    {
                        value = QString::number(from->value().toInt());
                        var = "width";
                    }
                }
                else if(property == "height")
                {
                    if(isEllipse)
                    {
                        value = QString::number(from->value().toReal() / 2.0);
                        var = "ry";
                        // save the actual height for cy calculation
                        item->setHeight(from->value().toReal());
                    }
                    else
                    {
                        value = QString::number(from->value().toInt());
                        var = "height";
                    }
                }
                else if(property == "opacity")
                {
                    value = QString::number((qreal)from->value().toInt() / 100.0);
                    var = "opacity";
                }
                else if(property == "brushColor")
                {
                    value = '"' + from->value().toString() +'"';
                    var = "fill";
                }
                else if(property == "penColor")
                {
                    value = '"' + from->value().toString() + '"';
                    var = "stroke";
                }
                else if(property == "textColor")
                {
                    value = '"' + from->value().toString() + '"';
                    var = "fill";
                }
                else
                    qDebug() << "animation for attribute " + property + " not yet implemented";


                QString obj = "tween" + QString::number(i) + "_" + QString::number(frameNumber);
                if(!tweenArray.isEmpty())
                    tweenArray += ", ";
                tweenArray += obj;
                js << "var " + obj + " = TweenLite.to(\"#" + item->id() + QString::number(i) + "\", ";
                if(from->prev() && from->prev()->easing() >= 0)
                    js << QString::number(double(from->time() - from->prev()->time())/1000.0);
                else
                    js << "0";
                js << ", {attr:{";
                js << var;
                js << ":";
                js << value;
                js << "}, delay: ";
                if(from->prev() && from->prev()->easing() >= 0)
                {
                    js << QString::number((double)(from->prev()->time() / 1000.0));
                    js << ", " + getEaseString(from->easing());
                }
                else
                    js << QString::number((double)from->time() / 1000.0);
                js << "});\n";

                frameNumber++;
            }
        }
    }
    return rc;
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
                    html << "<rect ";
                    html << "id=\"" + rect->id() + QString::number(i) + "\" ";
                    html << "x=\"" + QString::number(rect->x()) + "\" ";
                    html << "y=\"" + QString::number(rect->y()) + "\" ";
                    html << "width=\"" + QString::number(rect->width()) + "\" ";
                    html << "height=\"" + QString::number(rect->height()) + "\" ";
                    html << "fill=\"" + rect->brushColor().name() + "\" ";
                    html << "stroke=\"" + rect->penColor().name() + "\" ";
                    html << "stroke-width=\"1\" ";
                    html << "opacity=\"" + QString::number((qreal)rect->opacity() / 100.0) + "\" ";
                    html << "/>\n";
                }

                Ellipse *ellipse = dynamic_cast<Ellipse*>(item);
                if(ellipse)
                {
                    html << "<ellipse ";
                    html << "id=\"" + ellipse->id() + QString::number(i) + "\" ";
                    html << "cx=\"" + QString::number((qreal)ellipse->x() + ellipse->width() / 2.0) + "\" ";
                    html << "cy=\"" + QString::number((qreal)ellipse->y() + ellipse->height() / 2.0) + "\" ";
                    html << "rx=\"" + QString::number((qreal)ellipse->width() / 2.0) + "\" ";
                    html << "ry=\"" + QString::number((qreal)ellipse->height() / 2.0) + "\" ";
                    html << "fill=\"" + ellipse->brushColor().name() + "\" ";
                    html << "stroke=\"" + ellipse->penColor().name() + "\" ";
                    html << "stroke-width=\"1\" ";
                    html << "opacity=\"" + QString::number((qreal)ellipse->opacity() / 100.0) + "\" ";
                    html << "/>\n";
                }

                Text *text = dynamic_cast<Text*>(item);
                if(text)
                {
                    html << "<svg id=\"" + text->id() + QString::number(i) + "\" ";
                    html << "x=\"" + QString::number(text->x()) + "\" ";
                    html << "y=\"" + QString::number(text->y()) + "\" ";
                    html << "opacity=\"" + QString::number((double)text->opacity() / 100.0) + "\" ";
                    html << ">";
                    html << text->getTextTag();
                    html << "</svg>";
                }

                Bitmap *bitmap = dynamic_cast<Bitmap*>(item);
                if(bitmap)
                {
                    QByteArray byteArray;
                    QBuffer buffer(&byteArray);
                    bitmap->getImage().save(&buffer, "PNG");

                    html << "<image ";
                    html << "id=\"" + bitmap->id() + QString::number(i) + "\" ";
                    html << "x=\"" + QString::number(bitmap->x()) + "\" ";
                    html << "y=\"" + QString::number(bitmap->y()) + "\" ";
                    html << "width=\"" + QString::number(bitmap->width()) + "\" ";
                    html << "height=\"" + QString::number(bitmap->height()) + "\" ";
                    html << "xlink:href=\"data:image/png;base64," + QString::fromLatin1(byteArray.toBase64().data()) + "\"";
                    html << "opacity=\"" + QString::number((qreal)bitmap->opacity() / 100.0) + "\" ";
                    html << "/>\n";
                }

                Vectorgraphic *vg = dynamic_cast<Vectorgraphic*>(item);
                if(vg)
                {
                    html << "<svg id=\"" + vg->id() + QString::number(i) + "\" ";
                    html << "x=\"" + QString::number(vg->x()) + "\" ";
                    html << "y=\"" + QString::number(vg->y()) + "\" ";
                    html << "opacity=\"" + QString::number((double)vg->opacity() / 100.0) + "\" ";
                    html << ">";
                    html << vg->getInnerSvg();
                    html << "</svg>";
                }
                js << getTweens(tweenArray, item, i);
            }
        }
    }
    if(!tweenArray.isEmpty())
    {
        js << "var tl = new TimelineLite();\n";
        js << "tl.add([" + tweenArray + "]);\n";
        js << "tl.play();\n";
    }
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


