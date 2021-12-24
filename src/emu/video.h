// video.h - video management package
//
// Author:  Tim Stark
// Date:    12/23/21

#pragma once

class Machine;

class VideoManager
{
public:
    VideoManager(Machine &sysMachine);


private:
    Machine &sysMachine;

};