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

//绘制事件
void ChessBoardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);//创建绘图对象，绑定到当前窗口
    painter.setRenderHint(QPainter::Antialiasing);//启用抗锯齿渲染
    int dim = kBoardMargin * 2 + kBlockSize * kBoardSizeNum;//棋盘尺寸
    int boardSize = kBlockSize * (kBoardSizeNum - 1); // 14个间隔
    QRect boardRect(kBoardMargin, kBoardMargin, boardSize, boardSize);//棋盘位置和大小
    painter.fillRect(boardRect, QColor("#F0D9B5"));// 绘制棋盘背景

    // 绘制网格线时应覆盖整个区域
    for (int i = 0; i < kBoardSizeNum; ++i) {
        int pos = kBoardMargin + i * kBlockSize;//计算当前位置
        painter.drawLine(pos, kBoardMargin, pos, kBoardMargin + boardSize);//绘制垂直线
        painter.drawLine(kBoardMargin, pos, kBoardMargin + boardSize, pos);//绘制水平线
    }

    QBrush brush(Qt::SolidPattern);//创建实心画刷

    // 绘制落子预览标记（鼠标选停在空位置时）
    if (clickPosRow >= 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol >= 0 && clickPosCol < kBoardSizeNum &&
        game.gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        brush.setColor(game.playerFlag ? Qt::white : Qt::black);//设置标记颜色（当前玩家的颜色）
        painter.setBrush(brush);//应用画刷

        //计算预览标记的位置（居中在交叉点上）
        int x = kBoardMargin + clickPosCol * kBlockSize - kMarkSize/2;
        int y = kBoardMargin + clickPosRow * kBlockSize - kMarkSize/2;
        painter.drawRect(x, y, kMarkSize, kMarkSize);//绘制预览标记（小矩形 ）
    }

    // 绘制棋子
    for (int r = 0; r < kBoardSizeNum; ++r) {
        for (int c = 0; c < kBoardSizeNum; ++c) {
            int val = game.gameMapVec[r][c];
            if (val != 0)//确保该位置不为空
            {
                brush.setColor(val == 1 ? Qt::white : Qt::black);//设置棋子颜色（1=白，-1=黑）
                painter.setBrush(brush);//应用画刷

                //计算棋子位置（居中在交叉点上）
                int x = kBoardMargin + c * kBlockSize - kRadius;
                int y = kBoardMargin + r * kBlockSize - kRadius;
                painter.drawEllipse(x, y, kRadius*2, kRadius*2);//绘制棋子（圆形）
            }
        }
    }
}
