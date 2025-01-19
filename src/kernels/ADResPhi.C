#include "ADResPhi.h"

registerMooseObject("nsch_mooseApp", ADResPhi);

InputParameters ADResPhi::validParams()
{
    InputParameters params = ADKernel::validParams();
    params.addClassDescription("This class compute the residual for phase field equation");
    return params;
}

ADResPhi::ADResPhi(const InputParameters & parameters)
    : ADKernel(parameters),
    _Res_Phi_Value(getADMaterialProperty<Real>("Res_Phi_Value")),
    _Res_Phi_Grad(getADMaterialProperty<RealVectorValue>("Res_Phi_Grad"))
{
}

ADReal ADResPhi::computeQpResidual()
{
    return _Res_Phi_Value[_qp] * _test[_i][_qp] + _Res_Phi_Grad[_qp] * _grad_test[_i][_qp];
}
