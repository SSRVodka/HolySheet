#pragma once
#include <QtWidgets/QTableWidget>

#include "Cell.h"

#define QT_VERSION_FOR_DATASTREAM QDataStream::Qt_5_13
namespace holySpace {
    constexpr int MagicNumber = 0x7F51C882;
    constexpr int RowCount = ROW_MAXNUM;
    constexpr int ColumnCount = COL_MAXNUM;
};
namespace hsCompare {
    constexpr int KeyCount = 3;
};
using namespace holySpace;
using namespace hsCompare;

/*                      *.hs format
 *  {MagicNumber:32-bits unsigned}{row:16-bits unsigned}{col}{str}
 *  [{row}{col}{str}...]
*/


class HolySheetCompare {
public:
    bool operator()(const QStringList& row1, const QStringList& row2) const;

    int keys[KeyCount];
    bool ascending[KeyCount];
    bool numerical[KeyCount];
};

class HolySheet : public QTableWidget {
    Q_OBJECT
public:
    HolySheet(QWidget* parent = nullptr);
    
    bool isAutoRecalc() const { return autoRecalc; }
    QString currentLocation() const;
    QString currentFormula() const;
    QTableWidgetSelectionRange selectedRange() const;

    void clear();
    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);
    
    void sort(const HolySheetCompare& compare);
public slots:
    void cut();
    void copy();
    void paste();
    void del();
    void selectCurrentRow();    // Required by Menu: Edit -> Select
    void selectCurrentColumn();
    void recalculate();
    void setAutoRecalculate(bool recalc);
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrev(const QString& str, Qt::CaseSensitivity cs);
signals:
    void modified();
private slots:
    void somethingChanged();
private:
    bool autoRecalc;

    Cell* cell(int row, int col) const;
    QString text(int row, int col) const;
    QString formula(int row, int col) const;
    void setFormula(int row, int col, const QString& formula);
};
