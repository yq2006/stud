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

// 悔棋操作
void MainWindow::onUndo()
{
    // 检查是否可以悔棋
    if (remainingUndos <= 0 || !boardWidget->gameModel().canUndo())
        return;
    if(gameType == BOT)// 人机模式需要撤销两步
    {
        if (boardWidget->gameModel().moveHistory.size() < 2)
            return;
        boardWidget->gameModel().undo(); // 撤销AI
        boardWidget->gameModel().undo(); // 撤销玩家
    }
    else// 双人模式只需要撤销一步
    {
        if (boardWidget->gameModel().moveHistory.size() < 1)
            return; // 需要添加检查
        boardWidget->gameModel().undo(); // 仅撤销一步
    }

    remainingUndos--;// 减少剩余悔棋次数
    updateUndoUI(); // 更新悔棋UI
    boardWidget->reloadFromModel();// 刷新棋盘显示
    updateStats();// 更新统计数据
}

// 设置最大悔棋次数
void MainWindow::setMaxUndos() {
    bool ok;

    // 弹窗获取用户输入
    int newMax = QInputDialog::getInt(this, "设置悔棋次数","请输入允许的悔棋次数:",
                                      maxUndos,
                                      0,// 最小值
                                      10,// 最大值
                                      1,// 步长
                                      &ok);// 返回是否确认

    if (ok) {
        maxUndos = newMax;// 更新最大悔棋次数
        remainingUndos = maxUndos;// 重置剩余悔棋次数
        updateUndoUI();// 更新UI显示
    }
}

// 更新悔棋UI状态
void MainWindow::updateUndoUI()
{
    // 更新菜单文本显示剩余次数
    QString text = tr("Undo (剩余次数: %1)").arg(remainingUndos);
    undoAction->setText(text);

    undosLabel->setText(QString::number(remainingUndos)); // 更新统计面板标签

    // 设置悔棋按钮是否可用：
    bool canUndo = (remainingUndos > 0) &&     // 还有剩余次数
                   boardWidget->gameModel().canUndo() && // 可以悔棋
                   isMyTurn;                   // 轮到玩家回合
    undoAction->setEnabled(canUndo);
}

// 处理玩家移动
void MainWindow::onPlayerMove(int row, int col)
{
    if(!isMyTurn)// 检查是否轮到玩家
    {
        QMessageBox::warning(this, "提示", "请等待对方落子");
        return;
    }

    lastMoverWasHuman = true;// 标记此步为玩家移动

    boardWidget->gameModel().actionByPerson(row, col);

    checkGameEnd(row, col);// 检查游戏是否结束
    updateStats();// 更新统计数据

    // 人机模式下启动AI计时器
    if (gameType == BOT && !boardWidget->gameModel().playerFlag)
    {
        QTimer::singleShot(kAIDelay, this, &MainWindow::onAIMove);// 延时后触发AI移动
    }

    updateUndoUI();
}

// 处理AI移动
void MainWindow::onAIMove()
{
    lastMoverWasHuman = false;// 标记此步为AI移动
    int r, c;                 // AI落子位置
    boardWidget->gameModel().actionByAI(r, c);// AI决策并落子
    checkGameEnd(r, c);// 检查游戏是否结束
    updateStats();// 更新统计数据
    updateUndoUI();// 更新悔棋UI
}

