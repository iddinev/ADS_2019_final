//Corresponding header
#include "Not_Killer.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers

//Own components headers
#include "utils/CommonDefines.h"


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
    // Point (-1,-1) is used as an invalid marker.
    _enemy_currPos = Point(-1,-1);
    _get_rect = false;

    _path.emplace_back(_currPos);

    centerReset();

    //dummy check to satisfy Wunused-variable gcc warning
    if(totalPlayersCount) { }

    return EXIT_SUCCESS;
}

void Not_Killer::deinit()
{

}

// Whoever plays - wins, whoever doesn't play - does not win.
void Not_Killer::requestPlayerMove(const int64_t allowedTime,
                               int32_t *     outPlayerDir)
{
    //dummy check to satisfy Wunused-variable gcc warning
    if(allowedTime) { }


    Point prevPos(1000, 1000); //some invalid destination

    if(1 < _path.size())
    {
        //take the points before the last
        prevPos = _path[_path.size() - 2];
    }

    Point newPos;
    newPos = _currPos;
    // bool isNewPosInvalid = true;

    if ( _currPos == _centerPos )
    {
        if ( _posSeqSize > 1)
        {
            if ( _currSeqDir == _posSeqSize -1 )
            {
                _currSeqDir = 0;
            }
            else
            {
                // Change quadrant if you scored or were reset somehow.
                if(_get_rect)
                {
                    ++_currSeqDir;
                    _get_rect = false;
                }
            } 
        }
    }
    printf("Quadrant %d invoked\n", _posSeq[_currSeqDir]);

    // Walk, walk, walk...

    switch (_posSeq[_currSeqDir])
    {
        case PlayerDir::LEFT: *outPlayerDir = new_dir(PlayerDir::LEFT);
        break;
        case PlayerDir::DOWN: *outPlayerDir = new_dir(PlayerDir::DOWN);
        break;
        case PlayerDir::RIGHT: *outPlayerDir = new_dir(PlayerDir::RIGHT);
        break;
        case PlayerDir::UP: *outPlayerDir = new_dir(PlayerDir::UP);
        break;
        default:
        break;
    }
    switch (*outPlayerDir)
    {
        case PlayerDir::LEFT: printf("Direction LEFT\n");
        break;
        case PlayerDir::DOWN: printf("Direction DOWN\n");
        break;
        case PlayerDir::RIGHT:printf("Direction RIGHT\n");
        break;
        case PlayerDir::UP:   printf("Direction UP\n");
        break;
        default:
        break;
    }
    
    switch (*outPlayerDir)
    {
        case PlayerDir::LEFT: --newPos.x;
        break;
        case PlayerDir::RIGHT: ++newPos.x;
        break;
        case PlayerDir::UP: --newPos.y;
        break;
        case PlayerDir::DOWN: ++newPos.y;
        break;
        default:
        break;
    }

    _currPos = newPos;
    _path.emplace_back(_currPos);
    printf("New position - x: %d y : %d\n", _currPos.x, _currPos.y);
}

void Not_Killer::onOpponentMove(const int32_t playerIdx,
                            const Point * playerPos,
                            const int32_t playerDir)
{
    _enemy_currPos = *playerPos;
    Point enemy_newPos;
    enemy_newPos = _enemy_currPos;

    switch (playerDir)
    {
        case PlayerDir::LEFT: --enemy_newPos.x ;
        break;
        case PlayerDir::RIGHT: ++enemy_newPos.x;
        break;
        case PlayerDir::UP: --enemy_newPos.y;
        break;
        case PlayerDir::DOWN: ++enemy_newPos.y;
        break;
        default:
        break;
    }

    _enemy_currPos = enemy_newPos;
    _enemy_path.emplace_back(_enemy_currPos);
    //dummy check to satisfy Wunused-variable gcc warning
    if(playerIdx || playerPos || playerDir) { }
}

void Not_Killer::forceErasePath()
{
    printf("Player: %d - forceErasePath() invoked\n", _playerIdx);

    _get_rect = true;
    _path.clear();
    _path.emplace_back(_currPos);
    centerReset();
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

    _get_rect = true;
    _currPos = *playerPos;
    centerReset();
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


    _get_rect = true;
    _path.clear();
    _path.emplace_back(_currPos);
}

