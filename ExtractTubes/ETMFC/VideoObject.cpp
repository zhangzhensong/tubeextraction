#include "stdafx.h"
#include "VideoObject.h"

#include <fstream>


VideoObject::VideoObject()
{
	this->initialize();
}


VideoObject::~VideoObject()
{
}

void VideoObject::initialize()
{
	this->currentFrame = 0;
	this->currentTubeID = -1;
	this->allTubes.clear();
	//this->videopath = "";
	//this->imagefolder = "";
	this->allFrames.clear();
}

void VideoObject::saveInfo(std::string saveFile)
{
	std::ofstream outfile(saveFile, std::ios::binary);
	boost::archive::binary_oarchive oa(outfile);
	oa << *this;
	outfile.close();
}

void VideoObject::loadInfo(std::string saveFile)
{
	this->initialize();
	std::ifstream infile(saveFile, std::ios::binary);
	boost::archive::binary_iarchive ia(infile);
	ia >> *this;
	infile.close();
}

struct MyComparator
{
	const int find_id;

	MyComparator(const int val) :
		find_id(val) {}

	bool operator()(Tube &tube)
	{
		return find_id == tube.tube_id;
	}
};

std::vector<Tube>::iterator VideoObject::findTube(int tubeID)
{
	std::vector<Tube>::iterator it = this->allTubes.end();
	it = std::find_if(this->allTubes.begin(), this->allTubes.end(), MyComparator(tubeID));

	return it;
}

