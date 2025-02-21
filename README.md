# Embedded IoT system with RESTful API — From STM32 drivers to AWS Lambda 

![IoT_with_RESTful](https://github.com/user-attachments/assets/a1c34c7b-0677-4fec-adbd-b4e79e4bde2c)

In this IoT system, the AWS Lambda functions serve as the main processor of incoming data from IoT devices, such as temperature or humidity readings. An API Gateway Lambda function can be triggered when a device sends an HTTP POST request with sensor data. The function can:

• Parse the JSON payload from the request.

• Extract the sensor data (e.g., temperature, humidity, timestamp).

• Store the data in DynamoDB for long-term storage and analysis.
