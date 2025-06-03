#include "mainwindow.h"
#include "qapplication.h"/
#include "qlocale.h"
#include "qstyle.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMenuBar>
#include <QAction>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QScreen>
#include "QtDebug"
#include <algorithm>
#include <vector>

static const int kAIDelay = 700;//AI动作延迟时间

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    maxUndos(3),      //初始化悔棋次数
    remainingUndos(3) //初始化剩余悔棋次数
{
    // 1) 基本窗体／字体
    QFont font("Microsoft YaHei", 9);
    QApplication::setFont(font);

    // 2) 创建中心容器 + 垂直布局
    auto *central = new QWidget(this);        //中心部件
    auto *hLay    = new QHBoxLayout(central);//垂直布局

    // 3) 创建棋盘部件
    boardWidget = new ChessBoardWidget(this);//实例化棋盘控件
    QSize sz     = boardWidget->sizeHint();//获取推荐大小
    boardWidget->setFixedSize(sz);//设置棋盘固定尺寸

    // 4) 创建统计面板
    auto *statsWidget = new QWidget(central);//统计信息面板容器
    statsLayout = new QFormLayout(statsWidget);
    statsWidget->setLayout(statsLayout);

    // 5) 把两个 Widget 加入主布局
    hLay->addWidget(boardWidget);          //添加棋盘
    hLay->addWidget(statsWidget);          //添加统计面板
    hLay->setAlignment(Qt::AlignCenter);   //居中对齐
    statsWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);//固定尺寸
    hLay->setContentsMargins(10,10,10,10); //设置布局边距
    hLay->setSpacing(20);                  //设置控件边距

    // 6) 把中央部件设置为主窗口中心部件
    setCentralWidget(central);

    // 7) 创建统计面板内容
    createStatsPanel();


    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->primaryScreen()->availableGeometry()
            )
        );

    // 8) 菜单、信号槽、初始逻辑
    createMenus();//创建菜单栏

    //连接棋盘信号到主窗口槽函数
    connect(boardWidget, &ChessBoardWidget::playerMoved,
            this, &MainWindow::onPlayerMove);
    connect(boardWidget, &ChessBoardWidget::aiMoved,
            this, &MainWindow::onAIMove);
    connect(boardWidget, &ChessBoardWidget::hovered,
            this, &MainWindow::onHover);

    initPVPGame();//初始化双人对战模式
    updateStats();//更新统计数据

    // 让 中央部件 大小固定
    central->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

//初始化双人对战模式
void MainWindow::initPVPGame()
{
    currentMode = PERSON;//设置当前模式为双人对战
    if (currentMode != lastMode)
    {
        // 真正切到双人模式，清零统计
        wins = losses = draws = 0;//清零“胜，负，平”统计
        shortestWin = INT_MAX;
        remainingUndos = maxUndos;
        updateStats();
        updateUndoUI();
    }
    lastMode = currentMode;//更新当前模式
    isMyTurn = true;    // 双人黑方先行
    gameType  = PERSON;
    boardWidget->startGame(gameType);
    boardWidget->reloadFromModel();//刷新棋盘显示
}

//初始化人机对战模式
void MainWindow::initPVEGame()
{
    currentMode = BOT;
    if (currentMode != lastMode)
    {
        // 真正切到人机模式，清零统计
        wins = losses = draws = 0;//清零“胜，负，平”统计
        shortestWin = INT_MAX;
        remainingUndos = maxUndos;
        updateStats();
        updateUndoUI();
    }
    lastMode = currentMode;//保存当前模式
    isMyTurn = true;    // 人机玩家先行
    gameType  = BOT;
    boardWidget->startGame(gameType);
    boardWidget->reloadFromModel();//刷新棋盘显示
}

//窗口大小调整事件
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);//调用基类处理
    auto *lay = centralWidget()->layout();
    if (windowState() & Qt::WindowFullScreen)
    {
        lay->setAlignment(Qt::AlignCenter);// 全屏时，将整个布局的对齐方式改为居中
    }
    else
    {
        lay->setAlignment(Qt::AlignCenter);// 恢复普通窗口时也居中
    }
}

// 鼠标悬停事件
void MainWindow::onHover(int row, int col)
{
    hoverPosLabel->setText(QString("(%1, %2)").arg(row).arg(col));// 在悬停标签显示坐标
}

// 显示移动历史记录
void MainWindow::showHistory()
{
    const auto &h = boardWidget->gameModel().moveHistory;// 获取历史记录

    // 无历史记录时提示
    if (h.empty()) {
        QMessageBox::information(this, tr("History"), tr("No moves yet."));
        return;
    }
    QStringList lines; // 创建字符串列表存放记录

    for (int i = 0; i < h.size(); ++i)
    {
        // 格式化每条记录：序号+行列坐标
        lines << QString("%1. (%2, %3)").arg(i+1).arg(h[i].first).arg(h[i].second);
    }
    QMessageBox::information(this, tr("Move History"),lines.join("\n")); // 对话框显示历史记录
}

// 保存游戏
void MainWindow::saveGame()
{
    QString f = QFileDialog::getSaveFileName// 获取保存文件路径
        (
            this, tr("Save Game"), QString(),
            tr("WuZiQi Save (*.wqg)") // 文件类型过滤器
            );
    if (f.isEmpty()) return;// 用户取消
    if (!boardWidget->gameModel().saveToFile(f)) // 保存到文件
        QMessageBox::warning(this, tr("Error"), tr("Save failed!"));// 失败提示
}

// 加载游戏
void MainWindow::loadGame()
{
    QString f = QFileDialog::getOpenFileName// 获取加载文件路径
        (
            this, tr("Load Game"), QString(),
            tr("WuZiQi Save (*.wqg)")// 文件类型过滤器
            );
    if (f.isEmpty()) return; // 用户取消

    // 从文件加载
    if (boardWidget->gameModel().loadFromFile(f))
    {
        boardWidget->reloadFromModel();// 成功加载后刷新棋
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Load failed!"));// 失败提示
    }
}
