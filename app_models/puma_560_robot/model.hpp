#pragma once

#include "types.hpp"
#include "constants.hpp"
#include <boost/math/special_functions/fpclassify.hpp>

using namespace arma;

class Model
{
public:
	typedef arma::vec::fixed<states::state_size> state_type;
	typedef state_type deriv_type;

	typedef arma::vec::fixed<outputs::output_size> observer_type;
	typedef arma::vec::fixed<inputs::input_size> input_type;
	typedef arma::vec::fixed<mids::intermediate_size> intermediate_type;
	typedef arma::mat::fixed<config::buffer_headers,config::buffer_size> buffer_type;

	typedef double value_type;
	typedef double time_type;

public:

	static void rhs(
		const state_type 			&x,
		state_type 					&x_dot,
		const time_type				&t,
		const intermediate_type 	&last_observed_mids,
		const time_type 			&last_observed_t,
		input_type					&u
		);

	static void observer(
		const state_type 			&x ,
		const double 				&t,
		observer_type 				&ymat,
		const intermediate_type 	&last_observed_mids,
		const time_type 			&last_observed_t,
		input_type					&u
		);

	static void intermediates(
		const input_type			&u,
		const state_type			&x,
		intermediate_type			&mid,
		const time_type				&t,
		const intermediate_type		&last_observed_mids,
		const time_type				&last_observed_t
		);

	static double input(const time_type t,input_type &u)
	{
		// _unused(t);
		// _unused(u);

		u(inputs::theta_acc_ref_0)=0;
		u(inputs::theta_acc_ref_1)=+std::min(20.0/3.0,2*t);
		u(inputs::theta_acc_ref_2)=-std::min(20.0/3.0,2*t);

// atan(1000*x)*1/pi+0.5

		double next_sudden_change_time=-1;
		return next_sudden_change_time;
	}


	static const double I1,I2,I3,I4,I5,I6,I7,I8,I9,I10,I11,I12,I13,I14,I15,I16,I17,I18,I19,I20,I21,I22,I23;
	static const double Im1,Im2,Im3,Im4,Im5,Im6;
	static const double g1,g2,g3,g4,g5;

	static arma::mat kinetic_energy_mat(const double theta2,const double theta3)
	{
		const double s2=sin(theta2);
		const double c2=cos(theta2);
		const double s3=sin(theta3);
		const double c3=cos(theta3);
		const double s23=sin(theta2+theta3);
		const double c23=cos(theta2+theta3);

		const double m11=Im1+I1+I3*c2*c2+I7*s23+I10*s23*c23+I11*c2*s2+I21*s23*s23+2.0*(I5*c2*s23+I12*c23*c2+I15*s23*s23+I16*s23*c2+I22*s23*c23);
		const double m12=I4*s2+I9*c2+I8*c23+I13*s23-I18*c23;
		const double m13=I8*c23+I13*s23-I18*c23;
		const double m22=Im2+I2+I6+2.0*(I15+I12*c2+I15*s3+I16*s3);
		const double m23=I15*2.0+I12*c3+I6+I5*s3+I16*s3;
		const double m33=I15*2.0+I16+Im3;
		const double m35=I15+I17;
		const double m44=Im4+I14;
		const double m55=Im5+I17;
		const double m66=Im6+I23;

		const double m21=m12;
		const double m31=m13;
		const double m32=m23;

		arma::mat M={
			{ m11, m12, m13, 0.0, 0.0, 0.0 },
			{ m21, m22, m23, 0.0, 0.0, 0.0 },
			{ m31, m32, m33, 0.0, m35, 0.0 },
			{ 0.0, 0.0, 0.0, m44, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 0.0, m55, 0.0 },
			{ 0.0, 0.0, 0.0, 0.0, 0.0, m66 }
		};

		return M;
	}

	struct coriolis_type
	{
		double b112;
		double b113;
		double b115;
		double b123;
		double b214;
		double b223;
		double b225;
		double b235;
		double b314;
		double b412;
		double b413;
		double b415;
		double b514;
	};

	static coriolis_type coriolis_mat(const double theta2,const double theta3)
	{
		const double s2=sin(theta2);
		const double c2=cos(theta2);
		const double s3=sin(theta3);
		const double c3=cos(theta3);
		const double s23=sin(theta2+theta3);
		const double c23=cos(theta2+theta3);
		const double s223=sin(2*theta2+theta3);
		const double c223=cos(2*theta2+theta3);

		const double b112=2.0*(-I3*s2*c2+I5*c223+I7*s223*c223-I12*s223+2*I15*s23*c23+I16*c223+I21*s23*c23+I22*(2*s23*c23-1))+I10*(2*s23*c23-1)+I11*(2*s2*s2-1);
		const double b113=2.0*(I5*c2*c23+I7*s23*c23-I12*c2*s23+I15*2*s23*c23+I16*c2*c23+I21*s23*c23+I22*(2*s23*s23-1))+I10*(2*s23*s23-1);
		const double b115=2.0*((I15-1.0)*s23*c23+I16*c23*c2+I22*c23*c23);
		const double b123=2.0*(-I8*s23+I13*c23+I18*s23);
		const double b214=I14*s23+I19*s23+I20*s23;
		const double b223=2.0*(-I12*s3+I5*c3+I16*c3);
		const double b225=2.0*(I16*c3+I22);
		const double b235=2.0*(I16*c3+I22);
		const double b314=I14*s23+I19*s23+I20*s23;
		const double b412=-(I14*s23+I19*s23+I20*s23);
		const double b413=-(I14*s23+I19*s23+I20*s23);
		const double b415=-(I17*s23+I20*s23);
		const double b514=I17*s23+I20*s23;

		const coriolis_type B{b112,b113,b115,b123,b214,b223,b225,b235,b314,b412,b413,b415,b514};

		return B;
	}

