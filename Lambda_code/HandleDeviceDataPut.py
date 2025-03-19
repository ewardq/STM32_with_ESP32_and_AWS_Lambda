import json
import boto3
from decimal import Decimal
from datetime import datetime

# Initialize DynamoDB resource and table
dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('DeviceData')

# Custom JSON encoder to handle Decimal types
class DecimalEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, Decimal):
            return float(obj)   # Convert Decimal to float for JSON serialization
        return super(DecimalEncoder, self).default(obj)

def lambda_handler(event, context):
    try:
        # Parse the request body
        body = json.loads(event['body'])
        device_id = body['device_id']
        temperature = Decimal(str(body.get('temperature')))
        humidity = Decimal(str(body.get('humidity')))
        if(body.get('timestamp') is not None):
            timestamp = int(body.get('timestamp'))
        else:
            timestamp = int(datetime.now().timestamp()) # Generate timestamp within Lambda

        # Update the item in DynamoDB
        response = table.update_item(
            Key = {
                'device_id': device_id,
                'timestamp': timestamp
            },
            UpdateExpression = 'SET temperature = :temp, humidity = :hum',
            ExpressionAttributeValues = {
                ':temp': temperature,
                ':hum': humidity
            },
            ReturnValues = 'UPDATED_NEW'
        )

        # Return success response
        return {
            'statusCode': 200,
            'body': json.dumps({
                'message': 'Data updated successfully',
                'updatedAttributes': response ['Attributes']
            }, cls = DecimalEncoder)     # Use custom encoder here
        }


    except KeyError as e:
        # Handle missing required fields
        return {
            'statusCode': 400,
            'body': json.dumps({'error': f'Missing required key: {str(e)}'})
        }

    except Exception as e:
        # Handle other exceptions
        return {
            'statusCode': 500,
            'body': json.dumps({'error': str(e)})
        }
