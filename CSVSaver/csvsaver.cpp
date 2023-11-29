#include "csvsaver.h"
#include <QTextStream>

void CSVSaver::saveToFile(const QString &filename, const QMap<time_t, QVariant>& data, const QList<QString>& title)
{
    if (data.isEmpty())
        return;

    // Open CSV-file for writing
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        // title
        if (!title.isEmpty())
        {
            for (int i = 0; i < title.size(); ++i)
            {
                if (i != 0)
                    stream << ", ";

                stream << title.at(i);
            }
            stream << Qt::endl;
        }

        // data
        for (auto it = data.constBegin(); it != data.constEnd(); ++it)
        {
            stream << it.key() << "," << it.value().toString() << Qt::endl;
        }

        // close
        file.close();
    }
}

void CSVSaver::openFromFile(const QString &filename, QMap<time_t, QVariant>& data, QList<QString>& title)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        // title
        QString headerLine = stream.readLine();
        QStringList header = headerLine.split(",");
        title.append(header);



        if (!title.isEmpty())
        {
            for (int i = 0; i < 2; ++i)
            {
                if (i != 0)
                    stream << ", ";

                stream << title.at(i);
            }
            stream << Qt::endl;
        }

        // data
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList values = line.split(",");

            data.insert(QString(values.at(0)).toULong(), QVariant(values.at(1)));
        }

        // close
        file.close();
    }
}
