#include "things5.h"

Things5::Things5(bool timestamp_en) {
	_timestamp_enabled = timestamp_en;
	_doc.clear();
}

void Things5::setUUID(void) {
	// Generating UUID v4
	uint8_t uuid_array[16];
	ESPRandom::uuid4(uuid_array);

	_doc.clear();    // Clear document
	_doc["request_id"] = ESPRandom::uuidToString(uuid_array);
}

void Things5::setProperty(String key, String value) {
	_doc[key] = value;
}

//=================================================================================================
//===================================   THINGS5 METRIC METHODS   ==================================
//=================================================================================================

// Define a new metric [label] of integer/float [type] ------------------------
void Things5::defMetric(const char * label, metrics_t type) {
	switch(type) {
		case METRICS_INT:
			if(num_metrics_int < T5_MAX_NUM_VAR) {
				strcpy(metrics_int[num_metrics_int].label, label);
				metrics_int[num_metrics_int].value = 0;
				num_metrics_int++;
			}
			break;
		case METRICS_FLT:
			if(num_metrics_flt < T5_MAX_NUM_VAR) {
				strcpy(metrics_flt[num_metrics_flt].label, label);
				metrics_flt[num_metrics_flt].value = 0;
				num_metrics_flt++;
			}
			break;
		default:
			break;
	}
}

// Return the index of the metric [label] -------------------------------------
// return -1 if the metric doesn't exists -------------------------------------
int8_t Things5::findMetric(const char * label, metrics_t type) {
	int8_t index = -1;
	switch(type) {
		case METRICS_INT:
			for(uint8_t i=0 ; i<num_metrics_int ; i++) {
				if(!strcmp(metrics_int[i].label, label)) {
					index = i;
					break;
				}
			}
			break;
		case METRICS_FLT:
			for(uint8_t i=0 ; i<num_metrics_flt ; i++) {
				if(!strcmp(metrics_flt[i].label, label)) {
					index = i;
					break;
				}
			}
			break;
		default:
			break;
	}
	return index;
}

// Initialize metrics object --------------------------------------------------
void Things5::initMetrics(unsigned long long timestamp) {
	if(_doc.containsKey("metrics"))
		return;

    JsonArray metrics = _doc.createNestedArray("metrics");
    JsonObject metrics_0 = metrics.createNestedObject();
    if(_timestamp_enabled)
        metrics_0["timestamp"] = timestamp;
}

// Update metric [label] with integer [value] ---------------------------------
// return "true" if value has changed -----------------------------------------
bool Things5::updateMetric(const char * label, int32_t value) {
	int8_t index = findMetric(label, METRICS_INT);
	if(index > -1) {
		if(metrics_int[index].value != value) {
			metrics_int[index].value = value;
			#ifdef LOG_DEBUG_T5
			console.log(T5_T, "Metric \"" + String(label) + "\" has changed");
			#endif
			if(_building_msg) {
				initMetrics(_timestamp);
				addMetric(String(label), value);
			}
			return true;
		}
		return false;
	}
	#ifdef LOG_DEBUG_T5
	console.error(T5_T, "Metric not found");
	#endif
	return false;
}

// Update metric [label] with float [value] -----------------------------------
// return "true" if value has changed -----------------------------------------
bool Things5::updateMetric(const char * label, float value) {
	int8_t index = findMetric(label, METRICS_FLT);
	float round_value = roundf(value * 10)/10;
	if(index > -1) {
		if(metrics_flt[index].value != round_value) {
			metrics_flt[index].value = round_value;
			#ifdef LOG_DEBUG_T5
			console.log(T5_T, "Metric \"" + String(label) + "\" has changed: " + String(round_value));
			#endif
			if(_building_msg) {
				initMetrics(_timestamp);
				addMetric(String(label), round_value);
			}
			return true;
		}
		return false;
	}
    #ifdef LOG_DEBUG_T5
    console.error(T5_T, "Metric not found");
    #endif
    return false;
}

// Add metric [label] with signed integer [value] -----------------------------
void Things5::addMetric(String label, int32_t value) {
	JsonArray metrics_0_data;
	if(_doc["metrics"][0]["data"].isNull())
		metrics_0_data = _doc["metrics"][0].createNestedArray("data");
	else
		metrics_0_data = _doc["metrics"][0]["data"];

	StaticJsonDocument<64> obj;
	obj["name"] = label;
	obj["value"] = String(value);
	metrics_0_data.add(obj);
	obj.clear();
}

// Add metric [label] with unsigned integer [value] ---------------------------
void Things5::addMetric(String label, uint32_t value) {
	JsonArray metrics_0_data;
	if(_doc["metrics"][0]["data"].isNull())
		metrics_0_data = _doc["metrics"][0].createNestedArray("data");
	else
		metrics_0_data = _doc["metrics"][0]["data"];

	StaticJsonDocument<64> obj;
	obj["name"] = label;
	obj["value"] = String(value);
	metrics_0_data.add(obj);
	obj.clear();
}

// Add metric [label] with float integer [value] ------------------------------
void Things5::addMetric(String label, float value) {
	JsonArray metrics_0_data;
	if(_doc["metrics"][0]["data"].isNull())
		metrics_0_data = _doc["metrics"][0].createNestedArray("data");
	else
		metrics_0_data = _doc["metrics"][0]["data"];

	StaticJsonDocument<64> obj;
	obj["name"] = label;
	obj["value"] = String(value);
	metrics_0_data.add(obj);
	obj.clear();
}

//=================================================================================================
//==============================   THINGS5 GENERAL PURPOSE METHODS   ==============================
//=================================================================================================

// Create a new Things5 empty message 
void Things5::createMessage(unsigned long long timestamp) {
	setUUID();
	_timestamp = timestamp;
	_building_msg = true;
}

void Things5::createMessage(void) {
	setUUID();
	_building_msg = true;
}

String Things5::getPayload(void) {
	String payload;
	serializeJson(_doc, payload);
	_doc.clear();
	_building_msg = false;
	return payload;
}