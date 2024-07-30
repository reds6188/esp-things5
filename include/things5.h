#ifndef THINGS5_H_
#define THINGS5_H_

#include <Arduino.h>

#define T5_T 				"T5"

#define T5_LABEL_MAX_LENGTH	20		// Label maximum length
#define T5_MAX_NUM_VAR		8		// Metrics/State/Events array maximum length

typedef enum {
	METRICS_INT = 0,
	METRICS_FLT,
	METRICS_STR
}metrics_t;

typedef struct {
	char label[T5_LABEL_MAX_LENGTH];
	int32_t value;
}metrics_int_t;

typedef struct {
	char label[T5_LABEL_MAX_LENGTH];
	float value;
}metrics_flt_t;

typedef struct {
	char label[T5_LABEL_MAX_LENGTH];
	String value;
}t5_states_t;

typedef struct {
	char label[T5_LABEL_MAX_LENGTH];
	bool value;
}t5_events_t;

class Things5 {
	private:
		bool _timestamp_enabled;
		unsigned long long timestamp;
		metrics_int_t metrics_int[T5_MAX_NUM_VAR];
		metrics_flt_t metrics_flt[T5_MAX_NUM_VAR];
		t5_states_t states[T5_MAX_NUM_VAR];
		t5_events_t events[T5_MAX_NUM_VAR];
		uint8_t num_metrics_int;
		uint8_t num_metrics_flt;
		uint8_t num_states;
		uint8_t num_events;
	public:
		Things5(bool timestamp_en = false);
};

#endif  /* THINGS5_H_ */