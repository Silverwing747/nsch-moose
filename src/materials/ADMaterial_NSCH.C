#include "ADMaterial_NSCH.h"

registerMooseObject("nsch_mooseApp", ADMaterial_NSCH);

InputParameters ADMaterial_NSCH::validParams()
{
    InputParameters params = Material::validParams();
    params.addClassDescription("This class compute material object for NSCH");
    params.addCoupledVar("phase_field", "Coupling phase field");
    params.addCoupledVar("chemical_potential", "Coupling chemical potential");
    params.addCoupledVar("pressure", "Coupling pressure");
    params.addCoupledVar("velocity", "Coupling velocity");
    params.addRequiredParam<Real>("L0", "Scaling length dimension");
    params.addParam<Real>("Ca", 0.01, "Relative interface thickness w.r.t. to scaling length");
    params.addParam<Real>("Mobility", 1e-7, "Mobility in m^3.s/kg");
    params.addParam<Real>("rho1", 1.204, "Fluid 1 density in kg/m^3 (default value is air density)");
    params.addParam<Real>("rho2", 998.207, "Fluid 2 density in kg/m^3 (default value is water density)");
    params.addParam<Real>("viscosity1", 0.0000178, "Fluid 1 viscosity in Pa.s (default value is air viscosity)");
    params.addParam<Real>("viscosity2", 0.001002, "Fluid 2 viscosity in Pa.s (default value is water viscosity)");
    params.addParam<Real>("gravity", -9.81, "Gravitational force in m/s^2 (default earth gravity)");
    params.addParam<Real>("sigma", 0.0728, "Surface tension in N/m (default value is water-air surface tension)");
    params.addParam<bool>("Inviscid", false, "Viscous or inviscid flow");
    params.addParam<bool>("MonotonicPressure", false, "Use Monotonic Pressure or not");
    params.addParam<bool>("Stabilization", true, "Use stabilization or not");
    params.addParam<Real>("C_I", 0.1, "Coefficient in tau_SUPG stabilization");
    return params;
}

ADMaterial_NSCH::ADMaterial_NSCH(const InputParameters & parameters)
    : Material(parameters),

    // Phase field coupling
    _phi(isCoupled("phase_field") ? adCoupledValue("phase_field") : _ad_zero),
    _phi_t(isCoupled("phase_field") ? adCoupledDot("phase_field") : _ad_zero),
    _grad_phi(isCoupled("phase_field") ? adCoupledGradient("phase_field") : _ad_grad_zero),
    _hess_phi(isCoupled("phase_field") ? adCoupledSecond("phase_field") : _ad_second_zero),

    // Chemical potential coupling
    _mu(isCoupled("chemical_potential") ? adCoupledValue("chemical_potential") : _ad_zero),
    _grad_mu(isCoupled("chemical_potential") ? adCoupledGradient("chemical_potential") : _ad_grad_zero),

    // Pressure coupling
    _p(isCoupled("pressure") ? adCoupledValue("pressure") : _ad_zero),
    _grad_p(isCoupled("pressure") ? adCoupledGradient("pressure") : _ad_grad_zero),

    // Velocity coupling
    _u(isCoupled("velocity") ? adCoupledVectorValue("velocity") : _ad_grad_zero),
    _u_t(isCoupled("velocity") ? adCoupledVectorDot("velocity") : _ad_grad_zero),
    _grad_u(isCoupled("velocity") ? adCoupledVectorGradient("velocity") : _ad_second_zero),

    // Declare ADMaterial properties
    _rho(declareADProperty<Real>("rho")),
    _rRe(declareADProperty<Real>("rRe")),

    // Declare AD Residual properties
    _Res_Mass_Value(declareADProperty<Real>("Res_Mass_Value")),
    _Res_Mass_Grad(declareADProperty<RealVectorValue>("Res_Mass_Grad")),

    _Res_Mom_Value(declareADProperty<RealVectorValue>("Res_Mom_Value")),
    _Res_Mom_Grad(declareADProperty<RealTensorValue>("Res_Mom_Grad")),

    _Res_Phi_Value(declareADProperty<Real>("Res_Phi_Value")),
    _Res_Phi_Grad(declareADProperty<RealVectorValue>("Res_Phi_Grad")),

    _Res_Mu_Value(declareADProperty<Real>("Res_Mu_Value")),
    _Res_Mu_Grad(declareADProperty<RealVectorValue>("Res_Mu_Grad")),

    _ResMomStrong(declareADProperty<RealVectorValue>("ResMomStrong")),

    // Constant Input Material
    _L0(getParam<Real>("L0")),
    _Ca(getParam<Real>("Ca")),
    _Mobility(getParam<Real>("Mobility")),
    _rho1(getParam<Real>("rho1")),
    _rho2(getParam<Real>("rho2")),
    _viscosity1(getParam<Real>("viscosity1")),
    _viscosity2(getParam<Real>("viscosity2")),
    _gravity(getParam<Real>("gravity")),
    _sigma(getParam<Real>("sigma")),
    _Inviscid(getParam<bool>("Inviscid")),
    _MonotonicPressure(getParam<bool>("MonotonicPressure")),
    _Stabilization(getParam<bool>("Stabilization")),
    _C_I(getParam<Real>("C_I"))
{
}

