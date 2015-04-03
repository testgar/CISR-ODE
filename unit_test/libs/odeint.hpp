/*
 [auto_generated]
 boost/numeric/odeint.hpp

 [begin_description]
 Forward include for odeint. Includes nearly everything.
 [end_description]

 Copyright 2009-2013 Karsten Ahnert
 Copyright 2010-2013 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef BOOST_NUMERIC_ODEINT_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_HPP_INCLUDED

#include "odeint/version.hpp"
#include "odeint/config.hpp"

// start with ublas wrapper because we need its specializations before including state_wrapper.hpp
#include "odeint/util/ublas_wrapper.hpp"

// #include "odeint/stepper/euler.hpp"
// #include "odeint/stepper/runge_kutta4_classic.hpp"
// #include "odeint/stepper/runge_kutta4.hpp"
// #include "odeint/stepper/runge_kutta_cash_karp54.hpp"
// #include "odeint/stepper/runge_kutta_cash_karp54_classic.hpp"
#include "odeint/stepper/runge_kutta_dopri5.hpp"
// #include "odeint/stepper/runge_kutta_fehlberg78.hpp"

#include "odeint/stepper/controlled_runge_kutta.hpp"

#include "odeint/stepper/dense_output_runge_kutta.hpp"

#include "odeint/stepper/bulirsch_stoer.hpp"

#ifndef __CUDACC__
/* Bulirsch Stoer with Dense Output does not compile with nvcc
 * because of the binomial library used there which relies on unsupported SSE functions
 */
#include "odeint/stepper/bulirsch_stoer_dense_out.hpp"
#endif

#include "odeint/stepper/symplectic_euler.hpp"
#include "odeint/stepper/symplectic_rkn_sb3a_mclachlan.hpp"
#include "odeint/stepper/velocity_verlet.hpp"

#include "odeint/stepper/adams_bashforth_moulton.hpp"

#include "odeint/stepper/implicit_euler.hpp"
#include "odeint/stepper/rosenbrock4.hpp"
#include "odeint/stepper/rosenbrock4_controller.hpp"
#include "odeint/stepper/rosenbrock4_dense_output.hpp"

#include "odeint/algebra/algebra_dispatcher.hpp"
#include "odeint/algebra/multi_array_algebra.hpp"
#include "odeint/util/multi_array_adaption.hpp"
/*
 * Including this algebra slows down the compilation time
 */
// #include "odeint/algebra/fusion_algebra.hpp"

#include "odeint/integrate/integrate.hpp"
#include "odeint/integrate/integrate_adaptive.hpp"
#include "odeint/integrate/integrate_const.hpp"
#include "odeint/integrate/integrate_n_steps.hpp"
#include "odeint/integrate/integrate_times.hpp"

#include "odeint/integrate/observer_collection.hpp"

#include "odeint/stepper/generation.hpp"

#include "odeint/iterator/adaptive_iterator.hpp"
#include "odeint/iterator/adaptive_time_iterator.hpp"
#include "odeint/iterator/const_step_iterator.hpp"
#include "odeint/iterator/const_step_time_iterator.hpp"


#endif // BOOST_NUMERIC_ODEINT_HPP_INCLUDED
