#include "exception.h"

Exception::Exception(QString msg)
{
    m_msg = msg;
}
/*
void Exception::raise() const
{
    throw *this;
}

Exception *Exception::clone() const
{
    return new Exception(*this);
}
*/
