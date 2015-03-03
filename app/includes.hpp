// // added by me:
// #include "../simple/explicit_error_stepper_fsal_base.hpp"
// #include "../simple/controlled_runge_kutta.hpp"
// #include "../simple/runge_kutta_dopri5.hpp"
// // #include "../simple/runge_kutta_cash_karp54.hpp"
// #include "../simple/runge_kutta_fehlberg78.hpp"

// #include <boost/numeric/odeint.hpp>

// // original
// #include <stdexcept>

// #include <boost/throw_exception.hpp>

// #include <boost/numeric/odeint/stepper/stepper_categories.hpp>
// #include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
// #include <boost/numeric/odeint/integrate/detail/integrate_const.hpp>
// #include <boost/numeric/odeint/util/copy.hpp>

// #include <boost/numeric/odeint/util/detail/less_with_sign.hpp>

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <ctime>
#include <string>
#include <vector>
#include <armadillo>
#include <boost/algorithm/string.hpp> // for using boost::split to explode a string
