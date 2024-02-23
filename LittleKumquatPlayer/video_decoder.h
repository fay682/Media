#pragma once
#include <iostream>
class VideoDecoder {
  public:
    void openFile(std::string url);
    void closeFile();

    bool isEOF();

    void decodeFrame(float duration);

  private:
    void protocalParser();

    void formatDemuxer();

    void decoder();

};

