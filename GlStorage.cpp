#include "GlStorage.h"

#include <iostream>

vector<int> GlStorage::instances;

GlStorage::GlStorage()
{
	for (int i = 0; i < instances.size(); i++)
		if (instances[i] == 0)
		{
			instances[id = i] = 1;
			return;
		}
	//if instance array is full add new element
	id = instances.size();
	instances.push_back(1);
}

bool GlStorage::Release()
{
	return !--instances[id];
}

void GlStorage::Copy(int other)
{
	instances[id = other]++;
}
