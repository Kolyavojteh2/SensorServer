#ifndef CSVSAVER_H
#define CSVSAVER_H

#include <QFile>
#include <QMap>
#include <QVariant>
#include <QStyleHintReturnVariant>

class CSVSaver
{
public:
    static void saveToFile(const QString &filename, const QMap<time_t, QVariant>& data, const QList<QString>& title = QList<QString>());
    static void openFromFile(const QString &filename, QMap<time_t, QVariant>& data, QList<QString>& title);

};

#endif // CSVSAVER_H
