#ifndef _SCHEDULE_

#define _SCHEDULE_

#include <vector>
#include <iomanip>
#include <cmath>
#include "adj_list.hpp"
#include "task.hpp"

const unsigned int MAX_DUR = 100; //100ms
const double delta = 0.000001;

typedef struct {
	double start;
	task t;
	double freq;
} slot;

class schedule {
private:
	std::vector<slot> HP, LP;
	double lp_time, hp_time;

	void display(std::ofstream &stream, std::vector<slot> &P, char type) {
		for (unsigned int i = 0; i < P.size(); ++i) {
			if (i != 0) stream << ' ';
			double size;
			if (type == 'L') {
				size = P[i].t.lp_size() * f_LP_max/P[i].freq;;
			} else {
				size = P[i].t.size() * f_HP_max/P[i].freq;;
			}
			if (P[i].t.get_id() >= MAX_TASKs) {
				stream << 2*MAX_TASKs - 1 - P[i].t.get_id() << "' ";
			} else {
				stream << P[i].t.get_id() << ' ';
			}
			stream << P[i].start << ' ' << size << ' ' << P[i].freq;
		}
		stream << std::endl;
	}

	double energy_for(std::vector<slot> &P, double P_idle, char type, double &ended) {
		double E = 0.0;
		for (unsigned int i = 0; i < P.size(); ++i) {
			if (ended < P[i].start) {
				E += (P[i].start - ended) * P_idle;
			}
			double size;
			if (type == 'L') {
				size = P[i].t.lp_size() * f_LP_max/P[i].freq;
				E += size * P[i].t.power_lp(P[i].freq);
			} else {
				size = P[i].t.size() * f_HP_max/P[i].freq;
				E += size * P[i].t.power_hp(P[i].freq);
			}
			ended = P[i].start + size;
		}
		return E;
	}


public:
	schedule() {
		lp_time = hp_time = 0.0;
	}

	void wait() {
		lp_time = hp_time = std::max(lp_time, hp_time);
	}

	double lpt() const { return lp_time; }
	double hpt() const { return hp_time; }

	void add_HP_slot(task t, double freq=f_HP_max) {
		HP.push_back({hp_time, t, freq});
		hp_time += t.size() * f_HP_max/freq;
	}

	void add_LP_slot(task t, double freq=f_LP_max) {
		LP.push_back({lp_time, t, freq});
		lp_time += t.lp_size() * f_LP_max/freq;
	}

	double energy() {
		double E = 0.0;

		if (LP.size() != 0 && HP.size() != 0) {
			double lp_started = LP[0].start,
			       hp_started = HP[0].start;
			if (hp_started > lp_started) {
				E += (lp_started - hp_started) * P_HP_idle;
			} else if (lp_started > hp_started) {
				E += (hp_started - lp_started) * P_LP_idle;
			}
		}

		double lp_ended = 0.0, hp_ended = 0.0;
		E += energy_for(LP, P_LP_idle, 'L', lp_ended)
		  +  energy_for(HP, P_HP_idle, 'H', hp_ended);

		if (lp_ended > hp_ended) {
			E += (lp_ended - hp_ended) * P_HP_idle;
		} else if (hp_ended > lp_ended) {
			E += (hp_ended - lp_ended) * P_LP_idle;
		}
		return E;
	}

	void show(std::ofstream &stream) {
		display(stream, HP, 'H');
		display(stream, LP, 'L');
		stream << std::setprecision(15) << std::fixed << energy() << std::endl;
	}

	friend schedule uniform_scale(const schedule &);
	friend schedule contingency_schedule(const schedule &);
};

std::vector<int> parent_task_count(const adj_list &);

// Largest Task First
schedule ltf_schedule(const std::vector<task> &, const adj_list &);

// Threshold-based List Scheduling
schedule tbls_schedule(const std::vector<task> &, const adj_list &, double);

#endif
