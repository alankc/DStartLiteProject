#include <iostream>
#include <thread>
#include "DStartLite.h"
#include <player-3.0/libplayerc++/playerc++.h>

int8_t* m;

bool prox(player_pose2d_t a, player_pose2d_t b)
{
    double dx = std::abs(a.px - b.px);
    double dy = std::abs(a.py - b.py);
    return (dx < 1) && (dy < 1);
}

void Exec()
{
    DStartLite d;
    d.MountTheMap(m, 592, 592, 14, 10, 37, 10);
    VertexPosition vpS;
    vpS.x = 126;
    vpS.y = 3;

    VertexPosition vpG;
    vpG.x = 15;
    vpG.y = 182;

    d.Initialize(vpS, vpG);

    VertexPosition next;
    std::vector<VertexUpdate> up;
    
    std::cout << d.TotalCost() << std::endl;

    while (d.GetNext(next, up));
}

int main(int argc, char** argv)
{
    PlayerCc::PlayerClient robot("localhost");
    PlayerCc::Position2dProxy p(&robot, 2);
    PlayerCc::Position2dProxy po(&robot, 1);
    PlayerCc::MapProxy m1(&robot, 0);

    m1.RequestMap();
    m = new int8_t[m1.GetHeight() * m1.GetWidth()];
    m1.GetMap(m);

    int num_threads = 1;
    std::thread t[num_threads];

    //Launch a group of threads
    for (int i = 0; i < num_threads; ++i)
    {
        t[i] = std::thread(Exec);
    }


    //Join the threads with the main thread
    for (int i = 0; i < num_threads; ++i)
    {
        t[i].join();
    }
    return 0;
}

/*
 
       PlayerCc::PlayerClient robot("localhost");
    PlayerCc::Position2dProxy p(&robot, 2);
    PlayerCc::Position2dProxy po(&robot, 1);
    PlayerCc::MapProxy m1(&robot, 0);

    m1.RequestMap();
    int8_t* m = new int8_t[m1.GetHeight() * m1.GetWidth()];
    m1.GetMap(m);
    for (int i = 0; i < 200; i++)
    {
        DStartLite d;
        d.MountTheMap(m, m1.GetWidth(), m1.GetHeight(), 14, 10, 37, 10);
        int factor = 37 / 10;
        double mult = (double) factor / 37;

        robot.Read();
        VertexPosition vpS;
        vpS.x = 120;
        vpS.y = 3;

        VertexPosition vpG;
        vpG.x = 15;
        vpG.y = 182;

        d.Initialize(vpS, vpG);

        VertexPosition next;
        std::vector<VertexUpdate> up;

        player_pose2d_t ant;
        ant.px = p.GetXPos();
        ant.py = p.GetYPos();
        ant.pa = p.GetYaw();

        while (d.GetNext(next, up))
        {
            player_pose2d_t atu;
            atu.px = std::round(next.x * mult) - 7.5;
            atu.py = std::round(next.y * mult) - 7.5;


            if (atu.px == ant.px && atu.py == ant.py) continue;

            double theta = std::atan2(atu.py - ant.py, atu.px - ant.px);
            ant = atu;

            std::cout << "Next (x,y): " << atu.px << " " << atu.py << "|" << atu.px + 8 << " " << atu.py + 8 << std::endl;

            p.GoTo(atu);
            while (true)
            {
                robot.Read();
                player_pose2d_t tmp;
                tmp.px = po.GetXPos();
                tmp.py = po.GetYPos();
                if (prox(atu, tmp)) break;
            }

        }
    }
 
 */
