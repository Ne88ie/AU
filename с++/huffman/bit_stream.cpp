#include "bit_stream.h"

BitStream::BitStream(std::string file_name, Mode mode) : stream_(), mode_(mode) {
    switch (mode) {
        case rb:
            stream_.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            buffer_.reset(-1);
            break;
        case wb:
            stream_.open(file_name.c_str(), std::ofstream::binary | std::ofstream::out);
            break;
    }
}

BitStream::~BitStream() {
    if (mode_ == wb && !buffer_.empty()) {
        flush_buffer();
    }
    // stream_.close();
}

uint64_t BitStream::readblock() {
    uint64_t n = 0;
    for (int i = 0; i < 64; ++i) {
        bool bit = readBit();
        n += (bit << i);
    }
    return n;
}

void BitStream::read(size_t &n) {
    n = size_t(readblock());
}

bool BitStream::readBit() {
    if (buffer_.eof())
        buffer_.setChar((unsigned char)(stream_.get()));
    return buffer_.readBit();
}

void BitStream::writeblock(uint64_t n) {
    int writtenBits = 0;
    while (n) {
        writeBit(n & 1);
        n >>= 1;
        writtenBits++;
    }
    for (; writtenBits < 64; writtenBits++) {
        writeBit(0);
    }
}

void BitStream::write(size_t n) {
    writeblock(uint64_t(n));
}

void BitStream::writeBit(bool bit) {
    buffer_.writeBit(bit);
    if (buffer_.eof())
        flush_buffer();
}

void BitStream::flush_buffer() {
    stream_ << buffer_.getChar();
}

BitStream::Buffer::Buffer() : data_(0), current_position(7) { }

void BitStream::Buffer::writeBit(bool bit) {
    if (eof())
        reset();
    if (bit)
        data_ |= (1 << current_position);
    current_position--;
}

bool BitStream::Buffer::readBit() {
    if (eof())
        reset();
    bool result = bool(data_ & (1 << current_position));
    --current_position;
    return result;
}

bool BitStream::Buffer::empty() const {
    return current_position == 7;
}

bool BitStream::Buffer::eof() const {
    return current_position < 0;
}

unsigned char BitStream::Buffer::getChar() const {
    return data_;
}

void BitStream::Buffer::setChar(unsigned char c) {
    reset();
    data_ = c;
}

void BitStream::Buffer::reset(int position) {
    current_position = position;
    data_ = 0;
}