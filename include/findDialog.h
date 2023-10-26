#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QDialog>

class MainWindow;

class findDialog : public QDialog {
	friend class MainWindow;
	Q_OBJECT
private:
	QLabel* findLabel;
	QLineEdit* lineEdit;
	QCheckBox* matchCheck;
	QCheckBox* backCheck;
	QPushButton* findBtn;
	QPushButton* closeBtn;

signals:
	void findNext(const QString& text, Qt::CaseSensitivity cs);
	void findPrev(const QString& text, Qt::CaseSensitivity cs);
private slots:
	void checkFindButton(const QString& text);
	void findClicked();

public:
	findDialog(QWidget* parent = 0);

};