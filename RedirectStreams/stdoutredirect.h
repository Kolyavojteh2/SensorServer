#ifndef STDOUTREDIRECT_H
#define STDOUTREDIRECT_H

#include <streambuf>
#include <QTextEdit>

class StdOutRedirect : public std::streambuf
{
public:
    StdOutRedirect(QTextEdit *textEdit = nullptr);

    std::streamsize xsputn(const char *s, std::streamsize n) override;

private:
    QTextEdit *textEdit;
    std::streambuf *oldCoutBuffer;
};

#endif // STDOUTREDIRECT_H
