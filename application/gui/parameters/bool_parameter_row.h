/*****************************************************************************
 * HLMP                                                                      *
 * HighLevelMeshProcessing                                                   *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef MOP_APPLICATION_GUI_PARAMETERS_BOOL_PARAMETER_ROW_H
#define MOP_APPLICATION_GUI_PARAMETERS_BOOL_PARAMETER_ROW_H

#include <QCheckBox>

#include "parameter_row.h"

namespace mop {

class BoolParameterRow : public ParameterRow
{
    BoolParameter mParam;

    QCheckBox* mCheckBox = nullptr;

public:
    BoolParameterRow(const BoolParameter& param);

    // ParameterRow interface
    QWidget* parameterWidget() override;

    std::shared_ptr<Parameter> parameterFromWidget() const override;
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_PARAMETERS_BOOL_PARAMETER_ROW_H
