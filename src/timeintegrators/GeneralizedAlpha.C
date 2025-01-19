//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "GeneralizedAlpha.h"
#include "NonlinearSystem.h"
#include "FEProblemBase.h"

registerMooseObject("nsch_mooseApp", GeneralizedAlpha);

InputParameters
GeneralizedAlpha::validParams()
{
  InputParameters params = TimeIntegrator::validParams();
  params.addClassDescription(
      "Computes the first time derivative of variable using Generalized-Alpha method.");
  params.addParam<Real>("rho_infty", 0.5, "rho_infty value");
  params.addParam<int>("inactive_tsteps",
                       0,
                       "The time derivatives will set to be zero for this number of time steps.");
  return params;
}

GeneralizedAlpha::GeneralizedAlpha(const InputParameters & parameters)
  : TimeIntegrator(parameters),
    _u_alpha_f(_sys.currentSolution()),
    _u_old(_sys.solutionOld()),
    _rho_infty(getParam<Real>("rho_infty")),
    _time_new(_fe_problem.time()),
    _time_old(_fe_problem.timeOld()),
    _inactive_tsteps(getParam<int>("inactive_tsteps"))
{
  _fe_problem.setUDotOldRequested(true);

  _alpha_m = 0.5 * (3.0 - _rho_infty) / (1.0 + _rho_infty);
  _alpha_f = 1.0 / (1.0 + _rho_infty);
  _gamma = 0.5 + _alpha_m - _alpha_f;

  if (_rho_infty < 0.0 || _rho_infty > 1.0)
    mooseError("GeneralizedAlpha: rho_infty must be in [0, 1].");
}

GeneralizedAlpha::~GeneralizedAlpha() {}

void GeneralizedAlpha::preSolve()
{
  // std::cout << "Content of u_old: ";
  // for (unsigned int i = 0; i < _u_old.size(); ++i)
  // {
  //   std::cout << _u_old(i) << " ";
  // }
  // std::cout << std::endl;

  // Set timestamp to alpha stage time
  _fe_problem.time() = _time_old + _alpha_f * _dt;

}

void
GeneralizedAlpha::computeTimeDerivatives()
{
  if (!_sys.solutionUDot())
    mooseError("GeneralizedAlpha: Time derivative of solution (`u_dot`) is not stored. Please set "
               "uDotRequested() to true in FEProblemBase befor requesting `u_dot`.");

  if (!_sys.solutionUDotOld())
    mooseError("GeneralizedAlpha: Old time derivative of solution (`u_dot_old`) is not stored. Please "
               "set uDotOldRequested() to true in FEProblemBase befor requesting `u_dot_old`.");

  NumericVector<Number> & u_dot_alpha_m = *_sys.solutionUDot();
  NumericVector<Number> & u_dot_old = *_sys.solutionUDotOld();

  if (_fe_problem.timeStep() <= _inactive_tsteps)
  {
    u_dot_alpha_m.zero();
  }
  else
  {
    u_dot_alpha_m = *_u_alpha_f;
    computeTimeDerivativeHelper(u_dot_alpha_m, _u_old, u_dot_old);
  }

  // make sure _u_dot is in good state
  u_dot_alpha_m.close();

  // used for Jacobian calculations
  computeDuDotDu();
}

void
GeneralizedAlpha::computeADTimeDerivatives(ADReal & ad_u_dot_alpha_m,
                                      const dof_id_type & dof,
                                      ADReal & /*ad_u_dotdot*/) const
{
  const auto & u_old = _u_old(dof);
  const auto & u_dot_old = (*_sys.solutionUDotOld())(dof);

  computeTimeDerivativeHelper(ad_u_dot_alpha_m, u_old, u_dot_old);
}

void
GeneralizedAlpha::postResidual(NumericVector<Number> & residual)
{
  residual += *_Re_time;
  residual += *_Re_non_time;
  residual.close();
}

Real
GeneralizedAlpha::duDotDuCoeff() const
{
  return _alpha_m / (_alpha_f * _gamma);
}

void GeneralizedAlpha::postSolve()
{
  // After solving the system, using alpha stage vector to compute n+1 stage solution

  NumericVector<Number> & u_new = _sys.solution();
  u_new = *_u_alpha_f;
  u_new.scale(1.0/_alpha_f);
  MathUtils::addScaled((1.0 - 1.0 / _alpha_f), _u_old, u_new);
  u_new.close();

  NumericVector<Number> & u_dot_new = *_sys.solutionUDot();
  NumericVector<Number> & u_dot_old = *_sys.solutionUDotOld();
  u_dot_new.scale(1.0/_alpha_m);
  MathUtils::addScaled((1.0 - 1.0 / _alpha_m), u_dot_old, u_dot_new);
  u_dot_new.close();

  // Restore actual timestamp
  _fe_problem.time() = _time_new;

}