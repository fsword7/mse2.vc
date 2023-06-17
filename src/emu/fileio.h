// fileio.h - I/O File Management package
//
// Author:  Tim Stark
// Date:    Jun 17, 2023

namespace emu
{
    class ioFile
    {
    public:
        ioFile(uint32_t flags);
        virtual ~ioFile() = default;

        bool open(fs::path fname);
        void close();

        int read(uint8_t *data, int length);
        int write(uint8_t *data, int length);

    private:
        std::fstream file;
    };
}