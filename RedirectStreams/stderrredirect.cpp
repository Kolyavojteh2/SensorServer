#include "stderrredirect.h"
#include <iostream>
#include <QDateTime>

StdErrRedirect::StdErrRedirect(QTextEdit *textEdit) : textEdit(textEdit)
{
    oldCoutBuffer = std::cerr.rdbuf(this);
}

std::streamsize StdErrRedirect::xsputn(const char *s, std::streamsize n)
{
    QString currentTime = QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate);
    QString fontRedBegin = "<font color=\"red\">";
    QString fontRedEnd = "</font>";
    QString str(s);
    textEdit->append(currentTime + ": " + fontRedBegin + str + fontRedEnd);

    return n;
}
