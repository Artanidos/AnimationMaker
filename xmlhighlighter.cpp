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

#include "xmlhighlighter.h"

XmlHighlighter::XmlHighlighter(QTextDocument *document)
    : QSyntaxHighlighter(document)
{
    QTextCharFormat entityFormat;
    entityFormat.setForeground(QColor(0, 128, 0));
    entityFormat.setFontWeight(QFont::Normal);
    setFormatFor(Entity, entityFormat);

    QTextCharFormat elementFormat;
    elementFormat.setForeground(QColor("#f0e68c"));
    elementFormat.setFontWeight(QFont::Normal);
    setFormatFor(Element, elementFormat);

    QTextCharFormat attributeFormat;
    attributeFormat.setForeground(QColor("#1ca8b4"));
    attributeFormat.setFontWeight(QFont::Normal);
    setFormatFor(Attribute, attributeFormat);

    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor("#12ce00"));
    stringFormat.setFontWeight(QFont::Normal);
    setFormatFor(String, stringFormat);

    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor("#87ceeb"));
    commentFormat.setFontItalic(true);
    setFormatFor(Comment, commentFormat);
}

void XmlHighlighter::setFormatFor(Construct construct, const QTextCharFormat &format)
{
    m_formats[construct] = format;
    rehighlight();
}


void XmlHighlighter::highlightBlock(const QString &text)
{
    int state = previousBlockState();
    int len = text.length();
    int start = 0;
    int pos = 0;
    QChar quote = QChar::Null;

    while (pos < len)
    {
        switch (state)
        {
            case NormalState:
            default:
            {
                while (pos < len)
                {
                    QChar ch = text.at(pos);
                    if (ch == '<')
                    {
                        if (text.mid(pos, 4) == "<!--")
                        {
                            state = InComment;
                        }
                        else
                        {
                            state = InElement;
                        }
                        break;
                    }
                    else if (ch == '&')
                    {
                        start = pos;
                        while (pos < len && text.at(pos++) != ';')
                            ;
                        setFormat(start, pos - start, m_formats[Entity]);
                    }
                    else
                    {
                        pos++;
                    }
                }
                break;
            }
            case InComment:
            {
                start = pos;
                while (pos < len)
                {
                    if (text.mid(pos, 3) == "-->")
                    {
                        pos += 3;
                        state = NormalState;
                        break;
                    }
                    else
                    {
                        pos++;
                    }
                }
                setFormat(start, pos - start, m_formats[Comment]);
                break;
            }
            case InElement:
            {
                start = pos;
                while (pos < len)
                {
                    QChar ch = text.at(pos);
                    if (ch == ' ')
                    {
                        pos++;
                        state = InAttribute;
                        break;
                    }
                    else if (ch == '>')
                    {
                        pos++;
                        state = NormalState;
                        break;
                    }
                    else if(ch == "\"" || ch == '"')
                    {
                        quote = ch;
                        pos++;
                        state = InString;
                        break;
                    }
                    pos++;
                }
                setFormat(start, pos - start, m_formats[Element]);
                break;
            }

            case InAttribute:
            {
                start = pos;
                while (pos < len)
                {
                    QChar ch = text.at(pos);
                    if(ch == '>')
                    {
                        pos++;
                        state = NormalState;
                        break;
                    }
                    else if(ch == "\"" || ch == '"')
                    {
                        quote = ch;
                        pos++;
                        state = InString;
                        break;
                    }
                    else if(ch == "=")
                    {
                        state = InElement;
                        break;
                    }
                    pos++;
                }
                setFormat(start, pos - start, m_formats[Attribute]);
                break;
            }
            case InString:
            {
                start = pos;
                while (pos < len)
                {
                    QChar ch = text.at(pos);
                    if(ch == quote)
                    {
                        quote = QChar::Null;
                        state = InElement;
                        pos++;
                        break;
                    }
                    pos++;
                }
                setFormat(start - 1, pos - start + 1, m_formats[String]);
                break;
            }
        }
    }

    setCurrentBlockState(state);
}
