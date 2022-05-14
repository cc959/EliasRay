#pragma once

#include <vector>

using namespace std;

class GlStorage
{
public:
	int id;
	static vector<int> instances;

	virtual bool Release();

	void Copy(int other);

	GlStorage();
};
