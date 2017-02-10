#ifndef EXPANDER_H
#define EXPANDER_H

#include <QWidget>
#include <QToolButton>

class Expander : public QWidget
{
    Q_OBJECT
public:
    Expander(QString header);

public slots:
    void buttonClicked(bool);

private :
    QWidget *m_content;
    QToolButton *m_button;
    qreal m_contentHeight;
    bool m_isExpanded;
};

#endif // EXPANDER_H
