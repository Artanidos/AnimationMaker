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

#include "bitmap.h"
#include "commands.h"
#include <QtTest/QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QBuffer>
#include <QUndoCommand>
#include <QFileDialog>
#include <QMenu>

Bitmap::Bitmap()
    : AnimationItem(NULL)
{
    addMenu();
}

Bitmap::Bitmap(QString filename, AnimationScene *scene)
    : AnimationItem(scene)
{
    m_image.load(filename);
    setRect(0, 0, m_image.width(), m_image.height());
    addMenu();
}

Bitmap::Bitmap(QImage image, AnimationScene *scene)
    :AnimationItem(scene)
{
    m_image = image;
    setRect(0, 0, 0, 0);
    addMenu();
}

void Bitmap::addMenu()
{
    QAction *exchangeBitmapAct = new QAction("Exchange Bitmap");
    connect(exchangeBitmapAct, SIGNAL(triggered()), this, SLOT(exchangeBitmapAction()));
    m_contextMenu->addAction(exchangeBitmapAct);
}

void Bitmap::exchangeBitmapAction()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter("Image Files (*.png *.jpeg *.jpg *.gif *.bmp);;All Files (*)");
    dialog->setWindowTitle("Open Bitmap");
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if(fileName.isEmpty())
        return;

    QUndoCommand *cmd = new ExchangeBitmapCommand(this, fileName);
    m_scene->undoStack()->push(cmd);
}

QDomElement Bitmap::getXml(QDomDocument doc)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    getImage().save(&buffer, "PNG");
    QDomElement ele = doc.createElement("Bitmap");
    writeAttributes(ele);
    ele.appendChild(doc.createCDATASection(QString::fromLatin1(byteArray.toBase64().data())));
    return ele;
}

void Bitmap::setImage(QImage image)
{
    m_image = image;
}

int Bitmap::type() const
{
    return Bitmap::Type;
}

void Bitmap::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    paint->drawImage(0, 0, m_image.scaled(rect().width(), rect().height()));

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

QImage Bitmap::getImage()
{
    return m_image;
}
