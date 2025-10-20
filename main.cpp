#include <iostream>
#include <cmath>
#include <iomanip>

struct capacitor
{
	double* time;
	double* voltage;
	double* current;
	double C;
}; typedef struct capacitor Capacitor;

void initalize_capacitor(Capacitor* cap, int num_timesteps, double capacitance);
void cleanup_capacitor(Capacitor* cap);
void output_results(const Capacitor* cap, int timestep, bool is_current_source);
void solve_constant_current_source(Capacitor* cap, double I, double total_time, int num_timesteps);
void solve_constant_voltage_source(Capacitor* cap, double V, double total_time, int num_timesteps);




int main() {
	const int num_timesteps = 50000;
	const double total_time = 5e-6; // seconds
	const double capacitance = 100.0e-12; // Farads
	const double constant_current = 1.0e-2; // Amperes
	const double constant_voltage = 10.0; // Volts

	Capacitor* capacitor_current = new Capacitor;
	Capacitor* capacitor_voltage = new Capacitor;

	capacitor_current->time = new double[num_timesteps];
	capacitor_current->voltage = new double[num_timesteps];
	capacitor_current->current = new double[num_timesteps];
	capacitor_current->C = capacitance;

	capacitor_voltage->time = new double[num_timesteps];
	capacitor_voltage->voltage = new double[num_timesteps];
	capacitor_voltage->current = new double[num_timesteps];
	capacitor_voltage->C = capacitance;
	
	initalize_capacitor(capacitor_current, num_timesteps, capacitance);
	solve_constant_current_source(capacitor_current, constant_current, total_time, num_timesteps);
	cleanup_capacitor(capacitor_current);
	initalize_capacitor(capacitor_voltage, num_timesteps, capacitance);
	solve_constant_voltage_source(capacitor_voltage, constant_voltage, total_time, num_timesteps);
	cleanup_capacitor(capacitor_voltage);
	return 0;
}

void initalize_capacitor(Capacitor* cap, int num_timesteps, double capacitance) {
	cap->time = new double[num_timesteps];
	cap->voltage = new double[num_timesteps];
	cap->current = new double[num_timesteps];
	cap->C = capacitance;
}

void cleanup_capacitor(Capacitor* cap) {
	delete[] cap->time;
	delete[] cap->voltage;
	delete[] cap->current;

	cap->time = nullptr;
	cap->voltage = nullptr;
	cap->current = nullptr;
}

void output_results(const Capacitor* cap, int timestep, bool is_current_source) {
	if (timestep % 200 == 0) {
		if (is_current_source) {
			std::cout << std::fixed << std::setprecision(10) << "Time(s)" << cap->time[timestep] << "Voltage(V)" << cap->voltage[timestep] << "Current(A)" << cap->current[timestep] << " A" << std::endl;
		}
		else {
			std::cout << std::fixed << std::setprecision(10) << "Time(s)" << cap->time[timestep] << "Voltage(V)" << cap->voltage[timestep] << "Current(A)" << cap->current[timestep] << " A" << std::endl;
		}
	}
}

void solve_constant_current_source(Capacitor* cap, double I, double total_time, int num_timesteps) {
	double dt = total_time / num_timesteps;
	for (int n = 0; n < num_timesteps; ++n) {
		cap->time[n] = n * dt;
		if (n == 0) {
			cap->voltage[n] = 0.0;
		}
		else {
			cap->voltage[n] = cap->voltage[n - 1] + (I / cap->C) * dt;
		}
		cap->current[n] = I;
		output_results(cap, n, true);
	}
}

void solve_constant_voltage_source(Capacitor* cap, double V, double total_time, int num_timesteps) {
	double dt = total_time / num_timesteps;
	for (int n = 0; n < num_timesteps; ++n) {
		cap->time[n] = n * dt;
		cap->voltage[n] = V;
		if (n == 0) {
			cap->current[n] = 0.0;
		}
		else {
			cap->current[n] = cap->C * (cap->voltage[n] - cap->voltage[n - 1]) / dt;
		}
		output_results(cap, n, false);
	}
}