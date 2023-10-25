
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Settings
static const uint8_t msg_queue_len = 5;

//Globals
static QueueHandle_t msg_queue;

//*****************************************************************************
//Tasks
void printMessages(void *parameters) {

  int item;
  while (1) {
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE) {
      Serial.println(item);
    }

    //    Delay
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Demo---");

  //Create queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  // Start Serial receive task
  xTaskCreatePinnedToCore(printMessages,
                          "Read Serial",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
}

void loop()  {

  static int num = 0;
  //  Try to add item queue for 10 ticks,fails i queue is full
  if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
    Serial.println("Queue Full");
    //    Serial.println(num);
  }
  num++;

  //    Delay
  vTaskDelay(1000 / portTICK_PERIOD_MS);

}
