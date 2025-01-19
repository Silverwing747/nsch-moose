#pragma once

#include "ADKernel.h"

class ADResPhi : public ADKernel
{
public:
    ADResPhi(const InputParameters & parameters);

    static InputParameters validParams();

protected: 
    virtual ADReal computeQpResidual() override;

    const ADMaterialProperty<Real> & _Res_Phi_Value;
    const ADMaterialProperty<RealVectorValue> & _Res_Phi_Grad;
};