#include "xfile.h"

QStringList xfile::GetTexturesList()
{
	return texturesList;
}

void xfile::Load(QString filePath)
{
	if (!QFile::exists(filePath))
	{
		return;
		//TODO: Handle file not exists error
	}

	QFile meshFile(filePath);

	if (!meshFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}

	texturesList = QStringList();

	char* buffer = new char[256];
	QRegularExpression expression("\"(.*)\";", QRegularExpression::CaseInsensitiveOption);

	if(meshFile.readLine().contains(X_FILE_MAGIC))
	{
		while (!meshFile.atEnd())
		{
			if(meshFile.readLine().contains(TEXTURENAME_BRACKET))
			{
				QRegularExpressionMatch match = expression.match(meshFile.readLine());
				texturesList.append(match.captured(1));
			}
		}
	}

	meshFile.close();
}