RealTensorValue ADMaterial_NSCH::computeGeometricTensor(unsigned int qp)
{
    const int DIM = _current_elem->dim();
    const FEBase * fe = _assembly.getFE(FEType(), DIM);

    // Obtain the inverse of the Jacobian matrix
    const std::vector<Real> & dxidx(fe->get_dxidx());
    const std::vector<Real> & dxidy(fe->get_dxidy()); 
    const std::vector<Real> & dxidz(fe->get_dxidz());
    const std::vector<Real> & detadx(fe->get_detadx()); // Empty in 1D
    const std::vector<Real> & detady(fe->get_detady()); // Empty in 1D
    const std::vector<Real> & detadz(fe->get_detadz()); // Empty in 1D
    const std::vector<Real> & dzetadx(fe->get_dzetadx()); // Empty in 2D
    const std::vector<Real> & dzetady(fe->get_dzetady()); // Empty in 2D
    const std::vector<Real> & dzetadz(fe->get_dzetadz()); // Empty in 2D

    Real invJ[3][3];
    if (DIM == 1){
        invJ[0][0] = dxidx[qp];
    } else if (DIM == 2){
        invJ[0][0] = dxidx[qp];  invJ[0][1] = dxidy[qp];
        invJ[1][0] = detadx[qp]; invJ[1][1] = detady[qp];
    } else if (DIM == 3){
        invJ[0][0] = dxidx[qp];   invJ[0][1] = dxidy[qp];   invJ[0][2] = dxidz[qp];
        invJ[1][0] = detadx[qp];  invJ[1][1] = detady[qp];  invJ[1][2] = detadz[qp];
        invJ[2][0] = dzetadx[qp]; invJ[2][1] = dzetady[qp]; invJ[2][2] = dzetadz[qp];
    }

    // Compute geometric element tensor
    RealTensorValue G;
    for (int i = 0; i < DIM; ++i){
        for (int j = 0; j < DIM; ++j){
            G(i, j) = 0.0;
            for (int k = 0; k < DIM; ++k){
                G(i, j) += invJ[k][i] * invJ[k][j];
            }
        }
    }

    return G;
}

