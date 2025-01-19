#pragma once

#include "Material.h"
#include "Assembly.h"
#include "MooseMesh.h"

#define SQ(x) ((x) * (x))

/**
 * This class compute material object for NSCH
 */

class ADMaterial_NSCH : public Material
{
public:
    static InputParameters validParams();

    ADMaterial_NSCH(const InputParameters & parameters);

protected:

    virtual void computeQpProperties();

    // Coupling Variables
    const ADVariableValue & _phi;
    const ADVariableValue & _phi_t;
    const ADVariableGradient & _grad_phi;
    const ADVariableSecond & _hess_phi;

    const ADVariableValue & _mu;
    const ADVariableGradient & _grad_mu;

    const ADVariableValue & _p;
    const ADVariableGradient & _grad_p;

    const ADVectorVariableValue & _u;
    const ADVectorVariableValue & _u_t;
    const ADVectorVariableGradient & _grad_u;

    // AD Material property
    ADMaterialProperty<Real> & _rho;
    ADMaterialProperty<Real> & _rRe;

    // AD Residual Property
    ADMaterialProperty<Real> & _Res_Mass_Value;
    ADMaterialProperty<RealVectorValue> & _Res_Mass_Grad;

    ADMaterialProperty<RealVectorValue> & _Res_Mom_Value;
    ADMaterialProperty<RealTensorValue> & _Res_Mom_Grad;

    ADMaterialProperty<Real> & _Res_Phi_Value;
    ADMaterialProperty<RealVectorValue> & _Res_Phi_Grad;

    ADMaterialProperty<Real> & _Res_Mu_Value;
    ADMaterialProperty<RealVectorValue> & _Res_Mu_Grad;

    ADMaterialProperty<RealVectorValue> & _ResMomStrong;

    // Constant Input Material
    Real _L0;
    Real _Ca;
    Real _Mobility;
    Real _rho1;
    Real _rho2;
    Real _viscosity1;
    Real _viscosity2;
    Real _gravity;
    Real _sigma;
    bool _Inviscid;
    bool _MonotonicPressure;
    bool _Stabilization; 
    Real _C_I;

private: 
    RealTensorValue computeGeometricTensor(unsigned int qp);
    
};
