#pragma once

#include "ADKernel.h"

class ADResMass : public ADKernel
{
public:
    ADResMass(const InputParameters & parameters);

    static InputParameters validParams();

protected: 
    virtual ADReal computeQpResidual() override;

    const ADMaterialProperty<Real> & _Res_Mass_Value;
    const ADMaterialProperty<RealVectorValue> & _Res_Mass_Grad;
};