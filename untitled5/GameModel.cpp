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

//构造函数
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

//游戏开始
void GameModel::startGame(GameType type) {
    gameType = type;
    moveHistory.clear();
    // 清空棋盘和评分矩阵
    gameMapVec.clear();
    for (int i = 0; i < kBoardSizeNum; i++) {
        gameMapVec.push_back(vector<int>(kBoardSizeNum, 0));
        scoreMapVec.push_back(vector<int>(kBoardSizeNum, 0));
    }

    if (gameType == BOT) calculateScore();// AI模式初始化评分
    playerFlag = (type == BOT); // 设置先手（AI先下时为false）
}

//更新棋盘状态
void GameModel::updateGameMap(int row, int col) {
    gameMapVec[row][col] = playerFlag ? 1 : -1; // 玩家下白子(1)，AI下黑子(-1)
    playerFlag = !playerFlag;                   // 切换回合
    moveHistory.emplace_back(row, col);         // 记录落子
}

//玩家落子处理
void GameModel::actionByPerson(int row, int col) {
    updateGameMap(row, col);  // 更新棋盘
    calculateScore();         // 重新计算AI评分（为AI决策做准备）
}

//AI落子处理
void GameModel::actionByAI(int &clickRow, int &clickCol)
{
    calculateScore();// 计算评分

    // 从评分中找出最大分数的位置
    int maxScore = 0;
    vector<pair<int, int>> maxPoints;

    for (int row = 0; row < kBoardSizeNum; row++)
        for (int col = 0; col < kBoardSizeNum; col++)
        {
            // 前提是这个坐标是空的
            if (gameMapVec[row][col] == 0)
            {
                if (scoreMapVec[row][col] > maxScore)          // 找最大的数和坐标
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(make_pair(row, col));
                }
                else if (scoreMapVec[row][col] == maxScore)     // 如果有多个最大的数，都存起来
                    maxPoints.push_back(make_pair(row, col));
            }
        }

    // 随机落子，如果有多个点的话
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();
    auto mv = searchBestMove(1);
    clickRow = mv.first;
    clickCol = mv.second;
    pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first; // 记录落子点
    clickCol = pointPair.second;
    updateGameMap(clickRow, clickCol);
    calculateScore();
}