	static arma::mat centrifugal_mat(const double theta2,const double theta3)
	{
		const double s2=sin(theta2);
		const double c2=cos(theta2);
		const double s3=sin(theta3);
		const double c3=cos(theta3);
		const double s23=sin(theta2+theta3);
		const double c23=cos(theta2+theta3);
		const double s223=sin(2*theta2+theta3);
		const double c223=cos(2*theta2+theta3);

		const double cx12=I4*c2-I8*s23-I9*s2+I13*s23+I18*s23;
		const double cx13=-I8*s23+I13*c23+I18*s23;
		const double cx21=-0.5*(2*(-I3*s2*c2+I5*c223-I7*c23*s23-I12*s223+2*I15*s23*c23+I16*c223+I21*s23*c23+I22*(2*s23*s23-1.0))+I10*(2*s23*s23-1.0)+I11*(2*s2*s2-1.0));
		const double cx23=-I12*s3+I5*c3+I16*c3;
		const double cx31=-0.5*(2.0*(I15*c2*c23+I17*s23*c23-I12*c2*s23+2*I15*s23*c23+I16*c23*c2+I21*s23*c23+I22*(1-2*s23*s23))+I10*(1-2*s23*s23));
		const double cx32=-(-I12*s3+I5*c3+I16*c3);
		const double cx51=-(-s23*c23+I15*s23*c23+I16*c23*c2+I22*c23*c23);
		const double cx52=-(I16*c3+I22);

		arma::mat CX={
			{ 0.0 , cx12, cx13, 0.0 , 0.0 , 0.0 },
			{ cx21, 0.0 , cx23, 0.0 , 0.0 , 0.0 },
			{ cx31, cx32, 0.0 , 0.0 , 0.0 , 0.0 },
			{ 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 },
			{ cx51, cx52, 0.0 , 0.0 , 0.0 , 0.0 },
			{ 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 }
		};

		return CX;
	}

	static arma::mat gravity_mat(const double theta2,const double theta3)
	{
		const double s2=sin(theta2);
		const double c2=cos(theta2);
		// const double s3=sin(theta3);
		// const double c3=cos(theta3);
		const double s23=sin(theta2+theta3);
		const double c23=cos(theta2+theta3);

		const double gx2=g1*c2+g3*s2+g2*s23+g4*c23+g5*s23;
		const double gx3=g2*s23+g4*c23+g5*s23;
		const double gx5=g5*s23;

		const arma::vec::fixed<6> GX={
			{0.0},
			{gx2},
			{gx3},
			{0.0},
			{gx5},
			{0.0}
		};
		return GX;
	}

	static arma::vec::fixed<6> to_T_vec(
		const arma::mat::fixed<6,6> 	Inertia_mat,
		const arma::vec::fixed<6> 		DD_theta_controller,
		const coriolis_type 			B,
		const arma::mat 				C,
		const arma::mat 				G,
		const arma::vec::fixed<3> 		theta_D
		)
	{

		const double B1=B.b112*theta_D(0)*theta_D(1)+B.b113*theta_D(0)*theta_D(2)+B.b123*theta_D(1)*theta_D(2);
		const double B2=B.b223*theta_D(1)*theta_D(2);
		const double B4=B.b412*theta_D(0)*theta_D(1)+B.b413*theta_D(0)*theta_D(2);

		const arma::vec::fixed<3> theta_D_P2=theta_D % theta_D;

		const double C1=theta_D_P2(1)*C(0,1)+theta_D_P2(2)*C(0,2);
		const double C2=theta_D_P2(0)*C(1,0)+theta_D_P2(2)*C(1,2);
		const double C3=theta_D_P2(0)*C(2,0)+theta_D_P2(1)*C(2,1);
		const double C5=theta_D_P2(0)*C(4,0)+theta_D_P2(1)*C(4,1);

		const arma::vec::fixed<6> BCG={
			{B1+C1+G(0)},
			{B2+C2+G(1)},
			{C3+G(2)},
			{B4+G(3)},
			{C5+G(4)},
			{G(5)}
		};

		arma::vec::fixed<6> T;
		arma::vec::fixed<6> M_DDtheta=Inertia_mat*DD_theta_controller;
		T.subvec(0,2)=M_DDtheta.subvec(0,2)+BCG.subvec(0,2);
		T.subvec(3,5)=BCG.subvec(3,5);
		return T;
	}

