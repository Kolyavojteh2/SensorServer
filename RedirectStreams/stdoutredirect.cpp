#include "stdoutredirect.h"
#include <iostream>
#include <QDateTime>

StdOutRedirect::StdOutRedirect(QTextEdit *textEdit) : textEdit(textEdit)
{
    oldCoutBuffer = std::cout.rdbuf(this);
}

std::streamsize StdOutRedirect::xsputn(const char *s, std::streamsize n)
{
    QString currentTime = QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate);
    QString str(s);
    textEdit->append(currentTime + ": " + str);

    return n;
}
