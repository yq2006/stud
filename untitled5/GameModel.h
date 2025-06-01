#ifndef GAMEMODEL_H
#define GAMEMODEL_H

// ---- 五子棋游戏模型类 ---- //
#include <vector>
#include <utility>
#include <QString>
using namespace std;

// 游戏类型，双人还是AI（目前固定让AI下黑子）
enum GameType
{
    PERSON,
    BOT
};

// 游戏状态
enum GameStatus
{
    PLAYING,
    WIN,
    DEAD
};

// 棋盘尺寸
const int kBoardSizeNum = 15;

class GameModel
{
public:
    GameModel();//构造函数

    //成员变量
    vector<pair<int,int>> moveHistory;//落子历史记录
    vector<vector<int>> gameMapVec; // 存储当前游戏棋盘和棋子的情况,空白为0，白子1，黑子-1
    vector<vector<int>> scoreMapVec; // 存储各个点位的评分情况，作为AI下棋依据
    GameType gameType; // 游戏模式
    GameStatus gameStatus; // 游戏状态
    bool playerFlag; // 标示下棋方（true=玩家回合）
    vector<vector<int>> playerScoreMap;  // 玩家评分矩阵
    vector<vector<int>> aiScoreMap; //AI评分矩阵
    pair<int,int> searchBestMove(int depth = 1);

    // 序列化 / 反序列化
    bool saveToFile(const QString &filePath) const;
    bool loadFromFile(const QString &filePath);

    //成员函数
    void startGame(GameType type); // 开始游戏
    void calculateScore(); // 计算评分
    void actionByPerson(int row, int col); // 人执行下棋
    void actionByAI(int &clickRow, int &clickCol); // 机器执行下棋
    void updateGameMap(int row, int col); // 每次落子后更新游戏棋盘
    bool isWin(int row, int col); // 判断游戏是否胜利
    bool isDeadGame(); // 判断是否和棋
    bool canUndo() const { return !moveHistory.empty(); }
    void undo();
    int checkLine(int row, int col, int dx, int dy, int role);
    int evaluateThreat(int row, int col, int role);
    int  evaluateBoard() const;           // 整盘评估

};

#endif // GAMEMODEL_H
