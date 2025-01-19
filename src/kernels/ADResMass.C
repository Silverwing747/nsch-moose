#include "ADResMass.h"

registerMooseObject("nsch_mooseApp", ADResMass);

InputParameters ADResMass::validParams()
{
    InputParameters params = ADKernel::validParams();
    params.addClassDescription("This class compute the residual for mass conservation");
    return params;
}

ADResMass::ADResMass(const InputParameters & parameters)
    : ADKernel(parameters),
    _Res_Mass_Value(getADMaterialProperty<Real>("Res_Mass_Value")),
    _Res_Mass_Grad(getADMaterialProperty<RealVectorValue>("Res_Mass_Grad"))
{
}

ADReal ADResMass::computeQpResidual()
{
    // div u - Test function auto applied
    return _Res_Mass_Value[_qp] * _test[_i][_qp] + _Res_Mass_Grad[_qp] * _grad_test[_i][_qp];
}
