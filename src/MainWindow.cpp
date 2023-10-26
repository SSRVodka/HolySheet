#include "MainWindow.h"

MainWindow::MainWindow() {
    holySheet = new HolySheet;
    setCentralWidget(holySheet);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();

    fDialog = nullptr;

    setWindowIcon(QIcon(":/imgs/icon.png"));
    setCurrentFile("");     // 当前打开文件为空
}

void MainWindow::createActions() {
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/imgs/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new HolySheet file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon(":/imgs/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a HolySheet file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/imgs/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save current file"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As"), this);
    saveAsAction->setIcon(QIcon(":/imgs/saveAs.png"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save as a specified file"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setIcon(QIcon(":/imgs/exit.png"));
    exitAction->setStatusTip(tr("Exit the application"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/imgs/cut.png"));
    cutAction->setStatusTip(tr("Cut selected contents"));
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), holySheet, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/imgs/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy selected contents"));
    connect(copyAction, SIGNAL(triggered()), holySheet, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/imgs/paste.png"));
    pasteAction->setStatusTip(tr("Paste contents to selected area"));
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), holySheet, SLOT(paste()));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/imgs/delete.png"));
    deleteAction->setStatusTip(tr("Delete selected contents"));
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, SIGNAL(triggered()), holySheet, SLOT(del()));

    selectRowAction = new QAction(tr("&Row"), this);
    selectRowAction->setStatusTip(tr("Select the row of the selected cell"));
    selectRowAction->setShortcut(tr("Ctrl+W"));
    connect(selectRowAction, SIGNAL(triggered()), holySheet, SLOT(selectCurrentRow()));

    selectColumnAction = new QAction(tr("&Column"), this);
    selectColumnAction->setStatusTip(tr("Select the column of the selected cell"));
    selectColumnAction->setShortcut(tr("Ctrl+N"));
    connect(selectColumnAction, SIGNAL(triggered()), holySheet, SLOT(selectCurrentColumn()));
    
    selectAllAction = new QAction(tr("&All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("Select all the cells in the holySheet"));
    // QTableWidget::selectAll() 已提供，无需实现
    connect(selectAllAction, SIGNAL(triggered()), holySheet, SLOT(selectAll()));

    findAction = new QAction(tr("F&ind"), this);
    findAction->setIcon(QIcon(":/imgs/find.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setStatusTip(tr("Find the contents of the holySheet"));
    connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

    goToCellAction = new QAction(tr("&Go To Cell"), this);
    goToCellAction->setIcon(QIcon(":/imgs/goTo.png"));
    goToCellAction->setShortcut(tr("Ctrl+G"));
    goToCellAction->setStatusTip("Go to the specified cell");
    connect(goToCellAction, SIGNAL(triggered()), this, SLOT(goToCell()));

    recalcAction = new QAction(tr("&Recalculate"), this);
    recalcAction->setIcon(QIcon(":/imgs/recalc.png"));
    recalcAction->setShortcut(tr("F9"));
    recalcAction->setStatusTip("Recalculate all the cell immediately");
    connect(recalcAction, SIGNAL(triggered()), holySheet, SLOT(recalculate()));

    sortAction = new QAction(tr("&Sort"), this);
    sortAction->setIcon(QIcon(":/imgs/sort.png"));
    sortAction->setStatusTip(tr("Sort the selected area"));
    connect(sortAction, SIGNAL(triggered()), this, SLOT(sort()));

    showGridAction = new QAction(tr("&Show Grid"), this);
    showGridAction->setCheckable(true);
    // QTableWidget::showGrid() 已提供
    showGridAction->setChecked(holySheet->showGrid());
    showGridAction->setStatusTip(tr("Show or hide the holySheet's grid"));
    // QTableWidget::setShowGrid(bool) 已提供
    connect(showGridAction, SIGNAL(toggled(bool)), holySheet, SLOT(setShowGrid(bool)));

    autoRecalcAction = new QAction(tr("&Auto Recalculate"), this);
    autoRecalcAction->setCheckable(true);
    // HolySheet::isAutoRecalc() 自定义
    autoRecalcAction->setChecked(holySheet->isAutoRecalc());
    autoRecalcAction->setStatusTip(tr("Recalculate the cells automatically"));
    // HolySheet::setAutoRecalculate(bool) 自定义
    connect(autoRecalcAction, SIGNAL(toggled(bool)), holySheet, SLOT(setAutoRecalculate(bool)));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setStatusTip(tr("Show the about box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About Box"));
    // qApp 是 全局宏，这里只是调用它的 aboutQt()
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    sepAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    selectSubMenu = editMenu->addMenu(tr("&Select"));
    selectSubMenu->addAction(selectRowAction);
    selectSubMenu->addAction(selectColumnAction);
    selectSubMenu->addAction(selectAllAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(goToCellAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(recalcAction);
    toolsMenu->addAction(sortAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showGridAction);
    optionsMenu->addAction(autoRecalcAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createContextMenu() {
    holySheet->addAction(cutAction);
    holySheet->addAction(copyAction);
    holySheet->addAction(pasteAction);
    holySheet->addAction(deleteAction);
    holySheet->addAction(sortAction);
    holySheet->addAction(selectRowAction);
    holySheet->addAction(selectColumnAction);
    holySheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars() {
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(goToCellAction);
}

void MainWindow::createStatusBar() {
    locationLabel = new QLabel(" w999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

    // QTableWidget::currentCellChanged() 已提供
    connect(holySheet, SIGNAL(currentCellChanged(int, int, int, int)),
            this, SLOT(updateStatusBar()));
    // 自定义 HolySheet::modified()
    connect(holySheet, SIGNAL(modified()), this, SLOT(sheetModified()));
}

void MainWindow::updateStatusBar() {
    locationLabel->setText(holySheet->currentLocation());
    formulaLabel->setText(holySheet->currentFormula());
}

void MainWindow::sheetModified() {
    setWindowModified(true);
    updateStatusBar();
}

void MainWindow::newFile() {
    if (okToContinue()) {
        holySheet->clear();
        setCurrentFile("");
    }
}

bool MainWindow::okToContinue() {
    if (isWindowModified()) {
        int ans = QMessageBox::warning(this, tr("HolySheet"),
                    tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (ans == QMessageBox::Yes) return save();
        else if (ans == QMessageBox::Cancel) return false;
    }
    return true;
}

void MainWindow::open() {
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open HolySheet"), ".",
                                "HolySheet files (*.hs)");
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::loadFile(const QString& fileName) {
    // TODO: HolySheet::readFile(const QString&)
    if (!holySheet->readFile(fileName)) {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

bool MainWindow::save() {
    if (curFile.isEmpty()) return saveAs();
    else return saveFile(curFile);
}

bool MainWindow::saveFile(const QString& fileName) {
    if (!holySheet->writeFile(fileName)) {
        statusBar()->showMessage(tr("Saving canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this,
                            tr("Save HolySheet"), ".",
                            "HolySheet files (*.hs)");
    if (fileName.isEmpty()) return false;
    
    return saveFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (okToContinue()) {
        writeSettings();
        event->accept();
    }
    else event->ignore();
}

void MainWindow::setCurrentFile(const QString& fileName) {
    curFile = fileName;

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile); // 删除“最近文件”列表中相同名字的文件
        recentFiles.prepend(curFile);   // 在“最近文件”列表最前面加入当前文件名
        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                .arg(tr("HolySheet")));
    setWindowModified(false);
}

QString MainWindow::strippedName(const QString& fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFileActions() {
    QMutableStringListIterator iter(recentFiles);
    
    while (iter.hasNext()) {
        if (!QFile::exists(iter.next()))
            iter.remove();
    }

    for (int i = 0; i < MaxRecentFiles; ++i) {
        // 为什么不直接 0 ~ recentFile.count() ?
        // 因为需要将没有用到的区域 setVisible(false)，如下
        if (i < recentFiles.count()) {
            QString text = tr("&%1 %2").arg(i + 1)
                                        .arg(strippedName(recentFiles[i]));
            recentFileActions[i]->setText(text);
            recentFileActions[i]->setData(recentFiles[i]);
            recentFileActions[i]->setVisible(true);
        }
        else recentFileActions[i]->setVisible(false);
    }
    // 注意，只有有 recentFiles 列表，才需要 File 菜单上的那个分割线
    sepAction->setVisible(!recentFiles.isEmpty());
}

// 解决了 RecentFiles 列表更新问题，现在就能实现打开指定最近文件的功能
void MainWindow::openRecentFile() {
    if (okToContinue()) {
        QAction* specAction = qobject_cast<QAction*>(sender());
        if (specAction) loadFile(specAction->data().toString());
    }
}

// 下面使用之前的对话框类实现 ------------------------------------

void MainWindow::find() {
    if (!fDialog) {
        fDialog = new findDialog(this);
        // TODO: (SLOT) HolySheet::findNext HolySheet::findPrev
        connect(fDialog, SIGNAL(findNext(const QString&, Qt::CaseSensitivity)),
                holySheet, SLOT(findNext(const QString&, Qt::CaseSensitivity)));
        connect(fDialog, SIGNAL(findPrev(const QString&, Qt::CaseSensitivity)),
                holySheet, SLOT(findPrev(const QString&, Qt::CaseSensitivity)));
    }

    // 非模态框
    fDialog->show();
    fDialog->raise();           // 置顶
    fDialog->activateWindow();  //焦点激活
}

void MainWindow::goToCell() {
    goToCellDialog dialog(this);
    // 模态框阻塞
    if (dialog.exec()) {
        QString locationStr = dialog.lineEdit->text().toUpper();
        // QTableWidget::setCurrentCell 已提供
        holySheet->setCurrentCell(locationStr.mid(1).toInt() - 1,
                                    locationStr[0].unicode() - 'A');
    }
}

void MainWindow::sort() {
    sortDialog dialog(this);
    QTableWidgetSelectionRange range = holySheet->selectedRanges()[0];
    // 之前实现的限制 sortDialog 的 column comboBox 取值的函数
    dialog.setColumnRange('A' + range.leftColumn(),
                            'A' + range.rightColumn());
    if (dialog.exec()) {
        HolySheetCompare compare;
        compare.keys[0] = dialog.primaryGBColBox->currentIndex();
        compare.keys[1] = dialog.secondaryGBColBox->currentIndex() - 1;
        compare.keys[2] = dialog.tertiaryGBColBox->currentIndex() - 1;
        compare.ascending[0] = (dialog.primaryGBOrdBox->currentIndex() == 0);
        compare.ascending[1] = (dialog.secondaryGBOrdBox->currentIndex() == 0);
        compare.ascending[2] = (dialog.tertiaryGBOrdBox->currentIndex() == 0);
        compare.numerical[0] = dialog.primaryGBNumCheck->isChecked();
        compare.numerical[1] = dialog.secondaryGBNumCheck->isChecked();
        compare.numerical[2] = dialog.tertiaryGBNumCheck->isChecked();
        // self-defined HolySheet::sort(HolySheetCompare)
        holySheet->sort(compare);
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About HolySheet"),
        tr("<h2>HolySheet 0.1</h2>"
            "<p>Copyright &copy; 2023 SJTU-XHW Inc."
            "HolySheet is a small application that"
            "demonstrates numerous Qt classes,"
            "which is written by C++/Qt.</p>"
            "If you want to learn more about the application, "
            "please visit <a href='https://sjtuxhw.top/myBlog/'>my blog</a>"
            " or visit <a href='https://github.com/SSRVodka'>Github</a> :)"));
}

// 设置的持久化 -----------------------------------------

void MainWindow::writeSettings() {
    QSettings settings("SJTU-XHW Inc.", "HolySheet");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("showGrid", showGridAction->isChecked());
    settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

void MainWindow::readSettings() {
    QSettings settings("SJTU-XHW Inc.", "HolySheet");
    
    restoreGeometry(settings.value("geometry").toByteArray());

    recentFiles = settings.value("recentFils").toStringList();
    updateRecentFileActions();

    bool showGrid = settings.value("showGrid", true).toBool();
    // 这里 showGridAction 已经和 holySheet 连接了，只需改动作的状态就行
    showGridAction->setChecked(showGrid);

    bool autoRecalc = settings.value("autoRecalc", true).toBool();
    autoRecalcAction->setChecked(autoRecalc);
}
