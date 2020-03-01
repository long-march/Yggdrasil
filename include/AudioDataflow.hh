
#pragma once

#include <memory>
#include <vector>

namespace Volsung {
 
class AudioBuffer
{
public:
    const static inline size_t blocksize = 1024;
    using Block = std::array<float, blocksize>;
    
private:

    std::shared_ptr<Block> data = nullptr;

//    static inline std::vector<std::pair<bool, Block>> block_pool;
public:
    const static AudioBuffer zero;
 
    float& operator[](size_t);
    float operator[](size_t) const;
    float* data_pointer();
    AudioBuffer();

    auto begin() { return std::begin(*data); }
    auto end() { return std::end(*data); }
};
using MultichannelBuffer = std::vector<AudioBuffer>;
using Block = AudioBuffer::Block;

struct AudioConnector
{
    AudioBuffer stored_buffer;
};

class CircularBuffer
{
    std::vector<float> stream = { 0.f, 0.f };
    size_t pointer = 0;

public:
    float& operator[](long);
    float operator[](long) const;
    void resize_stream(const size_t);
    void increment_pointer();

    CircularBuffer() = default;
    CircularBuffer(const size_t);
};



struct AudioInput
{
    std::vector<std::shared_ptr<AudioConnector>> connections;

    bool is_connected() const;
    const AudioBuffer read_buffer() const;
};

struct AudioOutput
{
    std::vector<std::shared_ptr<AudioConnector>> connections;

    void write_buffer(const AudioBuffer);
    void connect(AudioInput&);
};

}

