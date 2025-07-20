#include "parameter_dialog.h"

#include <QPushButton>

ParameterDialog::ParameterDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Parameters");
}

ParameterDialog::ParameterDialog(
    const hlmp::ParameterVector& vec,
    const std::string&                title,
    QWidget*                          parent) : QDialog(parent)
{
    setWindowTitle(title.c_str());

    vcl::qt::ParametersGridLayout* layout = new vcl::qt::ParametersGridLayout(this);
    layout->setParameters(vec);

    setLayout(layout);
    setModal(true);

    QPushButton* okButton = new QPushButton("Ok", this);
    connect(
        okButton,
        &QPushButton::clicked,
        [&, this]() {
            this->accept();
        });
    layout->addWidget(okButton);
}

hlmp::ParameterVector ParameterDialog::parameters()
{
    hlmp::ParameterVector vec;
    auto layout = dynamic_cast<vcl::qt::ParametersGridLayout*>(this->layout());
    if (layout) {
        vec = layout->parameters();
    }
    return vec;
}
