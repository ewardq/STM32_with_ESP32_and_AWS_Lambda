# Embedded IoT system with RESTful API — From STM32 drivers to AWS Lambda 

![IoT_with_RESTful](https://github.com/user-attachments/assets/5965e42b-532f-4b4b-9656-5ec436253da1)

In this IoT system, the AWS Lambda functions serve as the main processor of incoming data from IoT devices, such as temperature or humidity readings. An API Gateway Lambda function can be triggered when a device sends an HTTP POST request with sensor data. The function can:

• Parse the JSON payload from the request.

• Extract the sensor data (e.g., temperature, humidity, timestamp).

• Store the data in DynamoDB for long-term storage and analysis.
