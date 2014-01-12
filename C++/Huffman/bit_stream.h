#ifndef BITSTREAM_H_
#define BITSTREAM_H_

#include <string>
#include <fstream>
#include <cstddef>

struct BitStream {
    enum Mode {rb, wb};
    BitStream(std::string file_name, Mode mode = rb);
    ~BitStream();

    void read(size_t &);
    bool readBit();

    void write(size_t);
    void writeBit(bool);

private:
    struct Buffer {
        Buffer();

        void writeBit(bool);
        bool readBit();

        bool empty() const;
        bool eof() const;

        unsigned char getChar() const;
        void setChar(unsigned char);
        void reset(int position = 7);

    private:
        unsigned char data_;
        int current_position;
    } buffer_;

    std::fstream stream_;
    Mode mode_;

    void flush_buffer();

    void writeblock(uint64_t);
    uint64_t readblock();
};

#endif