#pragma once
#include <armadillo>
// #include "../../libs/types.hpp"
#include "autogenerated/model_outputs.hpp"
#include "autogenerated/model_states.hpp"
#include "autogenerated/model_inputs.hpp"
#include "autogenerated/model_intermediates.hpp"
#include "../../application/config.hpp"

typedef arma::vec::fixed<states::state_size> state_type;
typedef state_type deriv_type;

typedef arma::vec::fixed<outputs::output_size> observer_type;
typedef arma::vec::fixed<inputs::input_size> input_type;
typedef arma::vec::fixed<mids::intermediate_size> intermediate_type;
typedef arma::mat::fixed<config::buffer_headers,config::buffer_size> buffer_type;

typedef double value_type;
typedef double time_type;