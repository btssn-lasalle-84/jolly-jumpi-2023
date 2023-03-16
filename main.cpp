#include <QApplication>
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Horse Race");

    QProgressBar *horse1ProgressBar = new QProgressBar();
    QProgressBar *horse2ProgressBar = new QProgressBar();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(horse1ProgressBar);
    layout->addWidget(horse2ProgressBar);
    window.setLayout(layout);

    window.show();

    // Start the race and update the progress bars
    for (int i = 0; i <= 100; i += 10) {
        horse1ProgressBar->setValue(i);
        horse2ProgressBar->setValue(i);

        // Sleep for some time to simulate the race
        QThread::msleep(1000);
    }

    return app.exec();
}