// 检查游戏是否结束（胜利/平局）
void MainWindow::checkGameEnd(int row, int col)
{
    GameModel *g = &boardWidget->gameModel(); // 获取游戏模型

    // 胜利判断
    if (g->isWin(row, col)) {
        bool winSideBlack = (g->gameMapVec[row][col] == -1);// 判断哪一方赢（黑棋为 -1，白棋为 +1）
        bool localWin = (winSideBlack && myColor == "BLACK")
                        || (!winSideBlack && myColor == "WHITE");// 判断是否是本机玩家胜利

        // 更新本地胜负统计
        if (localWin) wins++;
        else          losses++;

        // 记录历史到文件
        QFile file("history.log");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);  // 声明 out 变量
            bool winSideBlack = (g->gameMapVec[row][col] == -1);
            QString winner = winSideBlack ? "黑棋赢了" : "白棋赢了";
            out << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm") << "] "<< winner << "\n";
        }

        // 更新最短胜利步数记录
        int currentSteps = g->moveHistory.size();
        if (currentSteps < shortestWin) {
            shortestWin = currentSteps;
        }

        // 生成胜利消息
        QString message;
        if (winSideBlack)
        {
            message = localWin ? "赢了(黑棋)" : "赢了(黑棋)";
        } else
        {
            message = localWin ? "赢了(白棋)" : "赢了(白棋)";
        }
        QMessageBox::information(this, "结束", message);// 显示胜利信息
        updateStats();// 更新统计数据

        isMyTurn = true;

        // 1秒后重置游戏
        boardWidget->setEnabled(false);// 禁用棋盘
        QTimer::singleShot(1000, [this]()
                           {
                               boardWidget->setEnabled(true);// 启用棋盘
                           });
        g->startGame(g->gameType);// 重新开始游戏
        boardWidget->reloadFromModel();// 刷新棋盘
        g->calculateScore(); // 重新计算分数
        updateStats();                // 更新统计
        updateUndoUI();              // 更新悔棋UI
    }
    // 平局判断
    else if (g->isDeadGame()) {
        draws++;// 增加平局计数
        QMessageBox::information(this, "结束", "平局"); // 显示平局信息
        updateStats();// 更新统计数据

        // 1秒后重置游戏（同上）
        boardWidget->setEnabled(false);
        QTimer::singleShot(1000, [this]()
                           {
                               boardWidget->setEnabled(true);
                           });
        g->startGame(g->gameType);
        boardWidget->reloadFromModel();
        g->calculateScore();
        updateStats();
    }

    updateUndoUI();
    qDebug() << "当前步数:" << g->moveHistory.size()
             << " 最短步数记录:" << shortestWin;
}

// 创建菜单系统
void MainWindow::createMenus()
{
    // 创建"模式"菜单
    QMenu *gameMenu = menuBar()->addMenu(tr("模式"));
    QAction *pvp = gameMenu->addAction(tr("PVP"));// 双人对战
    QAction *pve = gameMenu->addAction(tr("PVE"));// 人机对战
    connect(pvp, &QAction::triggered, this, &MainWindow::initPVPGame);
    connect(pve, &QAction::triggered, this, &MainWindow::initPVEGame);

    // 创建"记录"菜单
    QMenu *recMenu = menuBar()->addMenu(tr("记录"));
    QAction *hist = recMenu->addAction(tr("历史记录"));
    QAction *save = recMenu->addAction(tr("保存游戏"));
    QAction *load = recMenu->addAction(tr("加载游戏"));
    connect(hist, &QAction::triggered, this, &MainWindow::showHistory);
    connect(save, &QAction::triggered, this, &MainWindow::saveGame);
    connect(load, &QAction::triggered, this, &MainWindow::loadGame);

    // 创建“悔棋”菜单
    QMenu *ctrlMenu = menuBar()->addMenu(tr("悔棋"));
    undoAction = ctrlMenu->addAction(tr("悔棋 (剩余次数: %1)").arg(remainingUndos));
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);

    QAction *setUndosAction = ctrlMenu->addAction(tr("设置悔棋次数"));
    connect(setUndosAction, &QAction::triggered, this, &MainWindow::setMaxUndos);
}

