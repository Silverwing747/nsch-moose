//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "TimeIntegrator.h"
#include "MathUtils.h"

/**
 * Generalized-Alpha time integration method
 */
class GeneralizedAlpha : public TimeIntegrator
{
public:
  static InputParameters validParams();

  GeneralizedAlpha(const InputParameters & parameters);
  virtual ~GeneralizedAlpha();

  virtual int order() override { return 1; }
  virtual void computeTimeDerivatives() override;
  virtual void computeADTimeDerivatives(ADReal & ad_u_dot,
                                        const dof_id_type & dof,
                                        ADReal & ad_u_dotdot) const override;
  virtual void preSolve() override;
  virtual void postResidual(NumericVector<Number> & residual) override;
  virtual void postSolve() override;
  virtual bool overridesSolve() const override { return false; }

protected:
  /**
   * Helper function that actually does the math for computing the time derivative
   */
  template <typename T, typename T2, typename T3>
  void computeTimeDerivativeHelper(T & u_dot,
                                   const T2 & u_old,
                                   const T3 & u_dot_old) const;

  virtual Real duDotDuCoeff() const override;

  /// Solution vectors for different states and variable restrictions
  const NumericVector<Number> * const & _u_alpha_f;
  NumericVector<Number> & _u_old; 

  /// Generalized-alpha time integration parameter
  Real _rho_infty, _alpha_m, _alpha_f, _gamma;

  /// Time stamp
  Real _time_new, _time_old;

  /// Inactive time steps
  int _inactive_tsteps;
};

template <typename T, typename T2, typename T3>
void
GeneralizedAlpha::computeTimeDerivativeHelper(
    T & u_dot_alpha_m, const T2 & u_old, const T3 & u_dot_old) const
{

  // Compute the time derivative for the alpha stage according to generalized-alpha method

  u_dot_alpha_m -= u_old;
  u_dot_alpha_m *= _alpha_m / (_alpha_f * _gamma * _dt);
  MathUtils::addScaled((1.0 - _alpha_m / _gamma), u_dot_old, u_dot_alpha_m);
}
