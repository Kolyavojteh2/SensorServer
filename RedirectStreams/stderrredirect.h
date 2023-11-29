#ifndef STDERRREDIRECT_H
#define STDERRREDIRECT_H

#include <streambuf>
#include <QTextEdit>

class StdErrRedirect : public std::streambuf
{
public:
    StdErrRedirect(QTextEdit *textEdit = nullptr);

    std::streamsize xsputn(const char *s, std::streamsize n) override;

private:
    QTextEdit *textEdit;
    std::streambuf *oldCoutBuffer;
};

#endif // STDERRREDIRECT_H
