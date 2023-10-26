#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>
#include <QtWidgets/QMessageBox>

#include "HolySheet.h"

bool HolySheetCompare::operator()(
    const QStringList& row1, const QStringList& row2
) const {
    bool ok = true, result;
    for (int i = 0; i < KeyCount; ++i) {
        int column = keys[i];
        if (column != -1) {     // "None" <=> -1
            if (row1[column] != row2[column]) {
                if (ascending[i]) {
                    if (numerical[i])
                        result = row1[column].toDouble(&ok) < row2[column].toDouble(&ok);
                    else result = row1[column] < row2[column];
                    if (ok) return result;
                    // non-numerical but 'numerical' is checked -> regard it as non-numerical.
                    else return row1[column] < row2[column];
                }
                else {
                    if (numerical[i])
                        result = row1[column].toDouble(&ok) > row2[column].toDouble(&ok);
                    else result = row1[column] > row2[column];
                    if (ok) return result;
                    else return row1[column] > row2[column];
                }
            }
        }
    }
    return false;
}



HolySheet::HolySheet(QWidget* parent) : QTableWidget(parent) {
    autoRecalc = true;  // Default

    setItemPrototype(new Cell);
    setSelectionMode(ContiguousSelection);

    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(somethingChanged()));
    clear();
}

QTableWidgetSelectionRange HolySheet::selectedRange() const {
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if (ranges.isEmpty())
        return QTableWidgetSelectionRange();
    return ranges.first();
}

void HolySheet::clear() {
    // In order to clear the contents of the table
    setRowCount(0); setColumnCount(0);
    setRowCount(RowCount); setColumnCount(ColumnCount);

    // Add table headers for every row.
    // Don't need to care about column because 
    // every column has a index by default.
    for (int i = 0; i < ColumnCount; ++i) {
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString(QChar('A' + i)));
        // set header.
        setHorizontalHeaderItem(i, item);
    }
    // Go to (0, 0).
    setCurrentCell(0, 0);
}

Cell* HolySheet::cell(int row, int col) const {
    return static_cast<Cell*>(item(row, col));
}

QString HolySheet::text(int row, int col) const {
    Cell* c = cell(row, col);
    if (c) return c->text();
    else return "";
}

QString HolySheet::formula(int row, int col) const {
    Cell* c = cell(row, col);
    // Use Cell::formula() <=> QTableWidget::data(Qt::EditRole) + setDirty
    if (c) return c->formula();
    else return "";
}

void HolySheet::setFormula(int row, int col, const QString& formula)  {
    Cell* c = cell(row, col);
    if (!c) {
        c = new Cell;
        setItem(row, col, c);
    }
    // Use Cell::setFormula to deal with formula string.
    c->setFormula(formula);
}

QString HolySheet::currentLocation() const {
    // QTableWidget::currentColumn/currentRow => 0-index
    return QString('A' + currentColumn()
                + QString::number(currentRow() + 1));
}

QString HolySheet::currentFormula() const {
    return formula(currentRow(), currentColumn());
}

void HolySheet::somethingChanged() {
    if (autoRecalc) recalculate();  // Realize recalculation.
    emit modified();
}

bool HolySheet::writeFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(
            this, tr("HolySheet"),
            tr("Cannot write file %1:\n%2.")
            .arg(file.fileName())
            .arg(file.errorString())
        );
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QT_VERSION_FOR_DATASTREAM);
    out << quint32(MagicNumber);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    for (int row = 0; row < RowCount; ++row) {
        for (int col = 0; col < ColumnCount; ++col) {
            // Write formula but not text.
            QString str = formula(row, col);
            if (!str.isEmpty())
                out << quint16(row) << quint16(col) << str;
        }
    }

    QApplication::restoreOverrideCursor();
    return true;
}

bool HolySheet::readFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(
            this, tr("HolySheet"),
            tr("Cannot read file %1:\n%2.")
            .arg(file.fileName())
            .arg(file.errorString())
        );
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QT_VERSION_FOR_DATASTREAM);
    quint32 magic; in >> magic;
    if (magic != MagicNumber) {
        QMessageBox::warning(
            this, tr("HolySheet"),
            tr("The file is broken / not a HolySheet file. Abort.")
        );
        return false;
    }
    clear();
    quint16 row, column;
    QString str;
    QApplication::setOverrideCursor(Qt::WaitCursor);

    while (!in.atEnd()) {
        in >> row >> column >> str;
        setFormula(row, column, str);
    }

    QApplication::restoreOverrideCursor();
    return true;
}

