#pragma once
#include<string>

class MediaSetting {
  public:
    static void GetAudioDataFromMediaFile(std::string const& src, std::string const& dst);

    static void GetVideoDataFromMediaFile(std::string const& src, std::string const& dst);

    static void SetFfmpegLog();
};