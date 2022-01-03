// fileio.h - I/O file management
//
// Author:  Tim Stark
// Date:    1/2/2022

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