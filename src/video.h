#define OLC_PGE_APPLICATION
#include "pge/olcPixelGameEngine.h"

class video : public olc::PixelGameEngine
{

public:
	
	std::thread video_thread;
	ram* video_ram;

	video(ram* video_ram)
	{	
		this->video_ram = video_ram;
		sAppName = "PPLVC Video display";
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

    void runChip(){
		video_thread = std::thread(&video::Start,this);
    }

	void join(){
		video_thread.join();
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				if(video_ram->m_pointer[256*y+x] == 1){
					Draw(x, y, olc::Pixel(255,255,255,255));
				}else{
					Draw(x, y, olc::Pixel(0,0,0,255));
				}
		return true;
	}
};
