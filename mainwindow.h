#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>

#include "BD.h"

namespace Ui {
	class MainWindow;
}

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void changeEvent(QEvent *e);
	void carrega_imagem(QString nome);

private:
	Ui::MainWindow *ui;
	BD *bd;
	QStringList files;

public slots:
	void enterImagem();
	void abreDialogImagem();
	void enterBD();
	void abreDialogBD();

	void escolheVC(int valor);
	void escolheMetrica(int valor);
	void adicionaImagem();
	void atualizaBD();
};

#endif // MAINWINDOW_H
