// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "parameters_grid_layout.h"

#include "parameters/bool_parameter_row.h"
#include "parameters/enum_parameter_row.h"
#include "parameters/scalar_parameter_row.h"
#include "parameters/uint_parameter_row.h"
#include "parameters/uscalar_parameter_row.h"
#include "parameters/point3_parameter_row.h"

namespace mop {

ParametersGridLayout::ParametersGridLayout(QWidget* parent) :
        QGridLayout(parent)
{
}

void ParametersGridLayout::setParameters(
    const ParameterVector& parameters)
{
    clear();
    mParameters = parameters;

    uint row = 0;
    for (const std::shared_ptr<const Parameter>& par : parameters) {
        if (par) {
            bool                          paramInserted = false;
            std::shared_ptr<ParameterRow> parRow;

            switch (par->type()) {
            case ParameterType::BOOL: {
                std::shared_ptr<const BoolParameter> bp =
                    std::dynamic_pointer_cast<const BoolParameter>(par);
                parRow = std::make_shared<BoolParameterRow>(*bp);

                paramInserted = true;

            } break;
            case ParameterType::ENUM: {
                std::shared_ptr<const EnumParameter> ep =
                    std::dynamic_pointer_cast<const EnumParameter>(par);
                parRow = std::make_shared<EnumParameterRow>(*ep);

                paramInserted = true;
            } break;
            case ParameterType::SCALAR: {
                std::shared_ptr<const ScalarParameter> sp =
                    std::dynamic_pointer_cast<const ScalarParameter>(par);
                parRow = std::make_shared<ScalarParameterRow>(*sp);

                paramInserted = true;
            } break;
            case ParameterType::UINT: {
                std::shared_ptr<const UintParameter> up =
                    std::dynamic_pointer_cast<const UintParameter>(par);
                parRow = std::make_shared<UintParameterRow>(*up);

                paramInserted = true;
            } break;
            case ParameterType::USCALAR: {
                std::shared_ptr<const UscalarParameter> usp =
                    std::dynamic_pointer_cast<const UscalarParameter>(
                        par);
                parRow = std::make_shared<UscalarParameterRow>(*usp);

                paramInserted = true;
            } break;
            case ParameterType::POINT3: {
                std::shared_ptr<const Point3Parameter> ptp =
                    std::dynamic_pointer_cast<const Point3Parameter>(par);
                parRow = std::make_shared<Point3ParameterRow>(*ptp);

                paramInserted = true;
            } break;
            default: break;
            }
            if (paramInserted) {
                parRow->addRowToGridLayout(this, row);
                parRow->connectValueChanged(this, SLOT(updateDependencies()));
                mRows.push_back(parRow);
                ++row;
            }
        }
    }
    updateDependencies();
}

ParameterVector ParametersGridLayout::parameters() const
{
    ParameterVector pars = mParameters;
    for (const std::shared_ptr<ParameterRow>& row : mRows) {
        auto par = row->parameterFromWidget();
        pars.get(par->name())->setValue(*par);
    }
    return pars;
}

void ParametersGridLayout::setHelpVisible(bool b)
{
    for (const std::shared_ptr<ParameterRow>& row : mRows)
        row->setHelpVisible(b);
}

void ParametersGridLayout::updateDependencies()
{
    ParameterVector currentParams = parameters();
    for (const std::shared_ptr<ParameterRow>& row : mRows) {
        auto p = row->parameterFromWidget();
        if (p->hasDependency()) {
            const auto& dep = p->dependency();
            auto depParam = currentParams.get(dep.parameterName);
            bool satisfied = false;
            if (depParam) {
                int val = -1;
                if (depParam->type() == ParameterType::BOOL) val = depParam->boolValue() ? 1 : 0;
                else if (depParam->type() == ParameterType::ENUM) val = depParam->uintValue();
                
                for (int expected : dep.expectedValues) {
                    if (val == expected) {
                        satisfied = true;
                        break;
                    }
                }
            }
            row->setVisible(satisfied);
        }
    }
}

void ParametersGridLayout::clear()
{
    clear(this);
    mParameters.clear();
    mRows.clear();
}

// https://stackoverflow.com/a/7077340/5851101
void ParametersGridLayout::clear(QLayout* layout)
{
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget())
            widget->deleteLater();
        if (QLayout* childLayout = item->layout())
            clear(childLayout);
        delete item;
    }
}

} // namespace mop
