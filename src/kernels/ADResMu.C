#include "ADResMu.h"

registerMooseObject("nsch_mooseApp", ADResMu);

InputParameters ADResMu::validParams()
{
    InputParameters params = ADKernel::validParams();
    params.addClassDescription("This class compute the residual for mass conservation");
    return params;
}

ADResMu::ADResMu(const InputParameters & parameters)
    : ADKernel(parameters),
    _Res_Mu_Value(getADMaterialProperty<Real>("Res_Mu_Value")),
    _Res_Mu_Grad(getADMaterialProperty<RealVectorValue>("Res_Mu_Grad"))
{
}

ADReal ADResMu::computeQpResidual()
{
    return _Res_Mu_Value[_qp] * _test[_i][_qp] + _Res_Mu_Grad[_qp] * _grad_test[_i][_qp];
}