//计算评分
void GameModel::calculateScore()
{
    // 1. 清空并重建评分矩阵
    scoreMapVec.assign(kBoardSizeNum,vector<int>(kBoardSizeNum, 0));

    // 2. 遍历棋盘上每一个格子（包括空格和已有棋子）
    for (int row = 0; row < kBoardSizeNum; ++row) {
        for (int col = 0; col < kBoardSizeNum; ++col) {
            // 我们对所有格子都计算潜力，所以无需跳过已落子的位置

            int whiteScore = 0;
            int blackScore = 0;

            // 3. 朝八个方向评估
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dy == 0 && dx == 0) continue;

                    // —— 白棋（玩家）的方向评估 ——
                    int pCount = 0, pBlocks = 0;
                    bool pEmpty1 = false, pEmpty2 = false;

                    // 正方向
                    for (int k = 1; k <= 4; ++k) {
                        int y = row + dy * k;
                        int x = col + dx * k;
                        if (y < 0 || y >= kBoardSizeNum ||
                            x < 0 || x >= kBoardSizeNum) {
                            ++pBlocks;
                            break;
                        }
                        if (gameMapVec[y][x] == 1) {
                            ++pCount;
                        } else if (gameMapVec[y][x] == -1) {
                            ++pBlocks;
                            break;
                        } else {
                            if (k == 1) pEmpty1 = true;
                            break;
                        }
                    }
                    // 反方向
                    for (int k = 1; k <= 4; ++k) {
                        int y = row - dy * k;  // 修正：使用“-”
                        int x = col - dx * k;  // 修正：使用“-”
                        if (y < 0 || y >= kBoardSizeNum ||
                            x < 0 || x >= kBoardSizeNum) {
                            ++pBlocks;
                            break;
                        }
                        if (gameMapVec[y][x] == 1) {
                            ++pCount;
                        } else if (gameMapVec[y][x] == -1) {
                            ++pBlocks;
                            break;
                        } else {
                            if (k == 1) pEmpty2 = true;
                            break;
                        }
                    }
                    // 根据棋型累加白棋分
                    if (pCount >= 4)
                    {
                        // 白棋连续4子及以上
                        if (pBlocks == 0) {            // 无阻挡：五连棋型
                            whiteScore += FIVE;
                        } else if (pBlocks == 1) {     // 单边阻挡：双冲四
                            whiteScore += BLOCK_FOUR * 2;
                        } else {                       // 双边阻挡：无效棋型
                            // 不增加分数
                        }
                    }
                    else if (pCount == 3)
                    {
                        // 白棋连续3子
                        if (pBlocks == 0) {            // 无阻挡
                            if (pEmpty1 && pEmpty2) { // 两端都有空位：活四
                                whiteScore += LIVE_FOUR;
                            } else {                   // 仅一端有空位：冲四
                                whiteScore += BLOCK_FOUR;
                            }
                        } else if (pBlocks == 1) {     // 单边阻挡：半冲四
                            whiteScore += BLOCK_FOUR / 2;
                        }
                    }
                    else if (pCount == 2)
                    {
                        // 白棋连续2子
                        if (pBlocks == 0) {            // 无阻挡
                            if (pEmpty1 && pEmpty2) { // 两端有空位：双活三
                                whiteScore += DUAL_LIVE_THREE;
                            } else if (pEmpty1 || pEmpty2) { // 单端有空位：活三
                                whiteScore += LIVE_THREE;
                            }
                        } else if (pBlocks == 1) {     // 单边阻挡：死三
                            whiteScore += DEAD_THREE;
                        }
                    }
                    else if (pCount == 1)
                    {
                        // 白棋单子
                        if (pBlocks == 0 && (pEmpty1 || pEmpty2)) { // 活二
                            whiteScore += LIVE_TWO;
                        }
                    }

                    // —— 黑棋（AI）的方向评估 ——
                    int bCount = 0, bBlocks = 0;
                    bool bEmpty1 = false, bEmpty2 = false;

                    // 正方向
                    for (int k = 1; k <= 4; ++k) {
                        int y = row + dy * k;
                        int x = col + dx * k;
                        if (y < 0 || y >= kBoardSizeNum ||
                            x < 0 || x >= kBoardSizeNum) {
                            ++bBlocks;
                            break;
                        }
                        if (gameMapVec[y][x] == -1) {
                            ++bCount;
                        } else if (gameMapVec[y][x] == 1) {
                            ++bBlocks;
                            break;
                        } else {
                            if (k == 1) bEmpty1 = true;
                            break;
                        }
                    }
                    // 反方向
                    for (int k = 1; k <= 4; ++k) {
                        int y = row - dy * k;
                        int x = col - dx * k;
                        if (y < 0 || y >= kBoardSizeNum ||
                            x < 0 || x >= kBoardSizeNum) {
                            ++bBlocks;
                            break;
                        }
                        if (gameMapVec[y][x] == -1) {
                            ++bCount;
                        } else if (gameMapVec[y][x] == 1) {
                            ++bBlocks;
                            break;
                        } else {
                            if (k == 1) bEmpty2 = true;
                            break;
                        }
                    }
                    // 根据棋型累加黑棋分
                    if (pCount >= 4)
                    {
                        // 白棋连续4子及以上
                        if (pBlocks == 0) {            // 无阻挡：五连棋型
                            blackScore += FIVE;
                        } else if (pBlocks == 1) {     // 单边阻挡：双冲四
                            blackScore += BLOCK_FOUR * 2;
                        } else {                       // 双边阻挡：无效棋型
                            // 不增加分数
                        }
                    }
                    else if (pCount == 3)
                    {
                        // 白棋连续3子
                        if (pBlocks == 0) {            // 无阻挡
                            if (pEmpty1 && pEmpty2) { // 两端都有空位：活四
                                blackScore += LIVE_FOUR;
                            } else {                   // 仅一端有空位：冲四
                                blackScore += BLOCK_FOUR;
                            }
                        } else if (pBlocks == 1) {     // 单边阻挡：半冲四
                            blackScore += BLOCK_FOUR / 2;
                        }
                    }
                    else if (pCount == 2)
                    {
                        // 白棋连续2子
                        if (pBlocks == 0) {            // 无阻挡
                            if (pEmpty1 && pEmpty2) { // 两端有空位：双活三
                                blackScore += DUAL_LIVE_THREE;
                            } else if (pEmpty1 || pEmpty2) { // 单端有空位：活三
                                blackScore += LIVE_THREE;
                            }
                        } else if (pBlocks == 1) {     // 单边阻挡：死三
                            blackScore += DEAD_THREE;
                        }
                    }
                    else if (pCount == 1)
                    {
                        // 白棋单子
                        if (pBlocks == 0 && (pEmpty1 || pEmpty2)) { // 活二
                            blackScore += LIVE_TWO;
                        }
                    }
                }
            }

            // 4. 将所有方向的分数累加起来，取两者中较大者为该点最终潜力
            scoreMapVec[row][col] = max(whiteScore, blackScore);
        }
    }
}

