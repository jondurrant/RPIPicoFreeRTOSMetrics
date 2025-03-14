/*
 * WebStats.h
 *
 *  Created on: 13 Mar 2025
 *      Author: jondurrant
 */

#ifndef EXAMPLE_SRC_WEBSTATS_H_
#define EXAMPLE_SRC_WEBSTATS_H_

#define WEBSTATS_NAME "WebStats"
#define WEBSTATS_STACK (1024*1)

#include "FreeRTOS.h"
#include "task.h"
#include <pico/stdlib.h>
#include "pico/cyw43_arch.h"

#include "mongoose.h"

#define HTTP_URL "http://0.0.0.0:80"
#define WEBSTATS_BUFFER_SIZE 1024

class WebStats {
public:

	static WebStats * getInstance();

	virtual ~WebStats();

	void connectToAP(const char *sid, const char *password);

	/***
	 * Start the task
	 * @param priority - priority - 0 is idle
	 * @return
	 */
	 bool start(UBaseType_t priority = tskIDLE_PRIORITY);

	/***
	 * Stop task
	 * @return
	 */
	void stop();


private:
	static WebStats * pSingleton;
	WebStats();
	/***
	 * Start the task via static function
	 * @param pvParameters - will be the Agent object
	 */
	static void vTask( void * pvParameters );

	/***
	 * Task main run loop
	 */
	 void run();

	 static void mgCB(struct mg_connection *c, int ev, void *ev_data);

	 char * getHeapStats();

	 char * getTaskStats();

	TaskHandle_t xHandle = NULL;

	char xBuffer[WEBSTATS_BUFFER_SIZE];

};

#endif /* EXAMPLE_SRC_WEBSTATS_H_ */
