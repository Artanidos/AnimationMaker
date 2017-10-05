/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  FlatSiteBuilder is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlatSiteBuilder is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class XmlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum Construct
    {
        Entity,
        Element,
        Attribute,
        String,
        Comment,
        LastConstruct = Comment
    };

    XmlHighlighter(QTextDocument *document);

    void setFormatFor(Construct construct, const QTextCharFormat &format);
    QTextCharFormat formatFor(Construct construct) const
    {
        return m_formats[construct];
    }

protected:
    enum State
    {
        NormalState = -1,
        InComment,
        InElement,
        InString,
        InAttribute
    };

    void highlightBlock(const QString &text);

private:
    QTextCharFormat m_formats[LastConstruct + 1];
};

#endif // XMLHIGHLIGHTER_H
