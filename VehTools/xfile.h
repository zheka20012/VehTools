#pragma once

#include <qfile.h>
#include <qregularexpression.h>

#define X_FILE_MAGIC "xof 0302txt"
#define TEXTURENAME_BRACKET "TextureFilename {"

class xfile
{
public:

	QStringList GetTexturesList();
	void Load(QString filePath);
	bool IsLoaded() { return isLoaded; }

private:
	bool isLoaded = false;
	QStringList texturesList;
};
