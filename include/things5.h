#ifndef THINGS5_H_
#define THINGS5_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPRandom.h>
#include <console.h>

#define T5_T 				"T5"

#define JSON_SIZE					1024	// JSON maximum size
#define T5_LABEL_MAX_LENGTH	20		// Label maximum length
#define T5_MAX_NUM_VAR			8		// Metrics/State/Events array maximum length

typedef enum {
	METRICS_INT = 0,
	METRICS_FLT,
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
		DynamicJsonDocument _doc = DynamicJsonDocument(JSON_SIZE);
		bool _timestamp_enabled;
		unsigned long long _timestamp;
		metrics_int_t metrics_int[T5_MAX_NUM_VAR];
		metrics_flt_t metrics_flt[T5_MAX_NUM_VAR];
		t5_states_t states[T5_MAX_NUM_VAR];
		t5_events_t events[T5_MAX_NUM_VAR];
		uint8_t num_metrics_int;
		uint8_t num_metrics_flt;
		uint8_t num_states;
		uint8_t num_events;
		bool _building_msg;
	public:
		Things5(bool timestamp_en = false);
		void setUUID(void);
		void setProperty(String key, String value);
		// Things5 Metrics methods ----------------------------------------------
		void defMetric(const char * label, metrics_t type);
		int8_t findMetric(const char * label, metrics_t type);
		void initMetrics(unsigned long long timestamp);
		bool updateMetric(const char * label, int32_t value);
		bool updateMetric(const char * label, float value);
		void addMetric(String label, int32_t value);
		void addMetric(String label, uint32_t value);
		void addMetric(String label, float value);
		// Things5 general purpose methods --------------------------------------
		void createMessage(unsigned long long timestamp);
		void createMessage(void);
		void deleteMessage(void);
		String getPayload(void);
		bool isEmptyMessage(void);
};

#endif  /* THINGS5_H_ */