//Corresponding header
#include "Not_Killer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "utils/CommonDefines.h"
#include "utils/Random.hpp"


Not_Killer::Not_Killer() : _playerIdx(INT32_MAX),
                   _boardWidth(INT32_MAX),
                   _boardHeight(INT32_MAX)
{

}

int32_t Not_Killer::init(const int32_t playerIdx,
                     const Point * playerPos,
                     const int32_t totalPlayersCount,
                     const int32_t boardWidth,
                     const int32_t boardHeight)
{
    _currPos     = *playerPos;
    _playerIdx   = playerIdx;
    _boardWidth  = boardWidth;
    _boardHeight = boardHeight;

    _path.emplace_back(_currPos);

    //dummy check to satisfy Wunused-variable gcc warning
    if(totalPlayersCount) { }

    return EXIT_SUCCESS;
}

void Not_Killer::deinit()
{

}

void Not_Killer::requestPlayerMove(const int64_t allowedTime,
                               int32_t *     outPlayerDir)
{
    //dummy check to satisfy Wunused-variable gcc warning
    if(allowedTime) { }

    Random & rng = Random::getInstance();

    Point prevPos(1000, 1000); //some invalid destination

    if(1 < _path.size())
    {
        //take the points before the last
        prevPos = _path[_path.size() - 2];
    }

    Point newPos;
    bool isNewPosInvalid = true;

    do
    {
        newPos = _currPos;

        if(rng.generate(0, 1)) //is horizontal move
        {
            if(rng.generate(0, 1)) //should go left
            {
                --newPos.x;
                *outPlayerDir = PlayerDir::LEFT;
            }
            else //go right
            {
                ++newPos.x;
                *outPlayerDir = PlayerDir::RIGHT;
            }
        }
        else //vertical move
        {
            if(rng.generate(0, 1)) //should go up
            {
                --newPos.y;
                *outPlayerDir = PlayerDir::UP;
            }
            else //go down
            {
                ++newPos.y;
                *outPlayerDir = PlayerDir::DOWN;
            }
        }

        isNewPosInvalid = (newPos == prevPos)          ||
                          (0 > newPos.x)               ||
                          (_boardWidth - 1 < newPos.x) ||
                          (0 > newPos.y)               ||
                          (_boardHeight - 1 < newPos.y);
    }while(isNewPosInvalid);

    _currPos = newPos;
    _path.emplace_back(_currPos);
}

void Not_Killer::onOpponentMove(const int32_t playerIdx,
                            const Point * playerPos,
                            const int32_t playerDir)
{
    //dummy check to satisfy Wunused-variable gcc warning
    if(playerIdx || playerPos || playerDir) { }
}

void Not_Killer::forceErasePath()
{
    printf("Player: %d - forceErasePath() invoked\n", _playerIdx);

    _path.clear();
    _path.emplace_back(_currPos);
}

void Not_Killer::forceRollbackToPos(const Point * playerPos)
{
    printf("Player: %d - forceRollbackToPos(%d, %d) invoked\n",
                                   _playerIdx, (*playerPos).x, (*playerPos).y);

    //path was already cleared by ::forceErasePath()
    if(1 == _path.size())
    {
        _path[0] = *playerPos;
    }
    else //board-out-of-bound case
    {
        while(*playerPos != _path.back())
        {
            _path.pop_back();
        }
    }

    _currPos = *playerPos;
}

void Not_Killer::scoreUpdate(const int32_t playerIdx,
                         const Point * scoreRects,
                         const int32_t scoreRectsCount)
{
    for(int32_t i = 0; i < scoreRectsCount; ++i)
    {
        printf("Player not_killer with Id: %d formed rect on x: %d, y: %d\n",
                                 playerIdx, scoreRects[i].x, scoreRects[i].y);
    }

    _path.clear();
    _path.emplace_back(_currPos);
}




