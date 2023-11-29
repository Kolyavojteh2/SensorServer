#include "texteditredirect.h"
#include <iostream>

TextEditRedirect::TextEditRedirect(QWidget *parent) : QTextEdit(parent) {}

void TextEditRedirect::redirectStdOut()
{
    oldStdOut = std::cout.rdbuf();
    newStdOut = new StdOutRedirect(this);
    std::cout.rdbuf(newStdOut);
}

void TextEditRedirect::restoreStdOut()
{
    if (newStdOut)
    {
        std::cout.rdbuf(oldStdOut);
        delete newStdOut;
    }
}

void TextEditRedirect::redirectStdErr()
{
    oldStdErr = std::cerr.rdbuf();
    newStdErr = new StdErrRedirect(this);
    std::cerr.rdbuf(newStdErr);
}

void TextEditRedirect::restoreStdErr()
{
    if (newStdErr)
    {
        std::cerr.rdbuf(oldStdErr);
        delete newStdErr;
    }
}
