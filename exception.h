#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QException>

class Exception : QException
{
public:
    Exception(QString msg);

    QString msg() {return m_msg;}

private:
    QString m_msg;
};

#endif // EXCEPTION_H
