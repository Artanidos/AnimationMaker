#ifndef EXPANDER_H
#define EXPANDER_H

#include <QLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QScrollArea>

class Expander : public QWidget
{
    Q_OBJECT
public:
    Expander(QString header);

    void addLayout(QLayout *l);

public slots:
    void buttonClicked(bool);

private :
    QWidget *m_content;
    QToolButton *m_button;
    bool m_isExpanded;
};

#endif // EXPANDER_H
