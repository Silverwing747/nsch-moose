#pragma once

#include "ADKernel.h"

class ADResMom : public ADVectorKernel
{
public:
    ADResMom(const InputParameters & parameters);

    static InputParameters validParams();

protected: 
    virtual ADReal computeQpResidual() override;

    const ADMaterialProperty<RealVectorValue> & _Res_Mom_Value;
    const ADMaterialProperty<RealTensorValue> & _Res_Mom_Grad;
};