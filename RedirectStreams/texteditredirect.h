#ifndef TEXTEDITREDIRECT_H
#define TEXTEDITREDIRECT_H

#include <QTextEdit>
#include "stdoutredirect.h"
#include "stderrredirect.h"

class TextEditRedirect : public QTextEdit
{
public:
    TextEditRedirect(QWidget *parent = nullptr);

    void redirectStdOut();
    void restoreStdOut();

    void redirectStdErr();
    void restoreStdErr();

private:
    std::streambuf *oldStdOut;
    StdOutRedirect *newStdOut;

    std::streambuf *oldStdErr;
    StdErrRedirect *newStdErr;

};

#endif // TEXTEDITREDIRECT_H