bool GameModel::isWin(int row, int col)
{
    // 获取当前棋子颜色（1=白，-1=黑）
    int current = gameMapVec[row][col];
    if (current == 0) return false;//空白位置返回false

    // 四个检测方向：水平、垂直、左斜、右斜
    int directions[4][2] = {{0,1}, {1,0}, {1,1}, {1,-1}};

    for (auto &dir : directions) {
        int dy = dir[0], dx = dir[1];
        int count = 1;

        // 向正方向检测
        for (int k = 1; k < 5; ++k) {
            int y = row + dy*k;
            int x = col + dx*k;
            if (y < 0 || y >= kBoardSizeNum || x < 0 || x >= kBoardSizeNum)break;//越界则终止检测
            if (gameMapVec[y][x] != current) break;//颜色不同则终止检测
            count++;
        }

        // 向反方向检测
        for (int k = 1; k < 5; ++k) {
            int y = row - dy*k;
            int x = col - dx*k;
            if (y < 0 || y >= kBoardSizeNum || x < 0 || x >= kBoardSizeNum)break;//越界则终止检测
            if (gameMapVec[y][x] != current) break;//颜色不同则终止检测
            count++;
        }

        if (count >= 5) return true;
    }
    return false;
}

//死局判断（棋盘已满而无人获胜）
bool GameModel::isDeadGame()
{
    // 所有空格全部填满
    for (int i = 1; i < kBoardSizeNum; i++)
        for (int j = 1; j < kBoardSizeNum; j++)
        {   //若存在空白格（0），则游戏未结束
            if (!(gameMapVec[i][j] == 1 || gameMapVec[i][j] == -1))
                return false;
        }
    return true;
}

//悔棋
void GameModel::undo()
{
    if (moveHistory.empty()) return;//无历史记录则退出

    auto lastMove = moveHistory.back();// 获取最后一步落子位置
    moveHistory.pop_back();//移除该记录

    gameMapVec[lastMove.first][lastMove.second] = 0;// 重置棋盘，使该位置为空（0）

    playerFlag = !playerFlag;// 切换玩家标志（回退到上一步的玩家回合）
}

