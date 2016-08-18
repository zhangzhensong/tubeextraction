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
	
	int tube_id;					     // �����ĸ�tube
	int current_frame_id;                // �ڵڼ�֡����
	
	cv::Point2f tlpt;
	cv::Point2f drpt;

	bool isKeyFrame;                     // �Ƿ�ͨ����Ϊ�����ˣ�����������ͼƬ��ΪKeyFrame
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

	int tube_id;                         // �ڼ�������
	int start_frame;                     // �������һ�γ��ֵ�λ��
	int end_frame;                       // ���������һ�γ��ֵ�λ��

	int keyframe_num;                    // keyframe number

	std::vector<TubeElem> tubeElems;     // ����������Ƶ��ÿһ֡���ֵ�λ��
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

	int frame_id;						 // �ڼ�֡
	std::string filePath;                // ��Ӧ��ͼƬ�Ĵ洢λ��
	std::vector<TubeElem> tubeElems;     // ��֡������������
};


class VideoObject
{
public:
	VideoObject();
	~VideoObject();

	void initialize();
	std::vector<Tube>::iterator findTube(int tubeID);            // ����id�ҵ�tube

	void loadInfo(std::string saveFile);
	void saveInfo(std::string saveFile);

	int currentTubeID;					 // ��ǰobject/tube ID
	int currentFrame;                    // ��ǰ֡
	std::string videopath;               // ����Ƶ�Ĵ洢λ�� 
	std::string imagefolder;             // ����ֱ�Ӵ�����Ƶ��Ӧ���Ǵ�����
	
	std::vector<FrameElem> allFrames;    // ÿһ֡����Ϣ
	std::vector<Tube> allTubes;          // ����Ƶ�ж��ٸ�����
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

