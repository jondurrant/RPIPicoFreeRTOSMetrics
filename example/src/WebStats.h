/*
 * WebStats.h
 *
 * Provide FreeRTOS Runtime stats to a Web Console via Mongoose
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
#include "message_buffer.h"
#include <pico/stdlib.h>
#include "pico/cyw43_arch.h"

#include "mongoose.h"

#define HTTP_URL "http://0.0.0.0:8080"
#define WEBSTATS_BUFFER_SIZE 1024
#define WEBSTATS_MSG_BUFFER_SIZE 1024

class WebStats {
public:

	/**
	 * Get Instance as Singleton
	 * @return Singleton
	 */
	static WebStats * getInstance();

	virtual ~WebStats();

	/***
	 * Connection to Wifi AP - Your app might already be doing this in which case no need to call
	 * @param sid of WiFi
	 * @param password of Wifi
	 */
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

	/***
	 * Add a message for display on the Web Console
	 * @param msg
	 * @return
	 */
	size_t queueMsg(const char * msg);


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

	 char * getMsgs();

	TaskHandle_t xHandle = NULL;

	char xBuffer[WEBSTATS_BUFFER_SIZE];

	MessageBufferHandle_t xMsgBuffer;

};

#endif /* EXAMPLE_SRC_WEBSTATS_H_ */
