#pragma once
#include <QtWidgets/QTableWidgetItem>

#define ROW_MAXNUM 999
#define COL_MAXNUM 26

class Cell : public QTableWidgetItem {
public:
    Cell();

    void setFormula(const QString& formula);
    QString formula() const;
    void setDirty();

    QTableWidgetItem* clone() const;
    void setData(int role, const QVariant& value);
    QVariant data(int role) const;

private:
    QVariant value() const;

    static QString expandAggExp(const QString& src);

    mutable QVariant cachedValue;
    mutable bool cacheIsDirty;
};
