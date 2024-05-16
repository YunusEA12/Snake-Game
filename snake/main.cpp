#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Blank GUI");
    window.resize(800, 600);
    window.show();

    return app.exec();
}