// 创建统计面板界面组件
void MainWindow::createStatsPanel()
{
    // 初始化各种统计标签并设置默认值
    undosLabel = new QLabel("3");              // 悔棋次数标签，默认值3
    winsLabel = new QLabel("0");               // 胜场统计标签，默认值0
    lossesLabel = new QLabel("0");             // 败场统计标签，默认值0
    drawsLabel = new QLabel("0");              // 平局统计标签，默认值0
    shortestWinLabel = new QLabel("—");        // 最短获胜步数标签，默认值"-"
    currentStepsLabel = new QLabel("0 步");    // 当前步数标签，显示如"0步"
    hoverPosLabel = new QLabel("(—, —)");      // 鼠标悬停位置标签，默认"(-，-)"
    lastMoveLabel = new QLabel("(—, —)");      // 最后落子位置标签，默认"(-，-)"
    myScoreLabel = new QLabel("0.0");
    opponentScoreLabel = new QLabel("0.0");
    evalLabel = new QLabel("—");               // 局势评估标签，默认值"一"(无评估)


    // 将标签添加到网格布局中，左侧为描述文字，右侧为数值标签
    statsLayout->addRow("剩余悔棋:", undosLabel);
    statsLayout->addRow("胜场:", winsLabel);
    statsLayout->addRow("负场:", lossesLabel);
    statsLayout->addRow("平局:", drawsLabel);
    statsLayout->addRow("最短胜步数:", shortestWinLabel);
    statsLayout->addRow("当前走了:", currentStepsLabel);
    statsLayout->addRow("鼠标行列:", hoverPosLabel);
    statsLayout->addRow("最后走:", lastMoveLabel);
    statsLayout->addRow("局势评估:", evalLabel);
}

//更新统计面板的所有数据
void MainWindow::updateStats()
{
    GameModel *g = &boardWidget->gameModel();// 获取游戏模型引用
    g->calculateScore(); // // 重新计算棋盘所有位置的评分

    winsLabel->        setText(QString::number(wins));
    lossesLabel->      setText(QString::number(losses));
    drawsLabel->       setText(QString::number(draws));

    // 更新最短获胜步数标签
    shortestWinLabel-> setText(
        shortestWin<INT_MAX? QString::number(shortestWin) : QString("!")
        );
    currentStepsLabel->setText(
        QString("%1 步!").arg(g->moveHistory.size())
        );

    // 更新最后移动位置标签：
    if (!g->moveHistory.empty())
    {
        auto mv = g->moveHistory.back();// 获取最后一步记录
        QString who;// 确定最后落子方身份
        if (g->gameType == BOT)// 人机对战模式
        {
            who = (g->gameMapVec[mv.first][mv.second] == 1) ? "您" : "电脑";
        }
        else// 双人对战模式
        {
            who = (g->gameMapVec[mv.first][mv.second] == -1) ? "黑方" : "白方";
        }
        lastMoveLabel->setText(QString("%1 在 (%2, %3)").arg(who).arg(mv.first).arg(mv.second));
    }

    // 计算己方和对手的最佳分数
    int maxMine = 0, maxOpp = 0;
    g->calculateScore();
    // 遍历整个棋盘
    for (int i = 0; i < kBoardSizeNum; i++) {
        for (int j = 0; j < kBoardSizeNum; j++) {
            int s = g->scoreMapVec[i][j];// 获取当前位置评分

            // 确定当前位置是否属于己方棋子
            bool isMineStone = (g->playerFlag ? (g->gameMapVec[i][j] == 1)
                                              : (g->gameMapVec[i][j] == -1));
            bool isOppStone = (g->playerFlag ? (g->gameMapVec[i][j] == -1)
                                             : (g->gameMapVec[i][j] == 1));

            // 确定当前位置是否属于对手棋子
            if ( isMineStone || (g->gameMapVec[i][j] == 0 && s > maxMine) )
            {
                maxMine = qMax(maxMine, s);
            }
            // 对手：同理
            if ( isOppStone || (g->gameMapVec[i][j] == 0 && s > maxOpp) )
            {
                maxOpp = qMax(maxOpp, s);
            }
        }
    }

    myScoreLabel->setText(QString::number(maxMine/1.0,'f',1));
    opponentScoreLabel->setText(QString::number(maxOpp/1.0,'f',1));


    // **整体评估**
    int evalScore = g->evaluateBoard();
    QString evalStr;
    if (std::abs(evalScore) > 20000) {
        evalStr = evalScore > 0
                      ? "黑方绝对优势"
                      : "白方绝对优势";
    } else if (std::abs(evalScore) > 5000) {
        evalStr = evalScore > 0
                      ? (g->gameType == BOT ? "劣势" : "黑方优势")
                      : (g->gameType == BOT ? "优势" : "白方优势");
    } else {
        evalStr = "局势平衡";
    }
    evalLabel->setText(evalStr);

    // 3) 可视化一下全盘分差
    qDebug() << "全盘评估分:" << evalScore << " 评估:" << evalStr;
}
