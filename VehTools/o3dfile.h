#pragma once

#include <qfile.h>
#include "IMeshFile.h"
#include "Logger.h"
#include "QStringList"

#define O3D_MAGIC	6532

#define O3D_VERTEXDATA	0x17
#define O3D_TRIDATA		0x49
#define O3D_MATERIAL	0x26
#define O3D_TRANSFORM	0x79
#define O3D_BONEDATA	0x54


class o3dfile
{
public:
	QStringList GetTexturesList();
	void Load(QString filePath);
	bool IsLoaded() { return isLoaded; }

private:
	bool isLoaded = false;
	char fileVersion;
	bool longIndicesSupport;
	bool equalityBitSupport;
	QStringList texturesList;

	void skipVerts(QFile* meshFile);
	void skipTris(QFile* meshFile);
	void skipBones(QFile* meshFile);
	void skipTransform(QFile* meshFile);
	void readMaterials(QFile* meshFile);

};
