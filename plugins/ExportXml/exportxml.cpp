/****************************************************************************
** Copyright (C) 2016 Olaf Japp
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

#include "exportxml.h"
#include <QTest>
#include <QMessageBox>
#include <QBuffer>

ExportXml::ExportXml()
{
}

QString ExportXml::displayName() const
{
    return "XML";
}

QString ExportXml::filter() const
{
    return "XML format (*.xml);;All Files (*)";
}

QString ExportXml::title() const
{
    return "Export Animation to XML";
}

void ExportXml::writeKeyframes(QDomDocument *doc, QDomElement *element, AnimationMaker::AnimationItem *item)
{
    QHash<QString, AnimationMaker::Keyframe*>::iterator it;
    for(it = item->keyframes.begin(); it != item->keyframes.end(); it++)
    {
        QDomElement frames = doc->createElement("Keyframes");
        frames.setAttribute("property", it.key());
        for(AnimationMaker::Keyframe *frame = it.value(); frame != NULL; frame = frame->next)
        {
            QDomElement f = doc->createElement("Keyframe");
            f.setAttribute("time", frame->time);
            f.setAttribute("value", frame->value.toString());
            f.setAttribute("easing", frame->easing);
            frames.appendChild(f);
        }
        element->appendChild(frames);
    }
}
void ExportXml::exportMeta(QString filename, AnimationMaker::Animation *animation, bool exportAll, QStatusBar *bar)
{
    QDomDocument doc;
    QDomElement root;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0, "Error", "Unable to open file " + filename);
        return;
    }
    bar->showMessage("Writing to file " + filename);

    if(exportAll)
    {
        root = doc.createElement("Animation");
        root.setAttribute("fps", animation->fps);
        root.setAttribute("width", animation->width);
        root.setAttribute("height", animation->height);
        doc.appendChild(root);
    }
    else
    {
        root = doc.createElement("AnimationItems");
        doc.appendChild(root);
    }

    for(int i=0; i < animation->items.count(); i++)
    {
        AnimationMaker::AnimationItem *item = animation->items.at(i);
        AnimationMaker::Rectangle *r = dynamic_cast<AnimationMaker::Rectangle*>(item);
        if(r)
        {
            QDomElement rect = doc.createElement("Rectangle");
            rect.setAttribute("id", r->id);
            rect.setAttribute("left", QVariant(r->left).toString());
            rect.setAttribute("top", QVariant(r->top).toString());
            rect.setAttribute("width", QVariant(r->width).toString());
            rect.setAttribute("height", QVariant(r->height).toString());
            rect.setAttribute("pen", r->pen);
            rect.setAttribute("brush", r->brush);
            rect.setAttribute("opacity", r->opacity);
            writeKeyframes(&doc, &rect, r);
            root.appendChild(rect);
        }
        AnimationMaker::Ellipse *e = dynamic_cast<AnimationMaker::Ellipse*>(item);
        if(e)
        {
            QDomElement ellipse = doc.createElement("Ellipse");
            ellipse.setAttribute("id", e->id);
            ellipse.setAttribute("left", QVariant(e->left).toString());
            ellipse.setAttribute("top", QVariant(e->top).toString());
            ellipse.setAttribute("width", QVariant(e->width).toString());
            ellipse.setAttribute("height", QVariant(e->height).toString());
            ellipse.setAttribute("pen", e->pen);
            ellipse.setAttribute("brush", e->brush);
            ellipse.setAttribute("opacity", e->opacity);
            writeKeyframes(&doc, &ellipse, e);
            root.appendChild(ellipse);
        }
        AnimationMaker::Text *t = dynamic_cast<AnimationMaker::Text*>(item);
        if(t)
        {
            QDomElement text = doc.createElement("Text");
            text.setAttribute("id", t->id);
            text.setAttribute("left", QVariant(t->left).toString());
            text.setAttribute("top", QVariant(t->top).toString());
            text.setAttribute("xscale", QVariant(t->xscale).toString());
            text.setAttribute("yscale", QVariant(t->yscale).toString());
            text.setAttribute("text", t->text);
            text.setAttribute("textcolor", t->textcolor);
            text.setAttribute("opacity", t->opacity);
            text.setAttribute("font-family", t->fontFamily);
            text.setAttribute("font-size", t->fontSize);
            text.setAttribute("font-style", t->fontStyle);
            writeKeyframes(&doc, &text, t);
            root.appendChild(text);
        }
        AnimationMaker::Bitmap *b = dynamic_cast<AnimationMaker::Bitmap*>(item);
        if(b)
        {
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            b->image.save(&buffer, "PNG");
            QDomElement bitmap = doc.createElement("Bitmap");
            bitmap.setAttribute("id", b->id);
            bitmap.setAttribute("left", QVariant(b->left).toString());
            bitmap.setAttribute("top", QVariant(b->top).toString());
            bitmap.setAttribute("width", QVariant(b->width).toString());
            bitmap.setAttribute("height", QVariant(b->height).toString());
            bitmap.setAttribute("opacity", b->opacity);
            bitmap.appendChild(doc.createCDATASection(QString::fromLatin1(byteArray.toBase64().data())));
            writeKeyframes(&doc, &bitmap, b);
            root.appendChild(bitmap);
        }
        AnimationMaker::Vectorgraphic *v = dynamic_cast<AnimationMaker::Vectorgraphic*>(item);
        if(v)
        {
            QDomElement vectorgraphic = doc.createElement("Vectorgraphic");
            vectorgraphic.setAttribute("id", v->id);
            vectorgraphic.setAttribute("left", QVariant(v->left).toString());
            vectorgraphic.setAttribute("top", QVariant(v->top).toString());
            vectorgraphic.setAttribute("xscale", QVariant(v->xscale).toString());
            vectorgraphic.setAttribute("yscale", QVariant(v->yscale).toString());
            vectorgraphic.setAttribute("opacity", v->opacity);
            vectorgraphic.appendChild(doc.createCDATASection(QString::fromLatin1(v->data)));
            writeKeyframes(&doc, &vectorgraphic, v);
            root.appendChild(vectorgraphic);
        }
    }
    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
    bar->showMessage(QString("Ready"));
}