PlayerDir Not_Killer::new_dir(const PlayerDir qDir)
{
    PlayerDir outDir = PlayerDir::LEFT; 

    if ( qDir == PlayerDir::RIGHT )
    {
        if ( _currPos.x < (_boardWidth - 1) && _currPos.y == _centerPos.y )
        {
            outDir = PlayerDir::RIGHT;
            printf("Q RIGHT :: RIGHT\n");
        }
        else if ( _currPos.y > 0 && _currPos.x == (_boardWidth - 1) )
        {
            outDir = PlayerDir::UP;
            printf("Q RIGHT :: UP\n");
        }
        else if ( _currPos.x > _centerPos.x && _currPos.y == 0 )
        {
            outDir = PlayerDir::LEFT;
            printf("Q RIGHT :: LEFT\n");
        }
        else if ( _currPos.y < _centerPos.y  && _currPos.x == _centerPos.x )
        {
            outDir = PlayerDir::DOWN;
            printf("Q RIGHT :: DOWN\n");
        }
    }
    else if ( qDir == PlayerDir::UP )
    {
        if ( _currPos.x < _centerPos.x && _currPos.y == _centerPos.y )
        {
            outDir = PlayerDir::RIGHT;
            printf("Q UP :: RIGHT\n");
        }
        else if ( _currPos.y > 0 && _currPos.x == _centerPos.x )
        {
            outDir = PlayerDir::UP;
            printf("Q UP :: UP\n");
        }
        else if ( _currPos.x > 0 && _currPos.y == 0 )
        {
            outDir = PlayerDir::LEFT;
            printf("Q UP :: LEFT\n");
        }
        else if ( _currPos.y < _centerPos.y && _currPos.x == 0 )
        {
            outDir = PlayerDir::DOWN;
            printf("Q UP :: DOWN\n");
        }
    }
    else if ( qDir == PlayerDir::LEFT )
    {
        if ( _currPos.x < _centerPos.x && _currPos.y == (_boardHeight -1) )
        {
            outDir = PlayerDir::RIGHT;
            printf("Q LEFT :: RIGHT\n");
        }
        else if ( _currPos.y > _centerPos.y && _currPos.x == _centerPos.x )
        {
            outDir = PlayerDir::UP;
            printf("Q LEFT :: UP\n");
        }
        else if ( _currPos.x > 0 && _currPos.y == _centerPos.y )
        {
            outDir = PlayerDir::LEFT;
            printf("Q LEFT :: LEFT\n");
        }
        else if ( _currPos.y < (_boardHeight - 1) && _currPos.x == 0 )
        {
            outDir = PlayerDir::DOWN;
            printf("Q LEFT :: DOWN\n");
        }
    }
    else if ( qDir == PlayerDir::DOWN )
    {
        if ( _currPos.x < (_boardWidth - 1) && _currPos.y == (_boardHeight - 1) )
        {
            outDir = PlayerDir::RIGHT;
            printf("Q DOWN :: RIGHT\n");
        }
        else if ( _currPos.y > _centerPos.y && _currPos.x == (_boardWidth - 1) )
        {
            outDir = PlayerDir::UP;
            printf("Q DOWN :: UP\n");
        }
        else if ( _currPos.x > _centerPos.x && _currPos.y == _centerPos.y )
        {
            outDir = PlayerDir::LEFT;
            printf("Q DOWN :: LEFT\n");
        }
        else if ( _currPos.y < (_boardHeight - 1) && _currPos.x == _centerPos.x )
        {
            outDir = PlayerDir::DOWN;
            printf("Q DOWN :: DOWN\n");
        }
    }

    return outDir;
}
void Not_Killer::centerReset()
{
    _centerPos   = _currPos;
    if ( ( (_centerPos.x < _boardWidth -1) && _centerPos.x > 0 ) &&
        ( (_centerPos.y < _boardHeight -1) && _centerPos.y > 0 ) )
    {
        _posSeq = {
            PlayerDir::RIGHT,
            PlayerDir::UP,
            PlayerDir::LEFT,
            PlayerDir::DOWN };
    }
    else if ( 0 == _centerPos.x  && 0 == _centerPos.y )
    {
        _posSeq = {
            PlayerDir::DOWN };
    }
    else if ( (_boardWidth -1) == _centerPos.x  && 0 == _centerPos.y )
    {
        _posSeq = {
            PlayerDir::LEFT };
    }
    else if ( 0 == _centerPos.x  && ( _boardHeight - 1) == _centerPos.y )
    {
        _posSeq = {
            PlayerDir::RIGHT };
    }
    else if ( (_boardWidth -1) == _centerPos.x  && (_boardHeight -1) == _centerPos.y )
    {
        _posSeq = {
            PlayerDir::UP };
    }
    else if ( ( (_centerPos.x < _boardWidth -1) && _centerPos.x > 0 ) &&
         _centerPos.y == 0 )
    {
        _posSeq = {
            PlayerDir::LEFT,
            PlayerDir::DOWN };
    }
    else if ( ( (_centerPos.x < _boardWidth -1) && _centerPos.x > 0 ) &&
         _centerPos.y == (_boardHeight -1)  )
    {
        _posSeq = {
            PlayerDir::UP,
            PlayerDir::RIGHT };
    }
    else if ( ( (_centerPos.y < _boardHeight -1) && _centerPos.y > 0 ) &&
         _centerPos.x == 0 )
    {
        _posSeq = {
            PlayerDir::DOWN,
            PlayerDir::RIGHT };
    }
    else if ( ( (_centerPos.y < _boardHeight -1) && _centerPos.y > 0 ) &&
         _centerPos.x == (_boardWidth -1) )
    {
        _posSeq = {
            PlayerDir::UP,
            PlayerDir::LEFT };
    }

    _currSeqDir = 0;
    _posSeqSize = static_cast<int32_t>(_posSeq.size());
}
