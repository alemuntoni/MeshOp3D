// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "main_window.h"

#include <vclib/render/application.h>

#include <qglobal.h>

int main(int argc, char** argv)
{
#ifdef Q_OS_LINUX
    // Workaround for Qt dark theme detection on Linux (Issue #155)
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORMTHEME")) {
        qputenv("QT_QPA_PLATFORMTHEME", "xdgdesktopportal");
    }
#endif

    auto app = vcl::Application(argc, argv);

    mop::MainWindow mw;

    mw.show();
    mw.showMaximized();

    return app.exec();
}
