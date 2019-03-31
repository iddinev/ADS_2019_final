#ifndef BOTS_NOT_KILLER_KILLER_H_
#define BOTS_NOT_KILLER_KILLER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "utils/Point.h"
#include "utils/CommonDefines.h"

//Forward declarations


class Not_Killer
{
    public:
        Not_Killer();
        ~Not_Killer() = default;

        int32_t init(const int32_t playerIdx,
                     const Point * playerPos,
                     const int32_t totalPlayersCount,
                     const int32_t boardWidth,
                     const int32_t boardHeight);

        void deinit();

        void requestPlayerMove(const int64_t allowedTime,
                               int32_t *     outPlayerDir);

        void onOpponentMove(const int32_t playerIdx,
                            const Point * playerPos,
                            const int32_t playerDir);

        void forceErasePath();

        void forceRollbackToPos(const Point * playerPos);

        void scoreUpdate(const int32_t playerIdx,
                         const Point * scoreRects,
                         const int32_t scoreRectsCount);

    private:
        PlayerDir new_dir(const PlayerDir qDir);
        void centerReset();

        Point                  _currPos;
        Point                  _enemy_currPos;
        Point                  _centerPos;

        std::vector<Point>     _path;
        std::vector<Point>     _enemy_path;
        std::vector<PlayerDir> _posSeq;

        int32_t                _playerIdx;
        int32_t                _enemy_playerIdx;

        int32_t                _boardWidth;
        int32_t                _boardHeight;
        int32_t                _currSeqDir;
        int32_t                _posSeqSize;

        bool                   _get_rect;
};
#endif /* BOTS_NOT_KILLER_KILLER_H_ */
