//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "nsch_mooseTestApp.h"
#include "nsch_mooseApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
nsch_mooseTestApp::validParams()
{
  InputParameters params = nsch_mooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

nsch_mooseTestApp::nsch_mooseTestApp(InputParameters parameters) : MooseApp(parameters)
{
  nsch_mooseTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

nsch_mooseTestApp::~nsch_mooseTestApp() {}

void
nsch_mooseTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  nsch_mooseApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"nsch_mooseTestApp"});
    Registry::registerActionsTo(af, {"nsch_mooseTestApp"});
  }
}

void
nsch_mooseTestApp::registerApps()
{
  registerApp(nsch_mooseApp);
  registerApp(nsch_mooseTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
nsch_mooseTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  nsch_mooseTestApp::registerAll(f, af, s);
}
extern "C" void
nsch_mooseTestApp__registerApps()
{
  nsch_mooseTestApp::registerApps();
}
