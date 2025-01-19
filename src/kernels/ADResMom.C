#include "ADResMom.h"

registerMooseObject("nsch_mooseApp", ADResMom);

InputParameters ADResMom::validParams()
{
    InputParameters params = ADVectorKernel::validParams();
    params.addClassDescription("This class compute the residual for momentum equation");
    return params;
}

ADResMom::ADResMom(const InputParameters & parameters)
    : ADVectorKernel(parameters),
    _Res_Mom_Value(getADMaterialProperty<RealVectorValue>("Res_Mom_Value")),
    _Res_Mom_Grad(getADMaterialProperty<RealTensorValue>("Res_Mom_Grad"))
{
}

ADReal ADResMom::computeQpResidual()
{
    return _Res_Mom_Value[_qp] * _test[_i][_qp] + _Res_Mom_Grad[_qp].contract(_grad_test[_i][_qp]);
}
