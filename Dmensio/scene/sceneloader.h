#ifndef _SCENELOADER_H_
#define _SCENELOADER_H_

#include "cube.h"
#include <vector>
#include <string>

class SceneLoader
{
public:
	SceneLoader(void);
	~SceneLoader(void) { }

	static inline int GetNumberOfObjinScene(char* filename)
	{
		int numberofobjs = 0;

		FILE * file = fopen(filename, "r");
		if (file == NULL)
		{
			int y = 0;
		}

		file = fopen(filename, "r");

		while (1)
		{
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);

			if (res == EOF)
				break;

			if (strcmp(lineHeader, "Cube") == 0)
			{
				numberofobjs++;
			}
		}

		return numberofobjs;
	}

	static inline void LoadLevel(char* filename, std::vector<Cube*> &cubes)
	{
		vector<XMFLOAT3> temp_pos;
		vector<XMFLOAT3> temp_scale;
		vector<XMFLOAT3> temp_rot;
		vector<int> temp_type;

		FILE * file = fopen(filename, "r");
		if (file == NULL)
		{
			int y = 0;
		}

		file = fopen(filename, "r");

		while (1)
		{
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);

			if (res == EOF)
				break;

			if (strcmp(lineHeader, "p") == 0)
			{
				XMFLOAT3 vertex;
				fscanf(file, "%f %f %f \n", &vertex.x, &vertex.y, &vertex.z);

				temp_pos.push_back(vertex);
			}

			if (strcmp(lineHeader, "s") == 0)
			{
				XMFLOAT3 scale;
				fscanf(file, "%f %f %f \n", &scale.x, &scale.y, &scale.z);

				temp_scale.push_back(scale);
			}
			if (strcmp(lineHeader, "r") == 0)
			{
				XMFLOAT3 rot;
				fscanf(file, "%f %f %f \n", &rot.x, &rot.y, &rot.z);

				temp_rot.push_back(rot);
			}
			if (strcmp(lineHeader, "t") == 0)
			{
				int type;
				fscanf(file, "%d \n", &type);

				temp_type.push_back(type);
			}
		}

		for (unsigned int i = 0; i < cubes.size(); i++)
		{
			cubes[i]->SetPosition(temp_pos[i]);
			cubes[i]->SetScale(temp_scale[i]);
			//cubes[i]->SetRotation(temp_rot[i]);
			cubes[i]->SetProjectionType(temp_type[i]);

			int y = 0;
		}
	}
};

#endif