#pragma once
#include <qlist.h>
#include <qfileinfo.h>
#include "VehTools.h"
#include <QTextStream>

class KeywordFinder
{
public:
	KeywordFinder(const QString& filename);
	KeywordFinder(const KeywordFinder&);
	QList<QString> GetFilesByKeyword(const QVector<QString>& keywords, bool absolute = true);
	QList<QString> GetFilesByKeyword(const QString& keyword, bool absolute = true);
	QString GetFileByKeyword(const QString& keyword);
	QList<QString> GetMultilineFileByKeyword(const QString& keyword, bool absolute = true);
	QString GetAbsolutePath();
private:
	QString filePath;
	QString fileName;
	QFile fileHandle;
	QTextStream inputStream;
};
