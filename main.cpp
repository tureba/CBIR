#include <QtGui/QApplication>
#include "mainwindow.h"

#include "BD.h"

int main(int argc, char *argv[])
{
	BD bd("teste");
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