void ADMaterial_NSCH::computeQpProperties()
{
    // Compute Scaling Parameters
    Real rho_scale = 0.5 * (_rho1 + _rho2);
    Real u_scale = std::sqrt(_sigma / rho_scale / _L0);
    Real p_scale = _sigma / _L0;

    // Compute Phyiscal parameters
    Real rho1_nondim = _rho1 / rho_scale;
    Real rho2_nondim = _rho2 / rho_scale;
    auto viscosity = (_viscosity1 + _viscosity2) / 2.0 + (_viscosity1 - _viscosity2) / 2.0 * _phi[_qp];
    _rho[_qp] = (rho1_nondim + rho2_nondim) / 2.0 + (rho1_nondim - rho2_nondim) / 2.0 * _phi[_qp];

    // Compute Non-dimensional parameters
    Real Ca_f = 3.0 / (2.0 * std::sqrt(2.0)) * _Ca;
    Real Bo = rho_scale * _gravity * _L0 * _L0 / _sigma;
    auto Pe = rho_scale * u_scale * _Mobility / _L0;// * (1.0 - _phi[_qp] * _phi[_qp]) / 4.0;;
    _rRe[_qp] = _Inviscid ? 0.0 : viscosity / (rho_scale * u_scale * _L0);

    // Compute Mass Residual
    _Res_Mass_Value[_qp] = _grad_u[_qp].tr();
    _Res_Mass_Grad[_qp] = 0;

    // Compute Momentum Residual
    _Res_Mom_Value[_qp] = _rho[_qp] * _u_t[_qp]; // Time derivative term
    _Res_Mom_Grad[_qp] = - _p[_qp] * RankTwoTensor::Identity(); // Pressure term
    _Res_Mom_Value[_qp] += _rho[_qp] * _grad_u[_qp] * _u[_qp]; // Velocity convective term
    //_Res_Mom_Value[_qp] -= (rho1_nondim - rho2_nondim) / 2.0 * Pe * _grad_u[_qp] * _grad_mu[_qp]; // Phase-field advection term
    if (_current_elem->dim() >= 2){
        _Res_Mom_Value[_qp](1) -= Bo * _rho[_qp]; // Gravity term
    }

    _Res_Mom_Grad[_qp] += _rRe[_qp] * (_grad_u[_qp] + _grad_u[_qp].transpose()); // Viscous Term
    _Res_Mom_Grad[_qp] -= Ca_f * outer_product(_grad_phi[_qp],_grad_phi[_qp]); // Phase-field diffusion term

    if (_MonotonicPressure){
        _Res_Mom_Grad[_qp] += Ca_f * _grad_phi[_qp].norm_sq() * RankTwoTensor::Identity(); // Extra stress term for monotonic pressure
    }

    // Compute Phase-field Residual
    _Res_Phi_Value[_qp] = _phi_t[_qp] + _u[_qp] * _grad_phi[_qp];
    _Res_Phi_Grad[_qp] = - Pe * Ca_f * _grad_mu[_qp];

    // Compute Chemical Potential Residual
    _Res_Mu_Value[_qp] = _mu[_qp] + 1.0 / SQ(_Ca) * _phi[_qp] * (_phi[_qp] * _phi[_qp] - 1.0);
    _Res_Mu_Grad[_qp] = _grad_phi[_qp];

    // Stabilization Part
    if (_Stabilization){

        // Compute Tau SUPG
        RealTensorValue G = computeGeometricTensor(_qp);

        ADReal TimeContribution = 4.0 / (_dt * _dt);
        ADReal AdvContribution = _u[_qp] * (G * _u[_qp]);
        ADReal DiffContribution_Vis = _C_I * G.contract(G) * SQ(_rRe[_qp] / _rho[_qp]);
        ADReal DiffContribution_Phi = _C_I * G.contract(G) * SQ(Pe * Ca_f / _Ca / _Ca);

        ADReal tau_SUPS = 1.0 / sqrt(TimeContribution + AdvContribution + DiffContribution_Vis);
        ADReal vu_LSIC = 1.0 / (G.tr() * tau_SUPS);
        ADReal tau_phi = 1.0 / sqrt(TimeContribution + AdvContribution + DiffContribution_Phi);

        // Compute Strong Residual
        auto ResStrong_Mass = _Res_Mass_Value[_qp];
        auto ResStrong_Mom = _Res_Mom_Value[_qp];
        auto ResStrong_Phi = _Res_Phi_Value[_qp];

        ResStrong_Mom += _grad_p[_qp];
        ResStrong_Mom += Ca_f * (_hess_phi[_qp] * _grad_phi[_qp] + _hess_phi[_qp].tr() * _grad_phi[_qp]);
        if (_MonotonicPressure){
            ResStrong_Mom -= Ca_f * 2.0 * _hess_phi[_qp] * _grad_phi[_qp];
        }

        _ResMomStrong[_qp] = ResStrong_Mom;

        // Compute Fine Scale Component
        auto p_s = - _rho[_qp] * vu_LSIC * ResStrong_Mass;
        auto u_s = - tau_SUPS / _rho[_qp] * ResStrong_Mom;
        auto phi_s = - tau_phi * ResStrong_Phi;

        // Add Stabilization to residual
        _Res_Mass_Grad[_qp] += - u_s;
        _Res_Mom_Grad[_qp] += - p_s * RankTwoTensor::Identity();
        _Res_Mom_Value[_qp] += _rho[_qp] * _grad_u[_qp] * u_s;
        _Res_Phi_Grad[_qp] += - _u[_qp] * phi_s;
        //_Res_Phi_Grad[_qp] += - _phi[_qp] * u_s;
        _Res_Phi_Value[_qp] += _grad_phi[_qp] * u_s;

    }
}