	static arma::vec::fixed<6> to_angular_vec(
		const arma::vec::fixed<6> T,
		const coriolis_type B,
		const arma::mat C,
		const arma::mat G,
		const arma::mat theta_D
		)
	{
		const double B1=B.b112*theta_D(0)*theta_D(1)+B.b113*theta_D(0)*theta_D(2)+B.b123*theta_D(1)*theta_D(2);
		const double B2=B.b223*theta_D(1)*theta_D(2);
		const double B4=B.b412*theta_D(0)*theta_D(1)+B.b413*theta_D(0)*theta_D(2);

		const arma::mat theta_D_P2=theta_D % theta_D;

		const double C1=theta_D_P2(1)*C(0,1)+theta_D_P2(2)*C(0,2);
		const double C2=theta_D_P2(0)*C(1,0)+theta_D_P2(2)*C(1,2);
		const double C3=theta_D_P2(0)*C(2,0)+theta_D_P2(1)*C(2,1);
		const double C5=theta_D_P2(0)*C(4,0)+theta_D_P2(1)*C(4,1);

		const arma::vec::fixed<6> BCG={
			{B1+C1+G(0)},
			{B2+C2+G(1)},
			{C3+G(2)},
			{B4+G(3)},
			{C5+G(4)},
			{G(5)}
		};

		const arma::vec::fixed<6> angle_vector=-BCG+T;

		return angle_vector;
	}

	static arma::vec::fixed<3> angle_to_position(arma::vec::fixed<3> angle)
	{
		const double theta1=angle(0);
		const double theta2=angle(1);
		const double theta3=angle(2);

		double x,y,z;
		x = cos(theta1)*(-0.4331*sin(theta2+theta3)-0.0203*cos(theta2+theta3)+0.4318*cos(theta2))+0.0934*sin(theta1);
		y = sin(theta1)*(-0.4331*sin(theta2+theta3)-0.0203*cos(theta2+theta3)+0.4318*cos(theta2))-0.0934*sin(theta1);
		z = -0.4331*cos(theta2+theta3)+0.0203*sin(theta2+theta3)-0.4318*sin(theta2);

		arma::vec::fixed<3> position={{x},{y},{z}};
		return position;
	}

	// template<int N,int M>
	static arma::vec::fixed<3> derivative_range3(const intermediate_type vec_now,const intermediate_type vec_last_observed,const std::size_t index_from,const std::size_t index_to,const double t,const double last_observed_t)
	{
		double dt=t-last_observed_t;
		arma::vec::fixed<3> D_vec;
		if(dt==0.0)
		{
			D_vec=arma::zeros<vec>(index_to-index_from+1);
		}
		else
			D_vec=(vec_now.subvec(index_from,index_to)-vec_last_observed.subvec(index_from,index_to))/dt;
		return D_vec;
	}


protected:

};

const double Model::I1=1.43+0*0.05;
const double Model::I2=1.75+0*0.07;
const double Model::I3=1.38+0*0.05;
const double Model::I4=0.69+0*0.02;
const double Model::I5=0.372+0*0.031;
const double Model::I6=0.333+0*0.016;
const double Model::I7=0.298+0*0.029;
const double Model::I8=-0.134+0*0.014;
const double Model::I9=0.0238+0*0.012;
const double Model::I10=-0.0213+0*0.0022;
const double Model::I11=-0.0142+0*0.0070;
const double Model::I12=-0.011+0*0.0011;
const double Model::I13=-0.00379+0*0.0009;
const double Model::I14=0.00164+0*0.000070;
const double Model::I15=0.00125+0*0.0003;
const double Model::I16=0.00124+0*0.0003;
const double Model::I17=0.000642+0*0.0003;
const double Model::I18=0.000431+0*0.00013;
const double Model::I19=0.0003+0*0.0014;
const double Model::I20=-0.000202+0*0.0008;
const double Model::I21=-0.0001+0*0.0006;
const double Model::I22=-0.000058+0*0.000015;
const double Model::I23=0.00004+0*0.00002;
const double Model::Im1=1.14+0*0.27;
const double Model::Im2=4.71+0*0.54;
const double Model::Im3=0.827+0*0.093;
const double Model::Im4=0.2+0*0.016;
const double Model::Im5=0.179+0*0.014;
const double Model::Im6=0.193+0*0.016;
const double Model::g1=-37.2+0*0.5;
const double Model::g2=-8.44+0*0.2;
const double Model::g3=1.02+0*0.5;
const double Model::g4=0.249+0*0.025;
const double Model::g5=-0.0282+0*0.0056;

#include "autogenerated/model_observer.hpp"
#include "model_mids.hpp"
#include "model_rhs.hpp"
