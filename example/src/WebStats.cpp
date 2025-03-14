/*
 * WebStats.cpp
 *
 *  Created on: 13 Mar 2025
 *      Author: jondurrant
 */

#include "WebStats.h"
#include <cstdio>

WebStats * WebStats::pSingleton = NULL;

WebStats * WebStats::getInstance(){
	if (WebStats::pSingleton == NULL){
		WebStats::pSingleton = new WebStats;
	}
	return WebStats::pSingleton;
}

WebStats::WebStats() {
	// TODO Auto-generated constructor stub

}

WebStats::~WebStats() {
	// TODO Auto-generated destructor stub
}

void WebStats::connectToAP(const char *sid, const char *password){
	cyw43_arch_init();
	cyw43_arch_enable_sta_mode();
	cyw43_arch_wifi_connect_blocking(
			sid,
			password,
			CYW43_AUTH_WPA2_AES_PSK
	);
	printf("Connected with  IP Address: %s\n",
			ipaddr_ntoa(
				netif_ip4_addr(&cyw43_state.netif[0])
			)
	);
}

/***
 * Start the task
 * @param priority - priority - 0 is idle
 * @return
 */
 bool WebStats::start(UBaseType_t priority){
	 BaseType_t res;

	res = xTaskCreate(
			WebStats::vTask,
			WEBSTATS_NAME,
			WEBSTATS_STACK,
			( void * ) this,
			priority,/* Priority at which the task is created. */
			&xHandle
	);
	return (res == pdPASS);
 }

void WebStats::stop(){
	if (xHandle != NULL){
		vTaskDelete(  xHandle );
		xHandle = NULL;
	}
}


void WebStats::vTask( void * pvParameters ){
	WebStats *task = (WebStats *) pvParameters;
	 if (task != NULL){
		 task->run();
	 }
}


 void WebStats::run(){
	struct mg_mgr mgr;        // Initialise Mongoose event manager
	mg_mgr_init(&mgr);        // and attach it to the interface
	mg_log_set(MG_LL_DEBUG);  // Set log level

	MG_INFO(("Initialising application..."));
	mg_http_listen(&mgr, HTTP_URL, WebStats::mgCB, this);

	MG_INFO(("Starting event loop"));
	for (;;) {
		mg_mgr_poll(&mgr, 10);
	}
}

 void WebStats::mgCB(struct mg_connection *c, int ev, void *ev_data){
 	if (ev == MG_EV_HTTP_MSG){
 		   printf("GET REQUEST\n");
 			struct mg_http_message *hm = (struct mg_http_message *) ev_data;
 			if (mg_match(hm->uri, mg_str("/metrics/taskStats"), NULL)) {
 				printf("GET taskStats\n");
 				mg_http_reply(c, 200,
 						"Content-Type: application/json\r\n",
						WebStats::getInstance()->getTaskStats()
				);
 			} else if (mg_match(hm->uri, mg_str("/metrics/heapStats"), NULL)) {
 				printf("GET heapStats\n");
 				mg_http_reply(c, 200,
 						"Content-Type: application/json\r\n",
						WebStats::getInstance()->getHeapStats()
				);
 			} else if (mg_match(hm->uri, mg_str("/metrics/msgs"), NULL)) {
 				printf("GET msg\n");
 				mg_http_reply(c, 200, "Content-Type: application/json\r\n", "[\"test\"]");
			}  else {
				printf("GET UNKNOWN\n");
				mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"result\": %m}", MG_ESC("Hello World"));
			}
 			c->recv.len = 0;     // Clean receive buffer
 			c->is_draining = 1;  // Close this connection when the response is sent;
 		}
 }

 char * WebStats::getHeapStats(){
	 HeapStats_t heapStats;
	vPortGetHeapStats(&heapStats);
	sprintf(xBuffer,
			"[ %d,  %d,  %d,  %d]",
		   heapStats.xAvailableHeapSpaceInBytes,
		   heapStats.xNumberOfFreeBlocks,
		   heapStats.xNumberOfSuccessfulAllocations,
		   heapStats.xNumberOfSuccessfulFrees
		   );
	return xBuffer;
 }

 char * WebStats::getTaskStats(){
	 TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime;

	// Get number of takss
	uxArraySize = uxTaskGetNumberOfTasks();

	//Allocate a TaskStatus_t structure for each task.
	pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

	strcpy(xBuffer, "[");
	char * out = &xBuffer[strlen(xBuffer)];

	if( pxTaskStatusArray != NULL ){
		// Generate raw status information about each task.
		uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
								  uxArraySize,
								  &ulTotalRunTime );

		 for( x = 0; x < uxArraySize; x++ )
		 {
			sprintf(out, "[\"%s\",%d, %d, %d, %d, %d],",
					pxTaskStatusArray[ x ].pcTaskName,
					pxTaskStatusArray[ x ].xTaskNumber ,
					pxTaskStatusArray[ x ].uxCurrentPriority ,
					pxTaskStatusArray[ x ].uxBasePriority ,
					pxTaskStatusArray[ x ].usStackHighWaterMark,
					pxTaskStatusArray[ x ].ulRunTimeCounter
					);
			out = &out[strlen(out)];
		 }
		 if (uxArraySize > 0){
			 //Switch last comma to close bracket
			 out[strlen(out)-1] = ']';
		 } else {
			 strcpy(xBuffer, "[]");
		 }

		   // Free array
		   vPortFree( pxTaskStatusArray );
	}
	return xBuffer;
 }
