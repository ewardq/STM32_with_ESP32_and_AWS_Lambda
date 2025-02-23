# Embedded IoT system with RESTful API — From STM32 drivers to AWS Lambda 

![IoT_with_RESTful](https://github.com/user-attachments/assets/a1c34c7b-0677-4fec-adbd-b4e79e4bde2c)

In this IoT system, the AWS Lambda functions serve as the main processor of incoming data from IoT devices, such as temperature or humidity readings. An API Gateway Lambda function can be triggered when a device sends an HTTP POST request with sensor data. The function can:

• Parse the JSON payload from the request.

• Extract the sensor data (e.g., temperature, humidity, timestamp).

• Store the data in DynamoDB for long-term storage and analysis.

## How to use this project
The STM32<->ESP32 part of this project is better developed in this repositroy: https://github.com/ewardq/STM32_Drivers

This repository uses the previously mentioned drivers to send HTTP Request to AWS Lambda.

The complete library is in the 9_ESP32_AWS_client folder, the other folders included in this repo are just examples of how to execute POST, GET, PUT and DELETE requests.

- HTTP POST request to AWS Lambda and to then update a DynamoDB database.
- HTTP GET request to retrieve data from DynamoDB.
- HTTP PUT request to AWS Lambda.
- HTTP DELETE request to remove data from DynamoDB.
