#pragma once
#include <QString>
#include <QStringList>

class IMeshFile
{
public:
	virtual ~IMeshFile() {};
	virtual QStringList GetTexturesList();
	virtual void Load(QString filePath);
	virtual bool IsLoaded();
};
