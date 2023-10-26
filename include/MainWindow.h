#pragma once

#define MaxRecentFiles 5

#include <QtWidgets/QMainWindow>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QAction>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QSettings>

#include "findDialog.h"
#include "goToCellDialog.h"
#include "sortDialog.h"
#include "HolySheet.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
protected:
    // Overwrite the close event in order to ask questions
    // before user close the application.
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void find();
    void goToCell();
    void sort();
    void about();

    void openRecentFile();  // 打开最近文件
    void sheetModified();   // 当前文档改变
    void updateStatusBar(); // 更新状态栏

private:
    QWidget* parent;        // the parent widget for MainWindow (app)
    HolySheet* holySheet;   // Central Widget
    findDialog* fDialog;    // 查找对话框可以懒加载，不用立即创建，也不立即删除
    
    QStringList recentFiles;    // 使用 QStringList 存储最近文件列表
    QString curFile;            // 记录当前文件名，被 setCurrentFIle 改变

    // 动作部分 -----------------------------------------------
    QAction* newAction;     // 新建文件动作
    QAction* openAction;    // 打开文件动作
    QAction* saveAction;    // 保存文件动作
    QAction* saveAsAction;  // 另存为动作
    QAction* recentFileActions[MaxRecentFiles]; // 最近文件动作
    QAction* exitAction;    // 退出动作
    QAction* sepAction;     // 分割线动作（视觉作用）

    QAction* cutAction;     // 剪切动作
    QAction* copyAction;    // 复制动作
    QAction* pasteAction;   // 粘贴动作
    QAction* deleteAction;  // 删除动作
    QMenu* selectSubMenu;      // 选择子菜单
    QAction* selectRowAction;   // 选择行动作
    QAction* selectColumnAction;// 选择列动作
    QAction* selectAllAction;   // 全选动作
    QAction* findAction;    // 查找动作
    QAction* goToCellAction;// 切换单元格动作

    QAction* recalcAction;  // 重计算动作
    QAction* sortAction;    // 排序动作

    QAction* showGridAction;// 显示网格动作
    QAction* autoRecalcAction;  // 自动重计算动作

    QAction* aboutAction;   // 关于动作
    QAction* aboutQtAction; // 关于Qt 动作
    // ------------------------------------------------------

    // 菜单栏 ---------------------------
    QMenu* fileMenu;        // File 菜单
    QMenu* editMenu;        // Edit 菜单
    QMenu* toolsMenu;       // Tools 菜单
    QMenu* optionsMenu;     // Options 菜单
    QMenu* helpMenu;        // Help 菜单
    // ---------------------------------

    // 工具栏 -----------------------------
    QToolBar* fileToolBar;  // File 工具栏
    QToolBar* editToolBar;  // Edit 工具栏
    // -----------------------------------

    // 状态栏中组件 ----------
    QLabel* locationLabel;  // 位置标签
    QLabel* formulaLabel;   // 公式计算标签
    // ---------------------

    void createActions();       // 创建动作变量
    void createMenus();         // 创建菜单
    void createContextMenu();   // 创建上下文菜单
    void createToolBars();      // 创建工具栏
    void createStatusBar();     // 创建状态栏

    void readSettings();        // 读取持久化的设置
    void writeSettings();       // 关闭程序前保存设置

    void setCurrentFile(const QString& fileName);   // 设置当前工作区文件
    QString strippedName(const QString& fullFileName);  // 去除文件名的后缀
    bool loadFile(const QString& fileName);         // 加载文件
    bool saveFile(const QString& fileName);         // 保存文件

    // 用户确认是否继续 messageBox
    // Return: continue or not
    bool okToContinue();

    // 更新 RecentFiles 的动作列表
    void updateRecentFileActions();
};