//棋局评估
int GameModel::evaluateBoard() const
{
    int totalWhite = 0, totalBlack = 0;// 遍历所有格子，累加每个点对白/黑的棋型评分

    for (int row = 0; row < kBoardSizeNum; ++row) {
        for (int col = 0; col < kBoardSizeNum; ++col) {
            int whiteScore = 0, blackScore = 0;// 对空格或已落子格，都做方向评分

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dy==0 && dx==0) continue;
                    // —— 白棋方向评分 ——
                    int pCount=0, pBlocks=0;//连续白子数，阻挡数
                    bool pE1=false, pE2=false;//两端是否为空
                    for (int k=1; k<=4; ++k) {
                        int y=row+dy*k, x=col+dx*k;
                        if (y<0||y>=kBoardSizeNum||x<0||x>=kBoardSizeNum) { ++pBlocks; break; }
                        if (gameMapVec[y][x]==1) ++pCount;
                        else if (gameMapVec[y][x]==-1) { ++pBlocks; break; }
                        else { if (k==1) pE1=true; break; }
                    }
                    for (int k=1; k<=4; ++k) {
                        int y=row-dy*k, x=col-dx*k;
                        if (y<0||y>=kBoardSizeNum||x<0||x>=kBoardSizeNum) { ++pBlocks; break; }
                        if (gameMapVec[y][x]==1) ++pCount;
                        else if (gameMapVec[y][x]==-1) { ++pBlocks; break; }
                        else { if (k==1) pE2=true; break; }
                    }
                    // 累加白棋分
                    if      (pCount>=4) whiteScore += (pBlocks==0?FIVE:(pBlocks==1?BLOCK_FOUR*2:0));
                    else if (pCount==3) whiteScore += (pBlocks==0?(pE1&&pE2?LIVE_FOUR:BLOCK_FOUR):(BLOCK_FOUR/2));
                    else if (pCount==2) whiteScore += (pBlocks==0?(pE1&&pE2?DUAL_LIVE_THREE:(pE1||pE2?LIVE_THREE:0)):DEAD_THREE);
                    else if (pCount==1) if (pBlocks==0&&(pE1||pE2)) whiteScore += LIVE_TWO;

                    // —— 黑棋方向评分 ——
                    int bCount=0, bBlocks=0; bool bE1=false, bE2=false;
                    for (int k=1; k<=4; ++k) {
                        int y=row+dy*k, x=col+dx*k;
                        if (y<0||y>=kBoardSizeNum||x<0||x>=kBoardSizeNum) { ++bBlocks; break; }
                        if (gameMapVec[y][x]==-1) ++bCount;
                        else if (gameMapVec[y][x]==1) { ++bBlocks; break; }
                        else { if (k==1) bE1=true; break; }
                    }
                    for (int k=1; k<=4; ++k) {
                        int y=row-dy*k, x=col-dx*k;
                        if (y<0||y>=kBoardSizeNum||x<0||x>=kBoardSizeNum) { ++bBlocks; break; }
                        if (gameMapVec[y][x]==-1) ++bCount;
                        else if (gameMapVec[y][x]==1) { ++bBlocks; break; }
                        else { if (k==1) bE2=true; break; }
                    }
                    // 累加黑棋分
                    if      (bCount>=4) blackScore += (bBlocks==0?FIVE:(bBlocks==1?BLOCK_FOUR*2:0));
                    else if (bCount==3) blackScore += (bBlocks==0?(bE1&&bE2?LIVE_FOUR:BLOCK_FOUR):(BLOCK_FOUR/2));
                    else if (bCount==2) blackScore += (bBlocks==0?(bE1&&bE2?DUAL_LIVE_THREE:(bE1||bE2?LIVE_THREE:0)):DEAD_THREE);
                    else if (bCount==1) if (bBlocks==0&&(bE1||bE2)) blackScore += LIVE_TWO;
                }
            }

            // 如果该格已经有子，再给一个成五的权重
            if (gameMapVec[row][col]==1)  whiteScore += FIVE;
            if (gameMapVec[row][col]==-1) blackScore += FIVE;

            totalWhite += whiteScore;
            totalBlack += blackScore;
        }
    }
    return   totalBlack - totalWhite; //返回黑白之差，数值越高，黑棋越占优势
}

//AI决策
pair<int, int> GameModel::searchBestMove(int depth)
{
    int bestScore = numeric_limits<int>::min();//初始化为最小整数
    pair<int,int> bestMove{-1,-1};

    // 候选落点：这里简单遍历所有空格
    for (int r=0; r<kBoardSizeNum; ++r) {
        for (int c=0; c<kBoardSizeNum; ++c) {
            if (gameMapVec[r][c]!=0) continue;//跳过非空格

            //模拟落子
            gameMapVec[r][c] = -1;           // AI 下黑子（-1）
            int score = evaluateBoard();
            gameMapVec[r][c] = 0;//恢复空白

            //更新最佳落子
            if (score > bestScore) {
                bestScore = score;
                bestMove = {r,c};
            }
        }
    }
    //如果没找到，随便返回
    if (bestMove.first<0) {
        int i=rand()%kBoardSizeNum, j=rand()%kBoardSizeNum;
        bestMove = {i,j};
    }
    return bestMove;
}



