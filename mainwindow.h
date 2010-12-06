#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>

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

private slots:
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();
};

#endif // MAINWINDOW_H
