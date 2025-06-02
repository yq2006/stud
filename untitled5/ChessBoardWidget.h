#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include <vector>
#include "GameModel.h"

class ChessBoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoardWidget(QWidget *parent = nullptr);//构造函数
    void startGame(GameType type);
    GameModel& gameModel() { return game; }// 提供对外的游戏模型访问
    QSize sizeHint() const override;//Qt基础方法重写
    void reloadFromModel();//状态刷新

    bool m_isMyTurn;//玩家状态

signals:
    void hovered(int row, int col);//鼠标移动
    void playerMoved(int row, int col);//玩家落子完成
    void aiMoved(int row, int col);//AI计算完成
    void scoresUpdated(int playerScore, int aiScore);//分数变化

protected:
    //关键事件重写
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    GameModel game;  // 游戏逻辑模型
    int clickPosRow{-1}, clickPosCol{-1};//当前点击位置（-1为无效值）
    void updateClickPosition(int x, int y);
};

#endif // CHESSBOARDWIDGET_H
