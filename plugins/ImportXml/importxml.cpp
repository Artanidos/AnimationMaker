#include "importxml.h"
#include <QTest>
#include <QMessageBox>
#include <QBuffer>

ImportXml::ImportXml()
{
}

QString ImportXml::displayName() const
{
    return "XML";
}

QString ImportXml::filter() const
{
    return "XML format (*.xml);;All Files (*)";
}

QString ImportXml::title() const
{
    return "Import XML";
}

void ImportXml::importMeta(QString filename, AnimationMaker::Animation *animation, QStatusBar *bar)
{   
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0, "Error", "Unable to open file " + filename);
        bar->showMessage("Unable to open file " + filename);
        return;
    }
    bar->showMessage("Reading from file " + filename);
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        QMessageBox::warning(0, "Error", "Unable to read file " + filename);
        file.close();
        bar->showMessage("Unable to read file " + filename);
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    if(docElem.nodeName() == "Animation")
    {
        animation->fps = docElem.attribute("fps", "24").toInt();
        animation->width = docElem.attribute("width", "1200").toInt();
        animation->height = docElem.attribute("height", "720").toInt();
    }
    for(int i=0; i < docElem.childNodes().count(); i++)
    {
        QDomNode node =docElem.childNodes().at(i);
        if(node.nodeName() == "Rectangle")
        {
            QDomElement ele = node.toElement();
            AnimationMaker::Rectangle *r = new AnimationMaker::Rectangle();
            r->id = ele.attribute("id", "Rectangle");
            r->width = ele.attribute("width", "50").toDouble();
            r->height = ele.attribute("height", "50").toDouble();
            r->left = ele.attribute("left", "0").toDouble();
            r->top = ele.attribute("top", "0").toDouble();
            r->pen = ele.attribute("pen", "#000000");
            r->brush = ele.attribute("brush", "#0000FF");
            r->opacity = ele.attribute("opacity", "100").toInt();
            readKeyframes(&ele, r);
            animation->items.append(r);
        }
        else if(node.nodeName() == "Ellipse")
        {
            QDomElement ele = node.toElement();
            AnimationMaker::Ellipse *e = new AnimationMaker::Ellipse();
            e->id = ele.attribute("id", "Ellipse");
            e->width = ele.attribute("width", "50").toDouble();
            e->height = ele.attribute("height", "50").toDouble();
            e->left = ele.attribute("left", "0").toDouble();
            e->top = ele.attribute("top", "0").toDouble();
            e->pen = ele.attribute("pen", "#000000");
            e->brush = ele.attribute("brush", "#0000FF");
            e->opacity = ele.attribute("opacity", "100").toInt();
            readKeyframes(&ele, e);
            animation->items.append(e);
        }
        else if(node.nodeName() == "Text")
        {
            QDomElement ele = node.toElement();
            AnimationMaker::Text *t = new AnimationMaker::Text();
            t->text = ele.attribute("text");
            t->id = ele.attribute("id", "Text");
            t->left = ele.attribute("left", "0").toDouble();
            t->top = ele.attribute("top", "0").toDouble();
            t->xscale = ele.attribute("xscale", "1").toDouble();
            t->yscale = ele.attribute("yscale", "1").toDouble();
            t->textcolor = ele.attribute("textcolor", "#000000");
            t->opacity = ele.attribute("opacity", "100").toInt();
            t->fontFamily = ele.attribute("font-family");
            t->fontSize = ele.attribute("font-size").toInt();
            t->fontStyle = ele.attribute("font-style");
            readKeyframes(&ele, t);
            animation->items.append(t);
        }
        else if(node.nodeName() == "Bitmap")
        {
            QDomElement ele = node.toElement();
            QDomNode data = ele.firstChild();
            QDomCDATASection cdata = data.toCDATASection();
            QImage img = QImage::fromData(QByteArray::fromBase64(cdata.data().toLatin1()), "PNG");
            AnimationMaker::Bitmap *b = new AnimationMaker::Bitmap();
            b->image = img;
            b->width = ele.attribute("width", "50").toDouble();
            b->height = ele.attribute("height", "50").toDouble();
            b->id = ele.attribute("id", "Bitmap");
            b->left = ele.attribute("left", "0").toDouble();
            b->top = ele.attribute("top", "0").toDouble();
            b->opacity = ele.attribute("opacity", "100").toInt();
            readKeyframes(&ele, b);
            animation->items.append(b);
        }
        else if(node.nodeName() == "Vectorgraphic")
        {
            QDomElement ele = node.toElement();
            QDomNode data = ele.firstChild();
            QDomCDATASection cdata = data.toCDATASection();
            AnimationMaker::Vectorgraphic *v = new AnimationMaker::Vectorgraphic();
            v->data = cdata.data().toLatin1();
            v->id = ele.attribute("id", "Vectorgraphic");
            v->left = ele.attribute("left", "0").toDouble();
            v->top = ele.attribute("top", "0").toDouble();
            v->xscale = ele.attribute("xscale", "1").toDouble();
            v->yscale = ele.attribute("yscale", "1").toDouble();
            v->opacity = ele.attribute("opacity", "100").toInt();
            readKeyframes(&ele, v);
            animation->items.append(v);
        }
    }

    bar->showMessage(QString("Ready"));
}

void ImportXml::readKeyframes(QDomElement *element, AnimationMaker::AnimationItem *item)
{
    AnimationMaker::Keyframe *m_tempKeyFrame = NULL;
    for(int i=0; i < element->childNodes().count(); i++)
    {
        QDomNode node = element->childNodes().at(i);
        if(node.nodeName() == "Keyframes")
        {
            QDomElement keyframes = node.toElement();
            for(int j=0; j < node.childNodes().count(); j++)
            {
                QDomNode frameNode = node.childNodes().at(j);
                if(frameNode.nodeName() == "Keyframe")
                {
                    QDomElement keyframe = frameNode.toElement();
                    AnimationMaker::Keyframe *key = new AnimationMaker::Keyframe();
                    key->time = keyframe.attribute("time", "0").toInt();
                    key->value = keyframe.attribute("value");
                    key->easing = keyframe.attribute("easing", "-1").toInt();
                    // set double linked list
                    if(m_tempKeyFrame)
                    {
                        m_tempKeyFrame->next = key;
                        key->prev = m_tempKeyFrame;
                    }
                    else
                        item->keyframes.insert(keyframes.attribute("property"), key);
                    m_tempKeyFrame = key;
                }
            }
            m_tempKeyFrame = NULL;
        }
    }
}
