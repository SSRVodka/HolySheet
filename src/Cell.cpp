#include <QtWidgets/QMessageBox>

#include "calc.h"
#include "Cell.h"

Cell::Cell() { setDirty(); }

void Cell::setDirty() { cacheIsDirty = true; }

QVariant Cell::data(int role) const {
    // The Caller would like to get the display contents.
    if (role == Qt::DisplayRole) {
        if (value().isValid()) return value().toString();
        else return "####"; // Calculation failure.
    }
    // The Caller wants to optain the alignment of the contents.
    else if (role == Qt::TextAlignmentRole) {
        // pure string in QVariant
        if (value().type() == QVariant::String)
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        // numerical data in QVariant
        else return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    // The caller desires to gain the source data of the cell.
    // Just call the base class of 'Cell': QTableWidgetItem.
    // In QTableWidgetItem, both DisplayRole and EditRole
    // share the same contents. 
    else return QTableWidgetItem::data(role);
}

void Cell::setFormula(const QString& formula) {
    setData(Qt::EditRole, formula);
}

QString Cell::formula() const {
    return data(Qt::EditRole).toString();
}

void Cell::setData(int role, const QVariant& value) {
    // Reuse the function in base class.
    QTableWidgetItem::setData(role, value);
    // set mark for the changed cell (waitting for recalculate).
    if (role == Qt::EditRole)
        setDirty();
}

QVariant Cell::value() const {
    if (cacheIsDirty) {
        cacheIsDirty = false;

        QString formulaStr = formula().toUpper();
        // " ' " is the mark for the pure string.
        if (formulaStr.startsWith('\''))
            cachedValue = formulaStr.mid(1);
        // " = " is the mark for the formula.
        else if (formulaStr.startsWith('=')) {
            cachedValue = QVariant();   // invalid value.
            QRegularExpression exp("([A-Z][1-9][0-9]{0,2})");
            QRegularExpression agg("(AVG|SUM)\\([A-Z][1-9][0-9]{0,2}:[A-Z][1-9][0-9]{0,2}\\)");
            QRegularExpressionMatch curMatch;
            // Expand all the aggregative functions.
            while (true) {
                curMatch = agg.match(formulaStr);
                if (!curMatch.hasMatch()) break;

                QString curSrc = curMatch.captured(0);
                QString target = expandAggExp(curSrc);
                formulaStr.replace(
                    formulaStr.indexOf(curSrc), // offset
                    curSrc.size(),              // length
                    target                      // new string
                );
            }
            // replace all the positions with their values.
            while (true) {
                curMatch = exp.match(formulaStr);
                if (!curMatch.hasMatch()) break;

                QString curPos = curMatch.captured(1);
                int column = curPos[0].unicode() - 'A';
                int row = curPos.mid(1).toInt() - 1;
                Cell* targetCell = static_cast<Cell*>(
                    tableWidget()->item(row, column)
                );
                QString target;
                if (targetCell) target = targetCell->text();
                else target = "0.0";

                formulaStr.replace(
                    formulaStr.indexOf(curPos),
                    curPos.size(),
                    target
                );
            }
            // calculate the formula with calc.h
            try {
                calc calculator(formulaStr.mid(1).toStdString().c_str());
                cachedValue = calculator.getResult();
            } catch (exprErr err) {
                QMessageBox::warning(tableWidget(), QObject::tr("HolySheet"), 
                                    QObject::tr(err.what()));
            }

        }
        else {  // Pure string. But not sure whether it is numerical.
            bool ok;
            double d = formulaStr.toDouble(&ok);
            if (ok) cachedValue = d;        // numerical
            else cachedValue = formulaStr;  // not numerical
        }
    }

    return cachedValue;
}

// Input: "AVG(pos1:pos2)" / "SUM(pos1:pos2)"
QString Cell::expandAggExp(const QString& src) {
    int i, s = src.size(), num = 0; QString ans = "((";
    QStringList posList = src.mid(4, s - 5).split(':');
    if (posList[0][0] == posList[1][0]) {       // Same column.
        int row1 = posList[0].mid(1).toInt() - 1;
        int row2 = posList[1].mid(1).toInt() - 1;
        if (row2 < row1) { int tmp = row2; row2 = row1; row1 = tmp; }
        num = row2 - row1;
        for (i = 1; i <= num && (row1 + i <= ROW_MAXNUM); ++i)
            ans += (QString(posList[0][0]) + QString::number(row1 + i) + '+');
        ans += (QString(posList[0][0]) + QString::number(row1 + i) + ')');
    }
    else {
        int col1 = posList[0][0].unicode() - 'A';
        int col2 = posList[1][0].unicode() - 'A';
        if (col2 < col1) { int tmp = col2; col2 = col1; col1 = tmp; }
        num = col2 - col1;
        for (i = 0; i < num && (col1 + i) < COL_MAXNUM; ++i)
            ans += (QString(col1 + i + 'A') + posList[0].mid(1) + '+');
        ans += (QString(col1 + i + 'A') + posList[0].mid(1) + ')');
    }
    if (src.startsWith('A'))
        ans += (QString('/') + QString::number(num + 1) + ')');
    else ans += QString(')');

    return ans;
}

QTableWidgetItem* Cell::clone() const {
    return new Cell(*this);
}
