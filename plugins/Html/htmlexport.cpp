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
#include "installer.h"
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
#include <QDesktopServices>

QString cleanId(QString id)
{
    return id.replace(" ", "_").replace("!","_").replace(".", "_");
}

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

QString HtmlExport::getTweens(QString &tweenArray, AnimationItem *item, int i, QStringList properties)
{
    QString rc = "";
    QTextStream js(&rc);
    QString var = "";
    QString value = "";
    QString id = "";
    QString idAdd = "";

    bool isEllipse = false;
    Ellipse *ellipse = dynamic_cast<Ellipse*>(item);
    if(ellipse)
        isEllipse = true;

    foreach(QString property, properties)
    {
        if(item->keyframes()->contains(property))
        {
            int frameNumber = 0;
            for(KeyFrame *from = item->keyframes()->value(property); from != nullptr; from = from->next())
            {
                id = item->id() + QString::number(i);
                idAdd = "";
                if(property == "left")
                {
                    value = QString::number(from->value().toInt());
                    var = "left: " + value;
                    idAdd = "_pos";
                }
                else if(property == "top")
                {
                    value = QString::number(from->value().toInt());
                    var = "top: " + value;
                    idAdd = "_pos";
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
                    var = "opacity: " + value;
                    idAdd = "_pos";
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
                    id = item->id() + QString::number(i);
                    value = '"' + from->value().toString() + '"';
                    var = "fill";
                }
                else if(property.contains(".")) // svg animation like path.fill
                {
                    int dot = property.indexOf(".");
                    id = property.left(dot);
                    value = '"' + from->value().toString() + '"';
                    var = '"' + property.mid(dot + 1) + '"';
                }
                else if(property == "rotationx")
                {
                    idAdd = "_x";
                    value = from->value().toString();
                    var = "rotationX: " + value + ", transformOrigin: \"" + QString::number(item->originX()) + "px " + QString::number(item->originY()) + "px\"";
                }
                else if(property == "rotationy")
                {
                    idAdd = "_y";
                    value = from->value().toString();
                    var = "rotationY: -" + value + ", transformOrigin: \"" + QString::number(item->originX()) + "px " + QString::number(item->originY()) + "px\"";
                }
                else if(property == "rotationz")
                {
                    idAdd = "_z";
                    value = from->value().toString();
                    var = "rotationZ: " + value + ", transformOrigin: \"" + QString::number(item->originX()) + "px " + QString::number(item->originY()) + "px\"";
                }
                else if(property == "shearx")
                {
                    idAdd = "_pos";
                    value = QString::number(from->value().toReal() * 50);
                    var = "skewX: " + value + ", transformOrigin: \"" + QString::number(item->originX()) + "px " + QString::number(item->originY()) + "px\"";
                }
                else if(property == "sheary")
                {
                    idAdd = "_pos";
                    value = QString::number(from->value().toReal() * 50);
                    var = "skewY: " + value + ", transformOrigin: \"" + QString::number(item->originX()) + "px " + QString::number(item->originY()) + "px\"";
                }
                else
                    qDebug() << "animation for attribute " + property + " not yet implemented";

                if(from->prev() && from->prev()->easing() >= 0)
                {
                    QString obj = "tween" + QString::number(i) + "_" + QString::number(frameNumber) + "_" + property.replace(".", "_");
                    if(!tweenArray.isEmpty())
                        tweenArray += ", ";
                    tweenArray += obj;
                    js << "var " + obj + " = TweenLite.to(\"#" + cleanId(id);
                    js << idAdd;
                    js << "\", ";
                    js << QString::number(double(from->time() - from->prev()->time())/1000.0);
                    js << ", {";
                    if(var.contains(":"))
                    {
                        js << var;
                    }
                    else
                    {
                        js << "attr:{";
                        js << var;
                        js << ":";
                        js << value;
                        js << "}";
                    }
                    js << ", delay: ";
                    if(from->prev() && from->prev()->easing() >= 0)
                    {
                        js << QString::number((double)(from->prev()->time() / 1000.0));
                        js << ", " + getEaseString(from->prev()->easing());
                    }
                    else
                        js << QString::number((double)from->time() / 1000.0);
                    js << "});\n";
                }
                else
                {
                    js << "TweenLite.set(\"#" + cleanId(id);
                    js << idAdd;
                    js << "\", {";
                    if(var.contains(":"))
                    {
                        js << var;
                    }
                    else
                    {
                        js << "attr:{";
                        js << var;
                        js << ":";
                        js << value;
                        js << "}";
                    }
                    js << ", delay: ";
                    js << QString::number((double)from->time() / 1000.0);
                    js << "});\n";
                }
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
    dir.mkdir("js");
    dir.mkdir("images");
    dir.cdUp();

#ifdef DEBUG
    Installer::installFiles("/home/olaf/SourceCode/AnimationMaker/plugins/Html/gsap", dir.absolutePath() + "/assets/js", true, false);
#else
    Installer::installFiles(QCoreApplication::applicationDirPath() + "/../../plugins/gsap", dir.absolutePath() + "/assets/js", true, false);
#endif
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
    html << "<body style=\"margin:0;padding:0\">\n";
    html << "<div style=\"position: absolute\">\n";
    html << "<svg width=\"" + QString::number(scene->width()) + "\" height=\"" + QString::number(scene->height()) +"\">\n";
    html << "<rect id=\"scene\" x=\"0\" y=\"0\" width=\"" + QString::number(scene->width()) + "\" height=\"" + QString::number(scene->height()) + "\" fill=\"" + scene->backgroundRect()->brushColor().name() + "\"/>\n";
    html << "</svg>\n";
    html << "</div>\n";

    QTextStream js(&jsFile);
    js.setCodec("UTF-8");
    js << "window.onload = function(){\n";

    QString tweenArray = "";
    for(int i = 0; i < itemList.count(); i++)
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(itemList.at(i));
        if(item)
        {
            QStringList properties;
            // fill in properties in correct order (width before left and height before top)
            properties << "width";
            properties << "height";
            properties << "left";
            properties << "top";
            properties << "opacity";
            properties << "brushColor";
            properties << "penColor";
            properties << "textColor";
            properties << "rotationx";
            properties << "rotationy";
            properties << "rotationz";
            properties << "shearx";
            properties << "sheary";

            if(!item->isSceneRect())
            {
                html << "<div id=\"" + cleanId(item->id() + QString::number(i)) + "_pos\" style=\"left: " + QString::number(item->x()) + "px; top: " + QString::number(item->top()) + "px; position: absolute; opacity: " + QString::number((qreal)item->opacity() / 100.0) + "; ";
                html << "transform: ";
                html << "skewX(" + QString::number(item->shearX() * 100) + "deg) ";
                html << "skewY(" + QString::number(item->shearY() * 100) + "deg) ";
                html << "scale(" + QString::number(item->scaleX()) + "," + QString::number(item->scaleY()) + ")";
                html << ">\n";
                html << "<div id=\"" + cleanId(item->id() + QString::number(i)) + "_x\" style=\"transform: perspective(1000px) rotateX(" + QString::number(item->rotationX()) +"deg); transform-origin: " + QString::number(item->originX()) + "px " + QString::number(item->originY()) + "px;\">\n";
                html << "<div id=\"" + cleanId(item->id() + QString::number(i)) + "_y\" style=\"transform: perspective(1000px) rotateY(" + QString::number(item->rotationY() * -1) +"deg); transform-origin: " + QString::number(item->originX()) + "px " + QString::number(item->originY()) + "px;\">\n";
                html << "<div id=\"" + cleanId(item->id() + QString::number(i)) + "_z\" style=\"transform: perspective(1000px) rotateZ(" + QString::number(item->rotationZ()) +"deg); transform-origin: " + QString::number(item->originX()) + "px " + QString::number(item->originY()) + "px;\">\n";
                html << item->getHtml(cleanId(item->id() + QString::number(i)), fileName + "/assets");
                properties.append(item->getPropertyList());
                js << getTweens(tweenArray, item, i, properties);
                html << "</div>\n";
                html << "</div>\n";
                html << "</div>\n";
                html << "</div>\n";
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

    html << "<script src='assets/js/CSSPlugin.min.js'></script>\n";
    html << "<script src='assets/js/EasePack.min.js'></script>\n";
    html << "<script src='assets/js/TweenLite.min.js'></script>\n";
    html << "<script src='assets/js/TimelineLite.min.js'></script>\n";
    html << "<script src='assets/js/AttrPlugin.min.js'></script>\n";
    html << "<script src='assets/js/animationmaker.js'></script>\n";
    html << "</body>\n";
    html << "</html>\n";
    htmlFile.close();

    bar->showMessage("Animation has been exported to " + dir.absoluteFilePath("index.html"));
    QDesktopServices::openUrl(QUrl(dir.absoluteFilePath("index.html")));
}