void HolySheet::cut() {
    copy(); del();
}

void HolySheet::copy() {
    QTableWidgetSelectionRange range = selectedRange();
    QString str;

    for (int i = 0; i < range.rowCount(); ++i) {
        if (i > 0) str += "\n";
        for (int j = 0; j < range.columnCount(); ++j) {
            if (j > 0) str += "\t";
            // Copy text but not formula.
            str += text(range.topRow() + i, range.leftColumn() + j);
        }
    }

    QApplication::clipboard()->setText(str);
}

void HolySheet::paste() {
    QTableWidgetSelectionRange range = selectedRange();
    QString str = QApplication::clipboard()->text();
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t') + 1;

    // check the area.
    if (range.rowCount() * range.columnCount() != 1
     && (range.rowCount() != numRows || range.columnCount() != numColumns)) {
        QMessageBox::warning(
            this, tr("HolySheet"),
            tr("The contents in the clipboard cannot be"
                " pasted because the contents and the "
                "selected area aren't the same size.")
        );
        return;
    }

    for (int i = 0; i < numRows; ++i) {
        QStringList columns = rows[i].split('\t');
        for (int j = 0; j < numColumns; ++j) {
            int curRow = range.topRow() + i;
            int curColumn = range.leftColumn() + j;
            if (curRow < RowCount && curColumn < ColumnCount)
                setFormula(curRow, curColumn, columns[j]);
        }
    }
    somethingChanged();
}

void HolySheet::del() {
    QList<QTableWidgetItem*> items = selectedItems();
    if (!items.isEmpty()) {
        foreach (QTableWidgetItem* item, items)
            delete item;    // the contents appealed to be cleared
        somethingChanged(); // when items are destructed.
    }
}

void HolySheet::selectCurrentRow() {
    selectRow(currentRow());
}

void HolySheet::selectCurrentColumn() {
    selectColumn(currentColumn());
}

void HolySheet::findNext(const QString& str, Qt::CaseSensitivity cs) {
    // FindNext starts from the same row && next column.
    // Don't worry about 'out of index' problem.
    // (Can you find out the reason?)
    int row = currentRow(), column = currentColumn() + 1;

    while (row < RowCount) {
        while (column < ColumnCount) {
            if (text(row, column).contains(str, cs)) {
                clearSelection();
                setCurrentCell(row, column);
                activateWindow();
                return;
            }
            ++column;
        }
        column = 0; ++row;
    }
    QApplication::beep();
}

void HolySheet::findPrev(const QString& str, Qt::CaseSensitivity cs) {
    int row = currentRow(), column = currentColumn() - 1;

    while (row >= 0) {
        while (column >= 0) {
            if (text(row, column).contains(str, cs)) {
                clearSelection();
                setCurrentCell(row, column);
                activateWindow();
                return;
            }
            --column;
        }
        column = ColumnCount - 1;
        --row;
    }
    QApplication::beep();
}

void HolySheet::recalculate() {
    for (int row = 0; row < RowCount; ++row) {
        for (int column = 0; column < ColumnCount; ++column) {
            if (cell(row, column))
                cell(row, column)->setDirty();
        }
    }
    // Update the whole area, which will redraw the dirty blocks.
    viewport()->update();
}

void HolySheet::setAutoRecalculate(bool recalc) {
    autoRecalc = recalc;
    // Refresh as soon as possible.
    if (autoRecalc) recalculate();
}

void HolySheet::sort(const HolySheetCompare& compare) {
    QList<QStringList> rows;
    QTableWidgetSelectionRange range = selectedRange();
    int i, j;

    for (i = 0; i < range.rowCount(); ++i) {
        QStringList curRow;
        for (j = 0; j < range.columnCount(); ++j) {
            curRow.append(
                formula(range.topRow() + i,
                        range.leftColumn() + j)
            );
        }
        rows.append(curRow);
    }

    qStableSort(rows.begin(), rows.end(), compare);

    for (i = 0; i < range.rowCount(); ++i) {
        for (j = 0; j < range.columnCount(); ++j)
            setFormula(
                range.topRow() + i,
                range.leftColumn() + j,
                rows[i][j]
            );
    }
    clearSelection();
    somethingChanged();
}
