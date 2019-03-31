//C system headers

//C++ system headers
#include <cstdint>
#include <cstdlib>
#include <cstdio>

//Other libraries headers

//Own components headers
#include "Not_Killer.h"

//Forward declarations
struct Point;

static Not_Killer * not_killer = nullptr;


extern "C"
{

int32_t init(const int32_t playerIdx,
             const Point * playerPos,
             const int32_t totalPlayersCount,
             const int32_t boardWidth,
             const int32_t boardHeight)
{
    int32_t err = EXIT_SUCCESS;

    if(nullptr != not_killer)
    {
        fprintf(stderr, "Error, object was already initialised!\n");

        err = EXIT_FAILURE;
    }
    else
    {
        not_killer = new Not_Killer();

        if(nullptr == not_killer)
        {
            fprintf(stderr, "Error, bad alloc for Not_Killer()");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != not_killer->init(playerIdx,
                                        playerPos,
                                        totalPlayersCount,
                                        boardWidth,
                                        boardHeight))
        {
            fprintf(stderr, "Error, not_killer->init() failed");

            err = EXIT_FAILURE;
        }
    }

    return err;
}

void deinit()
{
    not_killer->deinit();
}

void requestPlayerMove(const int64_t allowedTime,
                       int32_t *     outPlayerDir)
{
    not_killer->requestPlayerMove(allowedTime, outPlayerDir);
}

void onOpponentMove(const int32_t playerIdx,
                    const Point * playerPos,
                    const int32_t playerDir)
{
    not_killer->onOpponentMove(playerIdx, playerPos, playerDir);
}

void forceErasePath()
{
    not_killer->forceErasePath();
}

void forceRollbackToPos(const Point * playerPos)
{
    not_killer->forceRollbackToPos(playerPos);
}

void scoreUpdate(const int32_t playerIdx,
                 const Point * scoreRects,
                 const int32_t scoreRectsCount)
{
    not_killer->scoreUpdate(playerIdx, scoreRects, scoreRectsCount);
}

} //extern "C"




