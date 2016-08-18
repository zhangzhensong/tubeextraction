#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "boost/serialization/serialization.hpp"  
#include "boost/archive/binary_oarchive.hpp"  
#include "boost/archive/binary_iarchive.hpp"  
#include <boost/serialization/export.hpp>  
#include "boost/foreach.hpp"  
#include "boost/any.hpp"  
#include <boost/serialization/vector.hpp>  

struct TubeElem
{
	TubeElem()
	{
		isKeyFrame = false;
		tube_id = -1;
		current_frame_id = -1;
	}
	
	int tube_id;					     // 属于哪个tube
	int current_frame_id;                // 在第几帧出现
	
	cv::Point2f tlpt;
	cv::Point2f drpt;

	bool isKeyFrame;                     // 是否通过人为调整了，被调整过的图片称为KeyFrame
};


struct Tube
{
	Tube()
	{
		tube_id = -1;
		start_frame = -1;
		end_frame = -1;
		keyframe_num = 0;

		tubeElems.clear();
	}

	~Tube()
	{
		tubeElems.clear();
	}

	int tube_id;                         // 第几个物体
	int start_frame;                     // 该物体第一次出现的位置
	int end_frame;                       // 该物体最后一次出现的位置

	int keyframe_num;                    // keyframe number

	std::vector<TubeElem> tubeElems;     // 该物体在视频中每一帧出现的位置
};

struct FrameElem {

	FrameElem()
	{
		frame_id = -1;
		tubeElems.clear();
		filePath = "";
	}

	~FrameElem()
	{
		tubeElems.clear();
	}

	int frame_id;						 // 第几帧
	std::string filePath;                // 对应的图片的存储位置
	std::vector<TubeElem> tubeElems;     // 该帧包含几个物体
};


class VideoObject
{
public:
	VideoObject();
	~VideoObject();

	void initialize();
	std::vector<Tube>::iterator findTube(int tubeID);            // 根据id找到tube

	void loadInfo(std::string saveFile);
	void saveInfo(std::string saveFile);

	int currentTubeID;					 // 当前object/tube ID
	int currentFrame;                    // 当前帧
	std::string videopath;               // 该视频的存储位置 
	std::string imagefolder;             // 不能直接处理视频，应该是处理集合
	
	std::vector<FrameElem> allFrames;    // 每一帧的信息
	std::vector<Tube> allTubes;          // 该视频有多少个物体
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive & ar, cv::Point2f & d, const unsigned int version)
		{
			ar & d.x;
			ar & d.y;
		}

		template<class Archive>
		void serialize(Archive & ar, TubeElem & d, const unsigned int version)
		{
			ar & d.tube_id;
			ar & d.current_frame_id;
			ar & d.tlpt;
			ar & d.drpt;
			ar & d.isKeyFrame;
		}

		template<class Archive>
		void serialize(Archive & ar, Tube & d, const unsigned int version)
		{
			ar & d.tube_id;
			ar & d.start_frame;
			ar & d.end_frame;
			ar & d.keyframe_num;
			ar & d.tubeElems;
		}

		template<class Archive>
		void serialize(Archive & ar, FrameElem & d, const unsigned int version)
		{
			ar & d.frame_id;
			ar & d.filePath;
			ar & d.tubeElems;
		}

		template<class Archive>
		void serialize(Archive & ar, VideoObject & d, const unsigned int version)
		{
			ar & d.currentTubeID;
			ar & d.currentFrame;
			ar & d.videopath;
			ar & d.imagefolder;
			ar & d.allFrames;
			ar & d.allTubes;
		}
	}
}

