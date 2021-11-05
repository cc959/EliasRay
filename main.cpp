#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GL/glew.h>
#include <bits/stdc++.h>
#include <glm/gtx/transform.hpp>
#include "shader.h"

using namespace std;
using namespace sf;
using namespace std::complex_literals;

const double pi = 3.14159265359;

double herz = 440.0;

struct NoiseMaker
{
	static double squareIfy(double in)
	{
		if (in > 0)
			return 1;
		else
			return 0;
	}

	static double genSin(double dTime, double herz)
	{

		return sin(2 * pi * herz * dTime);
	}

	static double genSquare(double dTime, double herz)
	{
		dTime *= herz;
		return (dTime - (long)dTime < 0.5) ? 1.0 : -1.0;
	}

	static double makeNoise(double dTime)
	{
		double out = genSin(dTime, herz);

		return out;
	}

	NoiseMaker() = delete;
};

class SynthStream : public sf::SoundStream
{
	unsigned int sampleAt = 0, sampleRate, bufferSize;

public:
	void initialize(unsigned int sampleRate, unsigned int bufferSize)
	{
		sf::SoundStream::initialize(1, sampleRate);
		this->sampleRate = sampleRate;
		this->bufferSize = bufferSize;
	}

	virtual bool onGetData(Chunk &data)
	{
		vector<sf::Int16> samples(bufferSize);
		for (int i = 0; i < bufferSize; i++)
			samples[i] = sf::Int16(SHRT_MAX * NoiseMaker::makeNoise((sampleAt++) / double(sampleRate)));

		data.samples = &samples[0];
		data.sampleCount = bufferSize;

		cout << sampleAt << "\n";
		cout << herz << "\n";

		return true;
	}

	virtual void onSeek(sf::Time timeOffset)
	{
	}
};

// load the .wav file and return samples between -1 and 1
vector<short> loadSound(const string &path)
{
	SoundBuffer music;
	music.loadFromFile(path);
	auto point = music.getSamples();

	cout << music.getSampleRate() << "\n";

	vector<short> raw(point, point + music.getSampleCount());
	vector<double> samples(raw.size());

	for (int i = 0; i < raw.size(); i++)
		samples[i] = raw[i] / double(SHRT_MAX);

	return raw;
}

SoundBuffer bufferize()
{
}

struct Arrow
{
	complex<double> c; // starting config
	int herz;		   // herz
};

vector<Arrow> fourierSeries(vector<double> &samples, unsigned int detail)
{
	int N = samples.size();
	int v = detail * 2 + 1;

	vector<Arrow> series(v);

	for (int i = 0; i < v; i++)
	{
		int n = i - detail;
		for (int t = 0; t < N; t++)
			series[i].c += exp(-2.0 * pi * 1i * double(n) * double(t)) * samples[t];
		series[i].c /= N;
		series[i].herz = n;
	}
	return series;
}

vector<double> reconstruct(vector<Arrow> &fourier, int sampleCount)
{
	vector<double> samples(sampleCount);
	for (int i = 0; i < sampleCount; i++)
	{
		double t = i / double(sampleCount);
		complex<double> pos;
		for (Arrow arr : fourier)
		{
			pos += arr.c * exp(arr.herz * 2.0 * pi * 1i * t);
		}
		samples[i] = pos.real();
	}
	return samples;
}

int main()
{

	RenderWindow window(VideoMode(1000, 700), "Fourier", Style::Default, ContextSettings{24, 8, 4, 4, 6});
	window.setVerticalSyncEnabled(true);

	window.setActive(true);

	auto raw = loadSound("res/hello.wav");

	//auto fourier = fourierSeries(samples, 100);

	//auto newsamples = samples; //reconstruct(fourier, samples.size());

	// vector<short> raw(newsamples.size());

	// for (int i = 0; i < newsamples.size(); i++)
	// {
	// 	raw[i] = newsamples[i] * SHRT_MAX;
	// 	cout << samples[i] << " " << newsamples[i] << "\n";
	// }

	// SoundBuffer noise;
	// noise.loadFromSamples(&raw[0], raw.size(), 1, 44100);
	SoundBuffer noise;
	noise.loadFromFile("res/hello.wav");

	cout << "Playing sound\n";

	Sound sound;
	sound.setBuffer(noise);
	sound.play();

	bool running = true;
	while (running)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.display();
	}

	return 0;
}
