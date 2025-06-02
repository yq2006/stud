#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ChessBoardWidget.h"
#include "StartMenu.h"
#include "qformlayout.h"
#include <QLabel>
#include <QFont>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    void initPVPGame();//初始化PVP
    void initPVEGame();//初始化PVE

    void resizeEvent(QResizeEvent* event) override;//重写窗口大小改变处理事件

private slots://私有槽函数
    void onPlayerMove(int row, int col);
    void onAIMove();
    void onHover(int row, int col);
    void checkGameEnd(int row, int col);

    //菜单相关槽函数
    void showHistory();
    void saveGame();
    void loadGame();
    void onUndo();
    void setMaxUndos();
    void updateUndoUI();

private:
    //私有成员函数
    void createMenus();      //创建菜单栏
    void createStatsPanel(); //创建统计信息
    void updateStats();      //更新统计数据

    //私有成员变量
    ChessBoardWidget *boardWidget;//棋盘部件
    GameType gameType;//游戏类型（PVP/PVE)

    bool isMyTurn = false;
    QString myColor;

    StartMenu* startMenu = nullptr;
    QAction* showStartMenuAction;

    QFormLayout *statsLayout;

    //悔棋相关变量
    int maxUndos = 3;        // 最大悔棋次数(默认3)
    int remainingUndos = 3;  // 剩余悔棋次数(默认3)
    QAction *undoAction;     // 悔棋菜单项
    QLabel *undosLabel;      // 显示剩余次数

    //游戏状态记录
    GameType  currentMode;    // 记录当前游戏模式
    GameType  lastMode = PERSON;//记录上一次游戏模式（默认PVP)

    //统计数据
    int wins = 0;                   //胜利次数
    int losses = 0;                 //失败次数
    int draws = 0;                  //平局次数
    int shortestWin = INT_MAX;      //最短获胜步数（默认最大整数）
    bool lastMoverWasHuman = true;  //标记上一步是否为人类玩家

    // --- UI 控件 ---
    QLabel *winsLabel;         //胜利次数
    QLabel *lossesLabel;       //失败次数
    QLabel *drawsLabel;        //平局次数
    QLabel *shortestWinLabel;  //最短获胜步数
    QLabel *currentStepsLabel; //当前步数
    QLabel *hoverPosLabel;     //鼠标悬停位置
    QLabel *lastMoveLabel;     //上一步位置
    QLabel *myScoreLabel;      //玩家得分
    QLabel *opponentScoreLabel;//对手得分
    QLabel *evalLabel;         //评估局面
};

#endif
