#pragma once

namespace physics
{
	// Must be equal to the number of state pairs
	const std::size_t N=100;
	const double L_total=0.1;
	const double C_total=0.1;
	const double R_total=0.1;
	const double G_total=0.1;
	const double L=L_total/N;
	const double C=C_total/N;
	const double R=R_total/N;
	const double G=G_total/N;
}
