#ifndef PARAMETER_DIALOG_H
#define PARAMETER_DIALOG_H

#include "parameters_grid_layout.h"

#include <QDialog>

class ParameterDialog : public QDialog
{
public:
    ParameterDialog(QWidget* parent = nullptr);

    ParameterDialog(
        const vcl::proc::ParameterVector& vec,
        const std::string&                title  = "",
        QWidget*                          parent = nullptr);

    vcl::proc::ParameterVector parameters();
};

#endif // PARAMETER_DIALOG_H
