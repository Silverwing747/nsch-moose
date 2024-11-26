#include "nsch_mooseApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
nsch_mooseApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

nsch_mooseApp::nsch_mooseApp(InputParameters parameters) : MooseApp(parameters)
{
  nsch_mooseApp::registerAll(_factory, _action_factory, _syntax);
}

nsch_mooseApp::~nsch_mooseApp() {}

void
nsch_mooseApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<nsch_mooseApp>(f, af, s);
  Registry::registerObjectsTo(f, {"nsch_mooseApp"});
  Registry::registerActionsTo(af, {"nsch_mooseApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
nsch_mooseApp::registerApps()
{
  registerApp(nsch_mooseApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
nsch_mooseApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  nsch_mooseApp::registerAll(f, af, s);
}
extern "C" void
nsch_mooseApp__registerApps()
{
  nsch_mooseApp::registerApps();
}
