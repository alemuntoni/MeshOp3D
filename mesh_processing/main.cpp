#include "mesh_processing_main_window.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    vcl::qt::MeshProcessingMainWindow mw;

    mw.show();
    mw.showMaximized();

    return app.exec();
}
