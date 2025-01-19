#pragma once

#include "ADKernel.h"

class ADResMu : public ADKernel
{
public:
    ADResMu(const InputParameters & parameters);

    static InputParameters validParams();

protected: 
    virtual ADReal computeQpResidual() override;

    const ADMaterialProperty<Real> & _Res_Mu_Value;
    const ADMaterialProperty<RealVectorValue> & _Res_Mu_Grad;
};