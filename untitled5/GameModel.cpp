#include <utility>
#include <stdlib.h>
#include <time.h>
#include "GameModel.h"
#include <QFile>
#include <QDataStream>
using namespace std ;

// 权重定义
#define LIVE_TWO          10    // 活二
#define DEAD_THREE        30    // 死三
#define LIVE_THREE       100    // 活三
#define DUAL_LIVE_THREE  500    // 双活三
#define BLOCK_FOUR       300    // 冲四
#define LIVE_FOUR       2000    // 活四
#define FIVE           10000    // 成五

GameModel::GameModel() {
    gameMapVec.assign(kBoardSizeNum, vector<int>(kBoardSizeNum, 0)); // 初始化空棋盘
    scoreMapVec.assign(kBoardSizeNum, vector<int>(kBoardSizeNum, 0)); // 初始化评分矩阵
    srand(unsigned(time(nullptr)));  // 设置随机种子（用于AI随机落子）
}

//序列化，保存游戏状态
bool GameModel::saveToFile(const QString &filePath) const
{
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly)) return false;
    QDataStream out(&f);
    // 写入基本状态
    out << static_cast<qint32>(gameType)<< playerFlag;
    // 写入棋盘
    out << qint32(kBoardSizeNum);
    for (int i = 0; i < kBoardSizeNum; i++)
        for (int j = 0; j < kBoardSizeNum; j++)
            out << qint32(gameMapVec[i][j]);
    // 写入历史
    out << qint32(moveHistory.size());
    for (auto &mv : moveHistory) {
        out << qint32(mv.first) << qint32(mv.second);
    }
    return true;
}
//反序列化，加载游戏状态
bool GameModel::loadFromFile(const QString &filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) return false;
    QDataStream in(&f);
    qint32 type; in >> type >> playerFlag;
    gameType = static_cast<GameType>(type);
    // 读棋盘
    qint32 size; in >> size;
    gameMapVec.assign(size, vector<int>(size));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            qint32 v; in >> v;
            gameMapVec[i][j] = int(v);
        }
    // 读历史
    moveHistory.clear();
    qint32 histLen; in >> histLen;
    for (int k = 0; k < histLen; k++) {
        qint32 r, c; in >> r >> c;
        moveHistory.emplace_back(r, c);
    }
    return true;
}
