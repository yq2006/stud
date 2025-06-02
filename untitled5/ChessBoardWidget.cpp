#include "ChessBoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <cmath>//数学函数

static const int kBoardMargin = 30;//棋盘外边缘的边距
static const int kRadius = 15;     //棋子的半径
static const int kMarkSize = 6;    //落子预览标记的大小
static const int kBlockSize = 40;  //棋盘每个格子的尺寸

ChessBoardWidget::ChessBoardWidget(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);//鼠标追踪
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);//设置固定尺寸
}

//部件的尺寸
QSize ChessBoardWidget::sizeHint() const
{
    int dim = kBoardMargin * 2 + kBlockSize * kBoardSizeNum;//棋盘的边长
    return QSize(dim, dim);
}

//从游戏模型重新加载数据
void ChessBoardWidget::reloadFromModel()
{
    // 重置所有内部坐标、预览等状态
    clickPosRow = clickPosCol = -1;//重置点击位置（设置为无效值）
    update();  // 重新绘制整个棋盘，根据 game->gameMapVec更新棋盘
}

//开始游戏
void ChessBoardWidget::startGame(GameType type)
{
    game.startGame(type);
    clickPosRow = clickPosCol = -1;
    update();
}
