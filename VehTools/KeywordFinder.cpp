#include "KeywordFinder.h"



KeywordFinder::KeywordFinder(const QString& filename)
{
	QFileInfo info(filename);
	fileName = filename;
	filePath = info.absolutePath().append("/");	

	fileHandle.setFileName(filename);
	fileHandle.setTextModeEnabled(true);
		
	if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		Logger::instance()->Print("Device failed to open!");
		Logger::instance()->Print(fileHandle.fileName());
		return;
	}

	inputStream.setDevice(&fileHandle);
}


QList<QString> KeywordFinder::GetFilesByKeyword(const QVector<QString>& keywords, bool absolute)
{
	QList<QString> foundItems = QList<QString>();

	while (!inputStream.atEnd())
	{
		QString line = inputStream.readLine();

		for (int i = 0; i< keywords.length(); i++)
		{
			
			if(QString::compare(line,keywords[i], Qt::CaseSensitive) == 0)
			{
				foundItems.append((absolute ? filePath : "") + inputStream.readLine());
				//Logger::instance()->Print(foundItems[foundItems.count() - 1]);
			}
		}
	}
	inputStream.seek(0);
	
	return foundItems;
}

QList<QString> KeywordFinder::GetFilesByKeyword(const QString& keyword, bool absolute)
{

	QList<QString> foundItems = QList<QString>();


	while (!inputStream.atEnd())
	{
		QString line = inputStream.readLine();

		if (QString::compare(line, keyword, Qt::CaseSensitive) == 0)
		{
			foundItems.append((absolute ? filePath : "") + inputStream.readLine());
			//Logger::instance()->Print(foundItems[foundItems.count() - 1]);
		}
	}
	inputStream.seek(0);
	return foundItems;
}

QString KeywordFinder::GetFileByKeyword(const QString& keyword)
{

	while (!inputStream.atEnd())
	{
		QString line = inputStream.readLine();

		if (QString::compare(line, keyword, Qt::CaseSensitive) == 0)
		{
			QString path = (filePath + inputStream.readLine());
			inputStream.seek(0);
			return path;
		}
	}
	inputStream.seek(0);
	return Q_NULLPTR;
}

QList<QString> KeywordFinder::GetMultilineFileByKeyword(const QString& keyword, bool absolute)
{
	QList<QString> foundItems = QList<QString>();
	
	while (!inputStream.atEnd())
	{
		QString line = inputStream.readLine();

		if (QString::compare(line, keyword, Qt::CaseSensitive) == 0)
		{
			quint32 itemsCount = inputStream.readLine().toInt();
			for (int i = 0; i < itemsCount; i++)
			{
				foundItems.append((absolute ? filePath : "") + inputStream.readLine());

				//Logger::instance()->Print(foundItems[foundItems.count() - 1]);
			}


		}
	}
	inputStream.seek(0);
	return foundItems;
}



QString KeywordFinder::GetAbsolutePath()
{
	return filePath;
}


