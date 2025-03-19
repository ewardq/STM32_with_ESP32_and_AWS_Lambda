import json
import boto3
from datetime import datetime

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('DeviceData')

def lambda_handler(event, context):
    try:
        # Parse the incoming data
        body = json.loads(event['body'])
        device_id = body['device_id']
        temperature = body['temperature']
        humidity = body['humidity']
        timestamp = int(datetime.now().timestamp())

        # Insert data into DynamoDB
        table.put_item(
            Item={
                'device_id': device_id,
                'timestamp': timestamp,
                'temperature': temperature,
                'humidity': humidity
            }
        )

        return {
            'statusCode': 200,
            'body': json.dumps({'message': 'Data inserted successfully'})
        }
    
    except Exception as e:
        return {
            'statusCode': 400,
            'body': json.dumps({'error': str(e)})
        }
